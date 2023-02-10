#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cmath>

#include <vector>
#include <tuple>
#include <utility>
#include <optional>

#include <algorithm>
#include <functional>

#include "include/Vector3Util.h"

#include "include/block.h"
#include "include/chunk.h"
#include "include/raycastRequest.h"
#include "include/consts.h"

#include "include/world.h"

using std::vector;
using std::tuple;
using std::pair;
using std::optional;


World::World() {
	this->chunks = unordered_map<string, Chunk>();
	this->chunkOrder = vector<string>();
	this->chunksToGenerate = vector<pair<tuple<int, int, int>, float>>();
}

optional<Chunk&> World::getChunkAt(tuple<int, int, int> chunkPos) {
	// Does this copy
	string key = TUP_TO_STR(chunkPos);
	auto iter = this->chunks.find(key);
	if (iter != this->chunks.end()) {
		auto iter2 = *iter;
		Chunk& chunk = iter2.second;
		return chunk;
	} else {
		return {};
	}
}
Block World::getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) {
	optional<Chunk&> possibleChunk = this->getChunkAt(chunkPos);
	if (possibleChunk.has_value()) {
		Chunk& chunk = possibleChunk.value();
		return chunk.getBlockAt(x, y, z);
	} else {
		return BEDROCK_BLOCK;
	}
}

void World::generateChunks(PerlinNoise& pn) {
	for (size_t i = 0; i < this->chunksToGenerate.size(); i++) {
		tuple<int, int, int> chunkTup = this->chunksToGenerate[i].first;
		string key = TUP_TO_STR(chunkTup);

		Chunk chunk = Chunk(chunkTup);
		chunk.generateBlocks(pn);
		chunk.distanceFromCamera = this->chunksToGenerate[i].second;

		// this->chunks.insert(std::make_pair(key, chunk));

		// TODO: copies chunk!!!!!!
		pair<string, Chunk> pair = std::make_pair(key, chunk);
		this->chunks.insert(pair);

		for (size_t j = 0; j < 6; j++) {
			tuple<int, int, int> dirTuple = allDirTuples[j];

			tuple<int, int, int> neighborTup = std::make_tuple(
				std::get<0>(chunkTup) + std::get<0>(dirTuple),
				std::get<1>(chunkTup) + std::get<1>(dirTuple),
				std::get<2>(chunkTup) + std::get<2>(dirTuple)
			);

			optional<Chunk&> possibleChunk = this->getChunkAt(neighborTup);
			if (possibleChunk.has_value()) {
				Chunk& neighbor = possibleChunk.value();
				neighbor.dirty = true;
			}
		}
	}
	for (size_t i = 0; i < this->chunksToGenerate.size(); i++) {
		tuple<int, int, int> chunkTup = this->chunksToGenerate[i].first;
		string key = TUP_TO_STR(chunkTup);
		Chunk& chunk = this->chunks.at(key);

		chunk.generateModel(*this);
		if (!chunk.blank || !chunk.transparentBlank) {
			this->chunkOrder.push_back(key);	
		}
	}
	this->chunksToGenerate.clear();
}
void World::updateChunkModels() {
	// reverse loop with unsigned type: https://stackoverflow.com/questions/3623263/reverse-iteration-with-an-unsigned-loop-variable
	for (size_t i = this->chunkOrder.size(); i-- > 0; ) {
		string chunkKey = this->chunkOrder[i];
		Chunk& chunk = this->chunks.at(chunkKey);
		if (chunk.dirty) {
			chunk.generateModel(*this);
			// return; // only update one chunk per frame
		}
	}
}



// bool World::inBounds(tuple<int, int, int> chunkPos) {
// 	string key = TUP_TO_STR(chunkPos);
// 	return this->chunks.find(key) != this->chunks.end();
// }

void World::dirtyNeighbors(tuple<int, int, int> srcChunk, tuple<int, int, int> srcBlock) {
	int chunkX = std::get<0>(srcChunk);
	int chunkY = std::get<1>(srcChunk);
	int chunkZ = std::get<2>(srcChunk);
	
	int blockX = std::get<0>(srcBlock);
	int blockY = std::get<1>(srcBlock);
	int blockZ = std::get<2>(srcBlock);

	// blockX == 0: 				(-1, 0, 0)
	// blockX == CHUNK_SIZE - 1:	(1, 0, 0)
	// blockY == 0:					(0, -1, 0)
	// blockY == CHUNK_SIZE - 1:	(0, 1, 0)
	// blockZ == 0:					(0, 0, -1)
	// blockZ == CHUNK_SIZE - 1:	(0, 0, 1)

	vector<tuple<int, int, int>> neighborChunks; // could inlcude out of bounds idxs
	neighborChunks.reserve(6);

	if (blockX == 0) {
		neighborChunks.push_back(std::make_tuple(chunkX - 1, chunkY, chunkZ));
	}
	else if (blockX == CHUNK_SIZE - 1) {
		neighborChunks.push_back(std::make_tuple(chunkX + 1, chunkY, chunkZ));
	}
	if (blockY == 0) {
		neighborChunks.push_back(std::make_tuple(chunkX, chunkY - 1, chunkZ));
	}
	else if (blockY == CHUNK_SIZE - 1) {
		neighborChunks.push_back(std::make_tuple(chunkX, chunkY + 1, chunkZ));
	}
	if (blockZ == 0) {
		neighborChunks.push_back(std::make_tuple(chunkX, chunkY, chunkZ - 1));
	}
	else if (blockZ == CHUNK_SIZE - 1) {
		neighborChunks.push_back(std::make_tuple(chunkX, chunkY, chunkZ + 1));
	}

	for (size_t i = 0; i < neighborChunks.size(); i++) {
		tuple<int, int, int> neighborChunk = neighborChunks[i];

		optional<Chunk&> possibleChunk = this->getChunkAt(neighborChunk);
		if (possibleChunk.has_value()) {
			Chunk& chunk = possibleChunk.value();
			chunk.dirty = true;
		}
	}
}

bool World::cameraIsSubmerged(const CameraController& cameraController) {
	tuple<int, int, int> chunkPos = cameraController.getChunkPos();

	optional<Chunk&> possibleChunk = this->getChunkAt(chunkPos);
	if (possibleChunk.has_value()) {
		Chunk& chunk = possibleChunk.value();

		int blockX = EUCMOD((int)cameraController.camera.position.x, CHUNK_SIZE);
		int blockY = EUCMOD((int)cameraController.camera.position.y, CHUNK_SIZE);
		int blockZ = EUCMOD((int)cameraController.camera.position.z, CHUNK_SIZE);

		Block block = chunk.getBlockAt(blockX, blockY, blockZ);

		return block.blockType == BlockType::WATER;
	} else {
		return false;
	}
}

void World::sortChunks(const CameraController& cameraController) {
	// sorted order (farthest to closest) saved in World::chunkOrder
	std::sort(
		this->chunkOrder.begin(),
		this->chunkOrder.end(),
		[&cameraController, this](const string& key1, const string& key2){
			Chunk& chunk1 = this->chunks.at(key1);
			Chunk& chunk2 = this->chunks.at(key2);

			return chunk1.distanceFromCamera > chunk2.distanceFromCamera;
		}
	);
}

void World::cameraMoved(const CameraController& cameraController, PerlinNoise& pn) {
	// TODO: faster way to do this?

	this->chunkOrder.clear();

	this->chunksToGenerate.clear();


	tuple<int, int, int> cameraChunk = cameraController.getChunkPos();
	int cameraChunkX = std::get<0>(cameraChunk);
	int cameraChunkY = std::get<1>(cameraChunk);
	int cameraChunkZ = std::get<2>(cameraChunk);

	int startX = cameraChunkX - VIEW_DIST;
	int startY = cameraChunkY - VIEW_DIST;
	int startZ = cameraChunkZ - VIEW_DIST;

	int endX = cameraChunkX + VIEW_DIST;
	int endY = cameraChunkY + VIEW_DIST;
	int endZ = cameraChunkZ + VIEW_DIST;

	for (int x = startX; x <= endX; x++) {
		for (int y = startY; y <= endY; y++) {
			for (int z = startZ; z <= endZ; z++) {

				tuple<int, int, int> idx = std::make_tuple(x, y, z);
				string key = TUP_TO_STR(idx);

				int diffX = x - cameraChunkX;
				int diffY = y - cameraChunkY;
				int diffZ = z - cameraChunkZ;

				float dist = sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);

				if (dist <= VIEW_DIST) {
					optional<Chunk&> possibleChunk = this->getChunkAt(idx);
					if (possibleChunk.has_value()) {
						Chunk& chunk = possibleChunk.value();
						chunk.distanceFromCamera = dist;

						if (!chunk.blank || !chunk.transparentBlank) {
							this->chunkOrder.push_back(key);	
						}
					} else {
						this->chunksToGenerate.push_back(std::make_pair(idx, dist));
					}
				}

			}
		}
	}

	this->generateChunks(pn); // must happen before sorting
	this->sortChunks(cameraController);
}

optional<Vector3> World::handleRaycastRequest(const CameraController& cameraController, RaycastRequest& raycastRequest, Block selectedBlock) {

	tuple<int, int, int> cameraChunk = cameraController.getChunkPos();
	int cameraChunkX = std::get<0>(cameraChunk);
	int cameraChunkY = std::get<1>(cameraChunk);
	int cameraChunkZ = std::get<2>(cameraChunk);

	vector<RayCollision> rayCollisions;
	vector<Chunk*> chunkCollisions;

	Ray ray = {
		cameraController.camera.position,
		cameraController.calcForward()
	};
	// TODO: use chunkOrder and break when distance is out of neighboring chunks
	for (int x = cameraChunkX - 1; x <= cameraChunkX + 1; x++) {
		for (int y = cameraChunkY - 1; y <= cameraChunkY + 1; y++) {
			for (int z = cameraChunkZ - 1; z <= cameraChunkZ + 1; z++) {

				tuple<int, int, int> idx = std::make_tuple(x, y, z);

				optional<Chunk&> possibleChunk = this->getChunkAt(idx);
				if (possibleChunk.has_value()) {
					Chunk& chunk = possibleChunk.value();

					if (chunk.blank) {
						continue;
					}

					Vector3 pos = chunk.getWorldPos();
					Matrix qTransform = MatrixTranslate(pos.x, pos.y, pos.z);
					RayCollision qRayCollision = GetRayCollisionMesh(ray, chunk.model.meshes[0], qTransform);
					if (qRayCollision.hit && qRayCollision.distance < REACH) {
						rayCollisions.push_back(qRayCollision);
						chunkCollisions.push_back(&chunk);
					}

				}

			}
		}
	}

	if (rayCollisions.size() > 0) {
		RayCollision closestRayCollision = rayCollisions[0];
		Chunk* closestChunkCollision = chunkCollisions[0];

		for (size_t i = 1; i < rayCollisions.size(); i++) {
			if (rayCollisions[i].distance < closestRayCollision.distance) {
				closestRayCollision = rayCollisions[i];
				closestChunkCollision = chunkCollisions[i];
			}
		}

		tuple<size_t, size_t, size_t> bestBlockTuple = closestChunkCollision->handleRayCollision(closestRayCollision);

		if (raycastRequest == RaycastRequest::DESTROY_BLOCK) {
			closestChunkCollision->destroyBlockAt(bestBlockTuple, *this);
		} else if (raycastRequest == RaycastRequest::PLACE_BLOCK) {
			closestChunkCollision->placeBlockAt(bestBlockTuple, closestRayCollision.normal, selectedBlock, *this);
		} else { // center of outlined block
			Vector3 bestBlockOutlinePos = (Vector3){
				(float)std::get<0>(bestBlockTuple),
				(float)std::get<1>(bestBlockTuple),
				(float)std::get<2>(bestBlockTuple)
			} + Vector3Uniform(0.5);
			return bestBlockOutlinePos + closestChunkCollision->getWorldPos();
		}
	}

	return {};
}
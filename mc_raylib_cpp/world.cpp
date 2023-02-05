#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cmath>

#include <vector>
#include <tuple>
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
using std::optional;


World::World() {
	this->chunks = unordered_map<string, Chunk>();

	this->chunkOrder = vector<tuple<int, int, int>>();
}

Chunk& World::getChunkAt(tuple<int, int, int> chunkPos) {
	Chunk& chunk = this->chunks.at(TUP_TO_STR(chunkPos));
	return chunk;
}
Block World::getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) {
	if (this->inBounds(chunkPos)) {
		Chunk& chunk = this->getChunkAt(chunkPos);
		return chunk.getBlockAt(x, y, z);
	}
	else {
		return AIR_BLOCK;
	}
}

void World::generateChunks(PerlinNoise& pn) {
	this->chunks.clear();
	this->chunkOrder.clear();

	for (size_t x = 0; x < WORLD_SIZE; x++) {
		for (size_t y = 0; y < WORLD_SIZE; y++) {
			for (size_t z = 0; z < WORLD_SIZE; z++) {

				tuple<int, int, int> chunkTup = std::make_tuple(x, y, z);
				string key = TUP_TO_STR(chunkTup);

				Chunk chunk = Chunk(chunkTup);
				chunk.generateBlocks(pn);
				
				this->chunks.insert(std::make_pair(key, chunk));
				this->chunkOrder.push_back(chunkTup);

			}
		}
	}
}
void World::updateChunkModels() {
	// reverse loop with unsigned type: https://stackoverflow.com/questions/3623263/reverse-iteration-with-an-unsigned-loop-variable
	for (size_t i = this->chunkOrder.size(); i-- > 0;) {
		tuple<int, int, int> chunkTup = this->chunkOrder[i];
		Chunk& chunk = this->getChunkAt(chunkTup);
		if (chunk.dirty) {
			chunk.generateModel(*this);
			// return; // only update one chunk per frame
		}
	}
}



bool World::inBounds(tuple<int, int, int> chunkPos) {
	string key = TUP_TO_STR(chunkPos);
	return this->chunks.find(key) != this->chunks.end();
}

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

		if (this->inBounds(neighborChunk)) {
			Chunk& chunk = this->getChunkAt(neighborChunk);
			chunk.dirty = true;
		}
	}
}

bool World::cameraIsSubmerged(const CameraController& cameraController) {
	tuple<int, int, int> chunkPos = cameraController.getChunkPos();
	if (!this->inBounds(chunkPos)) {
		return false;
	}

	Chunk& chunk = this->getChunkAt(chunkPos);

	int blockX = EUCMOD((int)cameraController.camera.position.x, CHUNK_SIZE);
	int blockY = EUCMOD((int)cameraController.camera.position.y, CHUNK_SIZE);
	int blockZ = EUCMOD((int)cameraController.camera.position.z, CHUNK_SIZE);

	Block block = chunk.getBlockAt(blockX, blockY, blockZ);

	return block.blockType == BlockType::WATER;
}

void World::sortChunks(const CameraController& cameraController) {
	// sorted order (farthest to closest) saved in World::chunkOrder
	std::sort(
		this->chunkOrder.begin(),
		this->chunkOrder.end(),
		[&cameraController, this](const tuple<int, int, int>& idx1, const tuple<int, int, int>& idx2){
			string key1 = TUP_TO_STR(idx1);
			string key2 = TUP_TO_STR(idx2);

			Chunk& chunk1 = this->chunks.at(key1);
			Chunk& chunk2 = this->chunks.at(key2);

			return chunk1.distanceFromCamera > chunk2.distanceFromCamera;
		}
	);
}

void World::cameraMoved(const CameraController& cameraController) {
	// TODO: only do in some cases?
	// TODO: faster way to do this?

	this->chunkOrder.clear();


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

				if (this->inBounds(idx)) {
					int diffX = x - cameraChunkX;
					int diffY = y - cameraChunkY;
					int diffZ = z - cameraChunkZ;

					float dist = sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);
					this->getChunkAt(idx).distanceFromCamera = dist;

					if (dist <= VIEW_DIST && !this->getChunkAt(idx).blank) {
						this->chunkOrder.push_back(idx);
					}
				}

			}
		}
	}

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

				if (this->inBounds(idx)) {
					Chunk& chunk = this->getChunkAt(idx);

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
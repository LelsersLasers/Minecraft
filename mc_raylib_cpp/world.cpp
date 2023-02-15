#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cmath>

// #include <iostream>

#include <vector>
#include <tuple>
#include <utility>
#include <optional>

#include <algorithm>
#include <functional>

#include "include/block.h"
#include "include/chunk.h"
#include "include/raycastRequest.h"
#include "include/consts.h"
#include "include/PerlinNoise.h"
#include "include/PerlinNoiseUtil.h"
#include "include/blockPlaceRequest.h"

#include "include/world.h"

using std::vector;
using std::tuple;
using std::pair;
using std::optional;
using std::reference_wrapper;


World::World() {
	this->chunks = unordered_map<string, Chunk>();

	this->nearbyChunks = vector<reference_wrapper<Chunk>>();
	this->chunksToRender = vector<reference_wrapper<Chunk>>();

	this->shouldSortChunksToRender = false;

	this->chunksToGenerate = vector<pair<tuple<int, int, int>, float>>();

	this->blockPlaceRequests = vector<BlockPlaceRequest>();

	this->compareChunksByDistance = [this](const reference_wrapper<Chunk>& chunkRef1, const reference_wrapper<Chunk>& chunkRef2) -> bool {
		Chunk& chunk1 = chunkRef1;
		Chunk& chunk2 = chunkRef2;

		return chunk1.distanceFromCamera > chunk2.distanceFromCamera;
	};
}

optional<reference_wrapper<Chunk>> World::getChunkAt(tuple<int, int, int> chunkPos) {
	// Does this copy??
	string key = TUP_TO_STR(chunkPos);
	auto iter = this->chunks.find(key);
	if (iter != this->chunks.end()) {
		Chunk& chunk = iter->second;
		return chunk;
	} else {
		return {};
	}
}
Block World::getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) {
	optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(chunkPos);
	if (possibleChunk.has_value()) {
		Chunk& chunk = possibleChunk.value();
		return chunk.getBlockAt(x, y, z);
	} else {
		// TEMP: should usally return BEDROCK_BLOCK
		// return BEDROCK_BLOCK;
		return AIR_BLOCK;
	}
}

void World::generateChunk(PerlinNoise& pn) {

	if (this->chunksToGenerate.size() == 0) {
		return;
	}

	pair<tuple<int, int, int>, float> chunkToGenerate = this->chunksToGenerate.back();
	this->chunksToGenerate.pop_back();

	tuple<int, int, int> chunkTup = chunkToGenerate.first;
	string key = TUP_TO_STR(chunkTup);

	Chunk chunk = Chunk(chunkTup);
	// chunk.generateBlocks(pn);
	this->createChunkData(pn, chunk);
	chunk.distanceFromCamera = chunkToGenerate.second;

	// TODO: copies chunk!!!!!!
	// this->chunks.insert(std::make_pair(key, chunk));
	pair<string, Chunk> pair = std::make_pair(key, chunk);
	this->chunks.insert(pair);

	for (size_t j = 0; j < 6; j++) {
		tuple<int, int, int> dirTuple = allDirTuples[j];

		tuple<int, int, int> neighborTup = std::make_tuple(
			std::get<0>(chunkTup) + std::get<0>(dirTuple),
			std::get<1>(chunkTup) + std::get<1>(dirTuple),
			std::get<2>(chunkTup) + std::get<2>(dirTuple)
		);

		optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(neighborTup);
		if (possibleChunk.has_value()) {
			Chunk& neighbor = possibleChunk.value();
			neighbor.dirty = true;
		}
	}


	// TODO: better way to do this?
	Chunk& chunkRef = this->chunks.at(key);

	chunkRef.generateModel(*this);
	this->nearbyChunks.push_back(chunkRef);
	if (!chunkRef.blank || !chunkRef.transparentBlank) {
		this->chunksToRender.push_back(chunkRef);	
		this->shouldSortChunksToRender = true;
	}
}


int World::getHeightAt(PerlinNoise& pn, int x, int y) { // static
	double height = PerlinNoise3DWithOctaves(
		pn,
		(double)x / PERLIN_NOISE_DIVISOR,
		(double)y / PERLIN_NOISE_DIVISOR,
		1.0,
		OCTAVES
	);
	int maxHeight = CHUNK_SIZE * WORLD_SIZE;
	int scaledHeight = (int)((double)maxHeight * height);
	return scaledHeight;
}
void World::createChunkData(PerlinNoise& pn, Chunk& chunk) {
	chunk.blocks.clear();

	int worldChunkX = std::get<0>(chunk.position) * CHUNK_SIZE;
	int worldChunkY = std::get<1>(chunk.position) * CHUNK_SIZE;
	int worldChunkZ = std::get<2>(chunk.position) * CHUNK_SIZE;

	int scaledHeights[CHUNK_SIZE][CHUNK_SIZE];
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			int worldY = worldChunkY + (int)y;

			int scaledHeight = World::getHeightAt(pn, worldX, worldY);
			scaledHeights[x][y] = scaledHeight;
		}
	}


	// Shaping -----------------------------------------------------------------------//
	/*
		Z == 0 -> Bedrock
		Z <= scaledHeight -> Stone
		Z > scaledHeight && Z <= WATER_LEVEL -> Water
		else -> Air
	*/
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			int worldY = worldChunkY + (int)y;

			int scaledHeight = scaledHeights[x][y];

			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				int worldZ = worldChunkZ + (int)z;

				if (worldZ == 0) {
					chunk.blocks.push_back(BEDROCK_BLOCK);
				} else if (worldZ <= scaledHeight) {
					chunk.blocks.push_back(STONE_BLOCK);
				} else if (worldZ <= WATER_LEVEL) {
					chunk.blocks.push_back(WATER_BLOCK);
				} else {
					chunk.blocks.push_back(AIR_BLOCK);
				}

			}
		}
	}
	//--------------------------------------------------------------------------------//

	// Top ---------------------------------------------------------------------------//
	/*
		Z == scaledHeight -> Grass
		Z == scaledHeight - RAND(3, 5) -> Dirt
		else -> unaffected
	*/
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			int worldY = worldChunkY + (int)y;

			int scaledHeight = scaledHeights[x][y];
			int chunkZHeight = scaledHeight - worldChunkZ;

			if (chunkZHeight >= 0 && chunkZHeight < CHUNK_SIZE) { // only run for chunk that contains the scaledHeight
				int dirtDepth = RAND(3, 5);
				int dirtStart = chunkZHeight - dirtDepth;

				chunk.setBlockAt(x, y, chunkZHeight, GRASS_BLOCK);

				for (int z = dirtStart; z < chunkZHeight; z++) {
					if (z >= 0 && z < CHUNK_SIZE) {
						chunk.setBlockAt(x, y, z, DIRT_BLOCK);
					} else {
						int chunkZDiff = z < 0 ? -1 : 1;
						tuple<int, int, int> neighborTup = std::make_tuple(
							std::get<0>(chunk.position),
							std::get<1>(chunk.position),
							std::get<2>(chunk.position) + chunkZDiff
						);

						BlockPlaceRequest request = BlockPlaceRequest {
							chunkPos: neighborTup,
							x: x,
							y: y,
							z: (size_t)EUCMOD_SIMPLE(z, CHUNK_SIZE),
							block: DIRT_BLOCK
						};

						this->blockPlaceRequests.push_back(request);
					}
				}

			}
		}
	}
	//--------------------------------------------------------------------------------//
	

	chunk.dirty = true;
}

void World::updateChunkModels() {
	for (size_t i = this->nearbyChunks.size(); i-- > 0; ) {
		Chunk& chunk = this->nearbyChunks[i];

		// TODO: better way to do this?
		for (size_t j = 0; j < this->blockPlaceRequests.size(); j++) {
			BlockPlaceRequest& request = this->blockPlaceRequests[j];
			if (request.chunkPos == chunk.position) {
				chunk.setBlockAt(request.x, request.y, request.z, request.block);
				this->blockPlaceRequests.erase(this->blockPlaceRequests.begin() + j);
			}
		}

		if (chunk.dirty) {
			chunk.generateModel(*this);

			// TODO: better way to do this?
			if (!chunk.blank || !chunk.transparentBlank) { // newly not blank

				// can't use binary search, because keysToRender is not sorted yet
				bool found = false;
				for (size_t j = 0; j < this->chunksToRender.size(); j++) {
					Chunk& chunkToRender = this->chunksToRender[j];
					if (chunkToRender.position == chunk.position) {
						found = true;
						break;
					}
				}
				if (!found) {
					this->chunksToRender.push_back(chunk);
					this->shouldSortChunksToRender = true;
				}
			} else { // newly blank
				// can't use binary search, because keysToRender is not sorted yet
				for (size_t j = 0; j < this->chunksToRender.size(); j++) {
					Chunk& chunkToRender = this->chunksToRender[j];
					if (chunkToRender.position == chunk.position) {
						this->chunksToRender.erase(this->chunksToRender.begin() + j);
						break;
					}
				}
			}

			// return; // only update one chunk per frame????
		}
	}
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

		optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(neighborChunk);
		if (possibleChunk.has_value()) {
			Chunk& chunk = possibleChunk.value();
			chunk.dirty = true;
		}
	}
}

bool World::cameraIsSubmerged(const CameraController& cameraController) {
	tuple<int, int, int> chunkPos = cameraController.getChunkPos();

	optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(chunkPos);
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

void World::sortKeysToRender() {
	// sorted: farthest to closest
	std::sort(
		this->chunksToRender.begin(),
		this->chunksToRender.end(),
		this->compareChunksByDistance
	);
	this->shouldSortChunksToRender = false;
}
void World::sortChunksToGenerate() {
	// not needed, just looks cool
	// sorted: farthest to closest
	std::sort(
		this->chunksToGenerate.begin(),
		this->chunksToGenerate.end(),
		[this](const pair<tuple<int, int, int>, float>& a, const pair<tuple<int, int, int>, float>& b){

			return a.second > b.second;
		}
	);
}

void World::cameraMoved(const CameraController& cameraController, PerlinNoise& pn) {
	// TODO: faster way to do this?

	this->nearbyChunks.clear();
	this->chunksToRender.clear();

	this->chunksToGenerate.clear();


	tuple<int, int, int> cameraChunk = cameraController.getChunkPos();
	int cameraChunkX = std::get<0>(cameraChunk);
	int cameraChunkY = std::get<1>(cameraChunk);
	int cameraChunkZ = std::get<2>(cameraChunk);

	int startX = MAX(cameraChunkX - VIEW_DIST, 0); // TEMP: delete MAX
	int startY = MAX(cameraChunkY - VIEW_DIST, 0); // TEMP: delete MAX
	int startZ = MAX(cameraChunkZ - VIEW_DIST, LOWEST_CHUNK_Z);

	int endX = cameraChunkX + VIEW_DIST;
	int endY = cameraChunkY + VIEW_DIST;
	int endZ = cameraChunkZ + VIEW_DIST;

	for (int x = startX; x <= endX; x++) {
		for (int y = startY; y <= endY; y++) {
			for (int z = startZ; z <= endZ; z++) {

				tuple<int, int, int> idx = std::make_tuple(x, y, z);

				int diffX = x - cameraChunkX;
				int diffY = y - cameraChunkY;
				int diffZ = z - cameraChunkZ;

				float dist = sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);

				if (dist <= VIEW_DIST) {
					optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(idx);
					if (possibleChunk.has_value()) {
						Chunk& chunk = possibleChunk.value();
						chunk.distanceFromCamera = dist;

						this->nearbyChunks.push_back(chunk);
						if (!chunk.blank || !chunk.transparentBlank) {
							this->chunksToRender.push_back(chunk);	
						}
					} else {
						this->chunksToGenerate.push_back(std::make_pair(idx, dist));
					}
				}

			}
		}
	}

	this->sortChunksToGenerate();
	this->shouldSortChunksToRender = true;
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
	// TODO: use chunksToRender and break when distance is out of neighboring chunks
	for (int x = cameraChunkX - 1; x <= cameraChunkX + 1; x++) {
		for (int y = cameraChunkY - 1; y <= cameraChunkY + 1; y++) {
			for (int z = cameraChunkZ - 1; z <= cameraChunkZ + 1; z++) {

				tuple<int, int, int> idx = std::make_tuple(x, y, z);

				optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(idx);
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
			Vector3 blockCenter = (Vector3){
				(float)std::get<0>(bestBlockTuple) + 0.5f,
				(float)std::get<1>(bestBlockTuple) + 0.5f,
				(float)std::get<2>(bestBlockTuple) + 0.5f
			};
			Vector3 bestBlockOutlinePos = Vector3Add(blockCenter, closestChunkCollision->getWorldPos());
			return bestBlockOutlinePos;
		}
	}

	return {};
}
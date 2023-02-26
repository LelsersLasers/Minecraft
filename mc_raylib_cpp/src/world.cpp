#include "include/blockType.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cmath>

#include <iostream>

#include <vector>
#include <tuple>
#include <utility>
#include <optional>

#include <algorithm>
#include <functional>

#include "include/block.h"
#include "include/atlas.h"
#include "include/chunk.h"
#include "include/raycastRequest.h"
#include "include/consts.h"
#include "include/PerlinNoise.h"
#include "include/PerlinNoiseUtil.h"
#include "include/blockPlaceRequest.h"
#include "include/ChunkModelInfo.h"
#include "include/biome.h"
#include "include/biomeType.h"

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
		return BEDROCK_BLOCK;
	}
}

void World::generateChunk(PerlinNoise& pn, Atlas& atlas) {

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

	// Copies chunk!!!!!!
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

	chunkRef.generateModel(*this, atlas);
	this->nearbyChunks.push_back(chunkRef);

	bool allBlank = chunkRef.allBlankModels();
	if (!allBlank) {
		this->chunksToRender.push_back(chunkRef);	
		this->shouldSortChunksToRender = true;
	}
}


int World::getHeightAt(PerlinNoise& pn, vector<pair<Vector2, float>>& lowerResolutions, int x, int y) { // static

	float lowerResolutionX = (float)x / NOISE_RESOLUTION;
	float lowerResolutionY = (float)y / NOISE_RESOLUTION;

	Vector2 blockLowerResolutions[4] = {
		(Vector2){ floorf(lowerResolutionX), floorf(lowerResolutionY) }, // (0,0)
		(Vector2){ ceilf (lowerResolutionX), floorf(lowerResolutionY) }, // (1,0)
		(Vector2){ floorf(lowerResolutionX), ceilf (lowerResolutionY) }, // (0,1)
		(Vector2){ ceilf (lowerResolutionX), ceilf (lowerResolutionY) }, // (1,1)
	};

	Vector2 upscaleds[4];
	float heights[4];

	size_t indexes[4]; // indexes to lowerResolutions

	for (size_t i = 0; i < 4; i++) {

		bool found = false;
		
		for (size_t j = 0; j < lowerResolutions.size(); j++) {
			pair<Vector2, float> lowerResolution = lowerResolutions[j];
			Vector2 lowerResolutionPos = lowerResolution.first;

			if (Vector2Equals(lowerResolutionPos, blockLowerResolutions[i])) {
				indexes[i] = j;
				found = true;
				break;
			}
		}

		if (!found) {
			lowerResolutions.push_back(std::make_pair(blockLowerResolutions[i], -1));
			indexes[i] = lowerResolutions.size() - 1;
		}
	}

	for (size_t i = 0; i < 4; i++) {

		size_t index = indexes[i];
		pair<Vector2, float>& lowerResolution = lowerResolutions[index];
		Vector2 lowerResolutionPos = lowerResolution.first;

		Vector2 upscaled = Vector2Scale(lowerResolutionPos, NOISE_RESOLUTION);
		upscaleds[i] = upscaled;


		if (lowerResolution.second < 0) {

			int height = LOWEST_SURFACE_Z;
			int maxHeight = CHUNK_SIZE * WORLD_SIZE;
			
			for (int z = maxHeight; z >= LOWEST_SURFACE_Z; z--) {

				double noise = PerlinNoise3DWithOctaves(
					pn,
					(double)lowerResolutionPos.x / NOISE_DIVISOR,
					(double)lowerResolutionPos.y / NOISE_DIVISOR,
					(double)z / (NOISE_DIVISOR * NOISE_RESOLUTION),
					// lowerResolution / NOISE_DIVISOR,
					OCTAVES
				);

				int distToBottom = maxHeight - z; // lower Z -> higher value
				double distToBottomScale = (double)distToBottom / (double)maxHeight;
				
				double modifiedNoise = (noise + distToBottomScale);

				if (modifiedNoise >= 1.0) {
					height = z;
					break;
				}
			}
			lowerResolution.second = (float)height; // modifies the pair in the vector
		}
		heights[i] = (float)lowerResolution.second;

	}

	auto linearInterpolation = [](float x1, float y1, float x2, float y2, float x3) -> float {
		float rise = y2 - y1;
		float run  = x2 - x1;
		if (run == 0) {
			return y1;
		}
		float slope = rise / run;
		float y3 = slope * (x3 - x1) + y1;
		return y3;
	};

	// bilinear interpolation
	float height1 = linearInterpolation(upscaleds[0].x, heights[0], upscaleds[1].x, heights[1], (float)x); // (0,0) -> (1,0)
	float height2 = linearInterpolation(upscaleds[2].x, heights[2], upscaleds[3].x, heights[3], (float)x); // (0,1) -> (1,1) 
	float height3 = linearInterpolation(upscaleds[0].y, height1, upscaleds[2].y, height2, (float)y); // (0-y, height1) -> (1-y, height2)

	int scaledHeight = (int)roundf(height3);
	return scaledHeight;
}
void World::createBlockPlaceRequestAt(Chunk& chunk, int x, int y, int z, Block block, vector<BlockType> canOverwrite) {
	int chunkXDiff = (x < 0) * -1 + (x >= CHUNK_SIZE) * 1;
	int chunkYDiff = (y < 0) * -1 + (y >= CHUNK_SIZE) * 1;
	int chunkZDiff = (z < 0) * -1 + (z >= CHUNK_SIZE) * 1;

	tuple<int, int, int> neighborTup = std::make_tuple(
		std::get<0>(chunk.position) + chunkXDiff,
		std::get<1>(chunk.position) + chunkYDiff,
		std::get<2>(chunk.position) + chunkZDiff
	);

	BlockPlaceRequest request = BlockPlaceRequest {
		chunkPos: neighborTup,
		x: (size_t)EUCMOD_SIMPLE(x, CHUNK_SIZE),
		y: (size_t)EUCMOD_SIMPLE(y, CHUNK_SIZE),
		z: (size_t)EUCMOD_SIMPLE(z, CHUNK_SIZE),
		block: block,
		canOverwrite: canOverwrite
	};

	this->blockPlaceRequests.push_back(request);
}
void World::createChunkData(PerlinNoise& pn, Chunk& chunk) {
	chunk.blocks.clear();

	int worldChunkX = std::get<0>(chunk.position) * CHUNK_SIZE;
	int worldChunkY = std::get<1>(chunk.position) * CHUNK_SIZE;
	int worldChunkZ = std::get<2>(chunk.position) * CHUNK_SIZE;

	vector<pair<Vector2, float>> lowerResolutions = vector<pair<Vector2, float>>();

	int scaledHeights[CHUNK_SIZE][CHUNK_SIZE];
	vector<Biome> biomeFlatMap = vector<Biome>();
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			int worldY = worldChunkY + (int)y;

			int scaledHeight = World::getHeightAt(pn, lowerResolutions, worldX, worldY);
			scaledHeights[x][y] = scaledHeight;

			biomeFlatMap.push_back(Biome::biomeFromType(getBiomeFromHeight(scaledHeight)));
		}
	}

	// TODO: condension logic? lots of duplicated code (for loops, etc)


	// Shaping -----------------------------------------------------------------------//
	/*
		Z == 0 -> Bedrock
		Z <= scaledHeight -> Stone
		Z > scaledHeight && Z <= WATER_LEVEL -> Water
		else -> Air
	*/
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		// int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			// int worldY = worldChunkY + (int)y;

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
		// int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			// int worldY = worldChunkY + (int)y;

			int scaledHeight = scaledHeights[x][y];
			int chunkZHeight = scaledHeight - worldChunkZ;
			Biome& biome = biomeFlatMap[x * CHUNK_SIZE + y];

			if (chunkZHeight >= 0 && chunkZHeight < CHUNK_SIZE) { // only run for chunk that contains the scaledHeight

				int secondBlockDepth = RAND(biome.secondBlockLevelMin, biome.secondBlockLevelMax);
				int secondBlockStart = chunkZHeight - secondBlockDepth;

				chunk.setBlockAt(x, y, chunkZHeight, biome.topBlock);

				for (int z = secondBlockStart; z < chunkZHeight; z++) {
					this->createBlockPlaceRequestAt(chunk, x, y, z, biome.secondBlock, vector<BlockType>({ BlockType::STONE }));
				}

			}
		}
	}
	//--------------------------------------------------------------------------------//

	// Trees -------------------------------------------------------------------------//
	/*
		TREE_CHANCE chance to place a tree at every scaledHeight above WATER_LEVEL
		Trunk height: [4, 5]
		Leaves radius: 5w, 4h block around top of tree
	*/
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		// int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			// int worldY = worldChunkY + (int)y;

			int scaledHeight = scaledHeights[x][y];
			Biome& biome = biomeFlatMap[x * CHUNK_SIZE + y];
			if (biome.biomeType == BiomeType::NEAR_WATER) { // don't place trees on water or on water's edge
				continue;
			}

			int chunkZHeight = scaledHeight - worldChunkZ;

			if (chunkZHeight >= 0 && chunkZHeight < CHUNK_SIZE) { // only run for chunk that contains the scaledHeight
				bool tree = RAND_CHANCE(TREE_CHANCE);
				if (tree) {

					// tree trunk
					int treeHeight = RAND(4, 6);
					int treeStart = chunkZHeight + 1;
					int treeEnd = treeStart + treeHeight;

					for (int z = treeStart; z < treeEnd; z++) {
						this->createBlockPlaceRequestAt(chunk, x, y, z, LOG_BLOCK, vector<BlockType>({ BlockType::AIR, BlockType::LEAVES }));	
					}

					// tree leaves
					int leavesStartX = x - 2;
					int leavesStartY = y - 2;
					int leavesStartZ = treeEnd - 3;

					int leavesEndX = x + 2;
					int leavesEndY = y + 2;
					int leavesEndZ = treeEnd;

					for (int leavesX = leavesStartX; leavesX <= leavesEndX; leavesX++) {
						for (int leavesY = leavesStartY; leavesY <= leavesEndY; leavesY++) {

							bool leavesXOnEdge = leavesX == leavesStartX || leavesX == leavesEndX;
							bool leavesYOnEdge = leavesY == leavesStartY || leavesY == leavesEndY;

							bool leavesXOnInsideOfEdge = leavesX == leavesStartX + 1 || leavesX == leavesEndX - 1;
							bool leavesYOnInsideOfEdge = leavesY == leavesStartY + 1 || leavesY == leavesEndY - 1;

							if (leavesXOnEdge && leavesYOnEdge) {
								continue;
							}

							for (int leavesZ = leavesStartZ; leavesZ <= leavesEndZ; leavesZ++) {
								if (leavesX == (int)x && leavesY == (int)y && leavesZ < leavesEndZ) {
									continue;
								} else if ((leavesXOnEdge || leavesYOnEdge) && leavesZ >= leavesEndZ - 1) {
									continue;
								} else if ((leavesXOnInsideOfEdge && leavesYOnInsideOfEdge) && leavesZ == leavesEndZ) {
									continue;
								}

								this->createBlockPlaceRequestAt(chunk, leavesX, leavesY, leavesZ, LEAVES_BLOCK, vector<BlockType>({ BlockType::AIR }));
							}
						}
					}

				}
			}

		}
	}
	//--------------------------------------------------------------------------------//
	

	chunk.dirty = true;
}

void World::updateChunkModels(Atlas& atlas) {
	for (size_t i = this->nearbyChunks.size(); i-- > 0; ) {
		Chunk& chunk = this->nearbyChunks[i];

		// TODO: better way to do this?
		for (size_t j = 0; j < this->blockPlaceRequests.size(); j++) {
			BlockPlaceRequest& request = this->blockPlaceRequests[j];
			if (request.chunkPos == chunk.position) {
				Block blockToBeReplaced = chunk.getBlockAt(request.x, request.y, request.z);

				bool found = false;
				for (size_t k = 0; k < request.canOverwrite.size(); k++) {
					if (request.canOverwrite[k] == blockToBeReplaced.blockType) {
						found = true;
						break;
					}
				}
				if (found) {
					chunk.setBlockAt(request.x, request.y, request.z, request.block);
				}

				this->blockPlaceRequests.erase(this->blockPlaceRequests.begin() + j);
			}
		}

		if (chunk.dirty) {
			chunk.generateModel(*this, atlas);

			// TODO: better way to do this?

			bool allBlank = chunk.allBlankModels();
			if (!allBlank) { // newly not blank

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

	int startX = cameraChunkX - VIEW_DIST;
	int startY = cameraChunkY - VIEW_DIST;
	int startZ = MAX(cameraChunkZ - VIEW_DIST, LOWEST_CHUNK_Z);

	int endX = cameraChunkX + VIEW_DIST;
	int endY = cameraChunkY + VIEW_DIST;
	int endZ = cameraChunkZ + VIEW_DIST;

	for (int x = startX; x <= endX; x++) {
		for (int y = startY; y <= endY; y++) {
			for (int z = startZ; z <= endZ; z++) {

				tuple<int, int, int> idx = std::make_tuple(x, y, z);

				float diffX = (float)x - (float)cameraChunkX;
				float diffY = (float)y - (float)cameraChunkY;
				float diffZ = (float)z - (float)cameraChunkZ;

				float dist = sqrtf(diffX * diffX + diffY * diffY + diffZ * diffZ);

				if (dist <= VIEW_DIST) {
					optional<reference_wrapper<Chunk>> possibleChunk = this->getChunkAt(idx);
					if (possibleChunk.has_value()) {
						Chunk& chunk = possibleChunk.value();
						chunk.distanceFromCamera = dist;

						this->nearbyChunks.push_back(chunk);

						bool allBlank = chunk.allBlankModels();
						if (!allBlank) {
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

					for (size_t i = 0; i < RAYCAST_CHUNK_MODELS_COUNT; i++) {
						ChunkModelInfo cmi = RAYCAST_CHUNK_MODELS[i];

						if (chunk.isBlankModels[cmi]) {
							continue;
						}

						Vector3 pos = chunk.getWorldPos();
						Matrix qTransform = MatrixTranslate(pos.x, pos.y, pos.z);
						RayCollision qRayCollision = GetRayCollisionMesh(ray, chunk.models[cmi].meshes[0], qTransform);
						if (qRayCollision.hit && qRayCollision.distance < REACH) {
							rayCollisions.push_back(qRayCollision);
							chunkCollisions.push_back(&chunk);
						}
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

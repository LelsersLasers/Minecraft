#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "include/block.h"
#include "include/chunk.h"
#include "include/consts.h"

#include "include/world.h"

using std::vector;
using std::tuple;


World::World() {
	this->chunks = vector<Chunk>();
	this->chunks.reserve(WORLD_SIZE * WORLD_SIZE * WORLD_SIZE);
}

Chunk& World::getChunkAt(tuple<int, int, int> chunkPos) {
	size_t x = std::get<0>(chunkPos);
	size_t y = std::get<1>(chunkPos);
	size_t z = std::get<2>(chunkPos);
	
	return this->chunks[x * WORLD_SIZE * WORLD_SIZE + y + z * WORLD_SIZE];
}
Block World::getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) {
	if (World::inBounds(chunkPos)) {
		Chunk& chunk = this->getChunkAt(chunkPos);
		return chunk.getBlockAt(x, y, z);
	}
	else {
		return Block(BlockType::AIR);
	}
}

void World::generateChunks() {
	this->chunks.clear();

	for (size_t x = 0; x < WORLD_SIZE; x++) {
		for (size_t y = 0; y < WORLD_SIZE; y++) {
			for (size_t z = 0; z < WORLD_SIZE; z++) {

				Chunk chunk = Chunk({ x, y, z });
				chunk.generateBlocks();
				this->chunks.push_back(chunk);

			}
		}
	}
}
void World::updateChunkModels() {
	for (size_t i = 0; i < this->chunks.size(); i++) {
		if (this->chunks[i].dirty) {
			this->chunks[i].generateModel(*this);
		}
	}
}



bool World::inBounds(tuple<int, int, int> chunkPos) {
	int x = std::get<0>(chunkPos);
	int y = std::get<1>(chunkPos);
	int z = std::get<2>(chunkPos);
	return x >= 0 && x < WORLD_SIZE && y >= 0 && y < WORLD_SIZE && z >= 0 && z < WORLD_SIZE;
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
		if (World::inBounds(neighborChunk)) {
			Chunk& chunk = this->getChunkAt(neighborChunk);
			chunk.dirty = true;
		}
	}
}
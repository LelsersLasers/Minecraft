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

Chunk World::getChunkAt(tuple<int, int, int> chunkPos) const {
	size_t x = std::get<0>(chunkPos);
	size_t y = std::get<1>(chunkPos);
	size_t z = std::get<2>(chunkPos);
	return this->chunks[x * WORLD_SIZE * WORLD_SIZE + y + z * WORLD_SIZE];
}
Block World::getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) const {
	if (World::inBounds(chunkPos)) {
		Chunk chunk = this->getChunkAt(chunkPos);
		return chunk.getBlockAt(x, y, z);
	}
	else {
		return Block(AIR);
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
#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "include/block.h"
#include "include/blockType.h"
#include "include/consts.h"

using std::vector;
using std::tuple;

#include "include/chunk.h"

Chunk::Chunk(const tuple<int, int, int>& position) {
	this->position = position;

	this->blocks = vector<Block>();
	this->blocks.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
}

Block Chunk::getBlockAt(size_t x, size_t y, size_t z) const {
	return this->blocks[x * CHUNK_SIZE * CHUNK_SIZE + y + z * CHUNK_SIZE];
}
Vector3 Chunk::getWorldPos() const {
	return (Vector3) {
		(float)std::get<0>(this->position) * CHUNK_SIZE,
		(float)std::get<1>(this->position) * CHUNK_SIZE,
		(float)std::get<2>(this->position) * CHUNK_SIZE,
	};
}

void Chunk::generateBlocks() {
	this->blocks.clear();

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				
				BlockType blockType = getRandomBlockType();
				Block block(blockType);
				this->blocks.push_back(block);

			}
		}
	}

}

void Chunk::generateTriangles() {
	// TODO:
}
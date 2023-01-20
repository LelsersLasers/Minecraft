#ifndef CHUNK_GUARD
#define CHUNK_GUARD

#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "block.h"

using std::vector;
using std::tuple;


class Chunk {
	public:
		vector<Block> blocks;
		tuple<int, int, int> position;
	
		Chunk(const tuple<int, int, int>& position);

		Block getBlockAt(size_t x, size_t y, size_t z) const;
		Vector3 getWorldPos() const;

		void generateBlocks();
		void generateTriangles();


};






#endif
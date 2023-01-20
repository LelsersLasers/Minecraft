#ifndef CHUNK_GUARD
#define CHUNK_GUARD

#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "block.h"
#include "triangle.h"

using std::vector;
using std::tuple;


class Chunk {
	public:
		vector<Block> blocks;
		tuple<int, int, int> position;

		vector<Triangle> triangles;
		Model model;
		bool dirty;
	
		Chunk(const tuple<int, int, int>& position);

		Block getBlockAt(size_t x, size_t y, size_t z) const;
		Vector3 getWorldPos() const;

		void generateBlocks();
		void generateTriangles();

		void generateModel();

		static bool inBounds(int x, int y, int z);


};






#endif
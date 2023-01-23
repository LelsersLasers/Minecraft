#ifndef CHUNK_GUARD
#define CHUNK_GUARD

#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "block.h"
#include "world.h"

using std::vector;
using std::tuple;


class Chunk {
	public:
		vector<Block> blocks;
		tuple<int, int, int> position;

		Model model;
		bool dirty;

		Mesh oldMesh;
	
		Chunk(const tuple<int, int, int>& position);
		~Chunk();

		Block getBlockAt(size_t x, size_t y, size_t z) const;
		Vector3 getWorldPos() const;

		void generateBlocks();
		void generateModel(const World& world);

		static bool inBounds(int x, int y, int z);

		tuple<size_t, size_t, size_t> handleRayCollision(RayCollision rayCollision) const;


};






#endif
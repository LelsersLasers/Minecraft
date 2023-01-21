#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <vector>
#include <tuple>

#include "block.h"
#include "chunk.h"

using std::vector;
using std::tuple;


class World {
	public:
		vector<Chunk> chunks;

		World();
		// ~World();

		Chunk getChunkAt(tuple<int, int, int> chunkPos) const;
		Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z) const;

		void generateChunks();
		void updateChunkModels();

		static bool inBounds(tuple<int, int, int> chunkPos);
};












#endif
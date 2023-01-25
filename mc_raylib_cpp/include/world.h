#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <vector>
#include <tuple>

#include "block.h"
#include "cameraController.h"
class Chunk; // forward declaration to avoid circular dependency (effectively: #include "chunk.h")

using std::vector;
using std::tuple;


class World {
	public:
		vector<Chunk> chunks;

		World();
		// ~World();

		Chunk& getChunkAt(tuple<int, int, int> chunkPos);
		Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z);

		void generateChunks();
		void updateChunkModels();

		static bool inBounds(tuple<int, int, int> chunkPos);

		void dirtyNeighbors(tuple<int, int, int> srcChunk, tuple<int, int, int> srcBlock);

		bool cameraIsSubmerged(const CameraController& cameraController);
};












#endif
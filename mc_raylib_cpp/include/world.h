#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>

#include "block.h"
#include "cameraController.h"

#include "PerlinNoise.h"

class Chunk; // forward declaration to avoid circular dependency (effectively: #include "chunk.h")

using std::vector;
using std::tuple;
using std::unordered_map;
using std::string;


class World {
	public:
		unordered_map<string, Chunk> chunks;

		vector<tuple<int, int, int>> chunkOrder;

		World();
		// ~World();

		Chunk& getChunkAt(tuple<int, int, int> chunkPos);
		Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z);

		void generateChunks(PerlinNoise& pn);
		void updateChunkModels();

		static bool inBounds(tuple<int, int, int> chunkPos);

		void dirtyNeighbors(tuple<int, int, int> srcChunk, tuple<int, int, int> srcBlock);

		bool cameraIsSubmerged(const CameraController& cameraController);

		void sortChunks(const CameraController& cameraController);
};












#endif
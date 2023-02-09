#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <vector>
#include <tuple>
#include <utility>
#include <unordered_map>
#include <string>
#include <optional>

#include "block.h"
#include "cameraController.h"
#include "raycastRequest.h"

#include "PerlinNoise.h"
#include "noCopy.h"

class Chunk; // forward declaration to avoid circular dependency (effectively: #include "chunk.h")

using std::vector;
using std::tuple;
using std::pair;
using std::unordered_map;
using std::string;
using std::optional;


class World : public NoCopy {
	public:
		unordered_map<string, Chunk> chunks;
		vector<string> chunkOrder;
		vector<pair<tuple<int, int, int>, float>> chunksToGenerate;

		World();
		// ~World();

		Chunk& getChunkAt(tuple<int, int, int> chunkPos);
		Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z);

		void generateChunks(PerlinNoise& pn);
		void updateChunkModels();

		bool inBounds(tuple<int, int, int> chunkPos);

		void dirtyNeighbors(tuple<int, int, int> srcChunk, tuple<int, int, int> srcBlock);

		bool cameraIsSubmerged(const CameraController& cameraController);

		void sortChunks(const CameraController& cameraController);

		void cameraMoved(const CameraController& cameraController, PerlinNoise& pn);

		optional<Vector3> handleRaycastRequest(const CameraController& cameraController, RaycastRequest& raycastRequest, Block selectedBlock);
};












#endif
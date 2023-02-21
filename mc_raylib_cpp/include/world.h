#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <vector>
#include <tuple>
#include <utility>
#include <unordered_map>
#include <string>
#include <optional>

#include <functional>

#include "block.h"
#include "blockType.h"
#include "cameraController.h"
#include "raycastRequest.h"
#include "blockPlaceRequest.h"

#include "PerlinNoise.h"
#include "noCopy.h"

class Chunk; // forward declaration to avoid circular dependency (effectively: #include "chunk.h")

using std::vector;
using std::tuple;
using std::pair;
using std::unordered_map;
using std::string;
using std::optional;
using std::reference_wrapper;


class World : public NoCopy {
	public:
		unordered_map<string, Chunk> chunks;

		vector<reference_wrapper<Chunk>> nearbyChunks;
		vector<reference_wrapper<Chunk>> chunksToRender;
		
		vector<pair<tuple<int, int, int>, float>> chunksToGenerate;

		bool shouldSortChunksToRender;

		vector<BlockPlaceRequest> blockPlaceRequests;


		std::function<bool(const reference_wrapper<Chunk>&, const reference_wrapper<Chunk>&)> compareChunksByDistance;

		World();
		// ~World();

		optional<reference_wrapper<Chunk>> getChunkAt(tuple<int, int, int> chunkPos);
		Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z);

		void generateChunk(PerlinNoise& pn, Atlas& atlas);

		int getHeightAt(PerlinNoise& pn, int x, int z);
		void createBlockPlaceRequestAt(Chunk& chunk, int x, int y, int z, Block block, vector<BlockType> canOverwrite);
		void createChunkData(PerlinNoise& pn, Chunk& chunk);

		void updateChunkModels(Atlas& atlas);

		void dirtyNeighbors(tuple<int, int, int> srcChunk, tuple<int, int, int> srcBlock);

		bool cameraIsSubmerged(const CameraController& cameraController);

		void sortKeysToRender();
		void sortChunksToGenerate();

		void cameraMoved(const CameraController& cameraController, PerlinNoise& pn);

		optional<Vector3> handleRaycastRequest(const CameraController& cameraController, RaycastRequest& raycastRequest, Block selectedBlock);
};












#endif

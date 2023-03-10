#ifndef CHUNK_GUARD
#define CHUNK_GUARD

#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "block.h"
#include "world.h"
#include "PerlinNoise.h"
#include "ChunkModelInfo.h"
#include "atlas.h"

#include "noCopy.h"

using std::vector;
using std::tuple;


class Chunk { // : public NoCopy { TODO:
	public:
		vector<Block> blocks;
		tuple<int, int, int> position;

		bool dirty;

		bool isBlankModels[TOTAL_CHUNK_MESHES];
		Model models[TOTAL_CHUNK_MESHES];
		Mesh oldMeshes[TOTAL_CHUNK_MESHES];

		float distanceFromCamera;
	
		Chunk(const tuple<int, int, int>& position);
		~Chunk();

		Block getBlockAt(size_t x, size_t y, size_t z) const;
		void setBlockAt(size_t x, size_t y, size_t z, Block block);

		Vector3 getWorldPos() const;

		Block getBlockInDirection(size_t x, size_t y, size_t z, tuple<int, int, int> dirTuple, World& world);
		void generateModel(World& world, Atlas& atlas);

		static bool inBounds(int x, int y, int z);
		bool allBlankModels() const;
		// static bool onEdge(int x, int y, int z);

		tuple<size_t, size_t, size_t> handleRayCollision(RayCollision rayCollision) const;

		void destroyBlockAt(tuple<size_t, size_t, size_t> blockIdx, World& world);
		void placeBlockAt(tuple<size_t, size_t, size_t> blockIdx, Vector3 rayNormal, Block block, World& world);
};






#endif
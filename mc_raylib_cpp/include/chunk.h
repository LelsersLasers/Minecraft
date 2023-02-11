#ifndef CHUNK_GUARD
#define CHUNK_GUARD

#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "block.h"
#include "world.h"

#include "PerlinNoise.h"
#include "noCopy.h"

using std::vector;
using std::tuple;


class Chunk { // : public NoCopy {
	public:
		vector<Block> blocks;
		tuple<int, int, int> position;

		bool dirty;

		bool blank;
		bool transparentBlank;
	
		Model model;
		Mesh oldMesh;

		Model transparentModel;
		Mesh transparentOldMesh;

		float distanceFromCamera;
	
		Chunk(const tuple<int, int, int>& position);
		~Chunk();

		Block getBlockAt(size_t x, size_t y, size_t z) const;
		void setBlockAt(size_t x, size_t y, size_t z, Block block);

		Vector3 getWorldPos() const;

		void generateBlocks(PerlinNoise& perlinNoise);

		Block getBlockInDirection(size_t x, size_t y, size_t z, tuple<int, int, int> dirTuple, World& world);
		void generateModel(World& world);

		static bool inBounds(int x, int y, int z);
		// static bool onEdge(int x, int y, int z);

		tuple<size_t, size_t, size_t> handleRayCollision(RayCollision rayCollision) const;

		void destroyBlockAt(tuple<size_t, size_t, size_t> blockIdx, World& world);
		void placeBlockAt(tuple<size_t, size_t, size_t> blockIdx, Vector3 rayNormal, Block block, World& world);
};






#endif
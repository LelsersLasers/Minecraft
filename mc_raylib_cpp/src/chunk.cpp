#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cstring>		// memcpy
#include <cmath>		// sqrtf

#include <vector>
#include <tuple>
#include <optional>

#include "include/block.h"
#include "include/atlas.h"
#include "include/world.h"
#include "include/blockType.h"
#include "include/dir.h"
#include "include/consts.h"
#include "include/PerlinNoiseUtil.h"
#include "include/ChunkModelInfo.h"

#include "include/PerlinNoise.h"

using std::vector;
using std::tuple;
using std::optional;

#include "include/chunk.h"

Chunk::Chunk(const tuple<int, int, int>& position) {
	this->position = position;

	this->blocks = vector<Block>();
	this->blocks.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
	
	this->dirty = true;

	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		this->isBlankModels[i] = true;
		this->models[i] = { 0 };
		this->oldMeshes[i] = { 0 };
	}

	this->distanceFromCamera = 0.0f;
}
Chunk::~Chunk() {
	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		UnloadModel(this->models[i]);
	}
}

Block Chunk::getBlockAt(size_t x, size_t y, size_t z) const {
	return this->blocks[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z];
}
void Chunk::setBlockAt(size_t x, size_t y, size_t z, Block block) {
	this->blocks[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z] = block;
	this->dirty = true;
}

Vector3 Chunk::getWorldPos() const {
	return (Vector3){
		(float)std::get<0>(this->position) * CHUNK_SIZE,
		(float)std::get<1>(this->position) * CHUNK_SIZE,
		(float)std::get<2>(this->position) * CHUNK_SIZE,
	};
}

Block Chunk::getBlockInDirection(size_t x, size_t y, size_t z, tuple<int, int, int> dirTuple, World& world) {
	int dirX = std::get<0>(dirTuple);
	int dirY = std::get<1>(dirTuple);
	int dirZ = std::get<2>(dirTuple);

	int nx = x + dirX;
	int ny = y + dirY;
	int nz = z + dirZ;

	Block neighbor = AIR_BLOCK;
	if (Chunk::inBounds(nx, ny, nz)) {
		neighbor = this->getBlockAt(nx, ny, nz);
	} else {
		int neighborChunkX = std::get<0>(this->position) + dirX;
		int neighborChunkY = std::get<1>(this->position) + dirY;
		int neighborChunkZ = std::get<2>(this->position) + dirZ;
		tuple<int, int, int> neighborChunkIdx = std::make_tuple(neighborChunkX, neighborChunkY, neighborChunkZ);

		neighbor = world.getBlockAt(
			neighborChunkIdx,
			EUCMOD_SIMPLE(nx, CHUNK_SIZE),
			EUCMOD_SIMPLE(ny, CHUNK_SIZE),
			EUCMOD_SIMPLE(nz, CHUNK_SIZE)
		);
	}
	
	return neighbor;
}

void Chunk::generateModel(World& world, Atlas& atlas) {

	Mesh newMeshes[TOTAL_CHUNK_MESHES] = { 0 };

	float* newVertices[TOTAL_CHUNK_MESHES];
	unsigned char* newColors[TOTAL_CHUNK_MESHES];
	float* newTexcoords[TOTAL_CHUNK_MESHES];

	int newVertexCounts[TOTAL_CHUNK_MESHES] = { 0 };

	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		// max vertices: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 3 floats per vertex
		newVertices[i] = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 3 * sizeof(float));

		// max colors: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 4 floats per vertex (rgba)
		newColors[i] = (unsigned char *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 4 * sizeof(unsigned char));
		
		// max texcoords: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 2 floats per vertex
		newTexcoords[i] = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 2 * sizeof(float));
	}

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {

				Block block = this->getBlockAt(x, y, z);
				if (block.blockType == BlockType::AIR) { // always will be blank
					continue;
				}

				Vector3 pos = {
					(float)x,
					(float)y,
					(float)z
				};

				bool shortenZ = false;
				if (block.blockType == BlockType::WATER) {
					Block upNeighbor = this->getBlockInDirection(x, y, z, allDirTuples[Dir::Top], world);
					if (upNeighbor.blockType != BlockType::WATER) {
						shortenZ = true;
					}
				}

				ChunkModelInfo cmi = block.chunkModelInfo;

				for (size_t i = 0; i < 6; i++) { // 6 faces per block
					Dir dir = allDirEnums[i];
					tuple<int, int, int> dirTuple = allDirTuples[i];

					Block neighbor = this->getBlockInDirection(x, y, z, dirTuple, world);
					if (
						(!neighbor.transparent // neight has non-see through faces
							|| (block.blockType == BlockType::WATER && neighbor.blockType == BlockType::WATER) // water on water
						) && !(block.blockType == BlockType::WATER && dir == Dir::Top && neighbor.blockType != BlockType::WATER) // avoid shortenZ weridness
					) {
						continue;
					}

					Color color = allDirColorModifiers[i];
					for (size_t j = 0; j < 2; j++) { // 2 triangles per face
						for (size_t k = 0; k < 3; k++) { // 3 vertices per triangle

							size_t triangleOffset = allTriangleOffsets[i][j][k];

							Vector3 cubeVertex = CUBE_VERTICES[triangleOffset];
							Vector3 vertex = Vector3Add(pos, cubeVertex);

							size_t texcoordTriangleOffset = allTexcoordsTriangleOffsets[i][j][k];
							Vector2 cubeTexcoord = CUBE_TEXCOORDS[texcoordTriangleOffset];
							Vector2 texcoordScaled = Vector2Scale(cubeTexcoord, 1.0f / TEXCOORDS_DIVISOR);
							Vector2 texcoordOffset = Vector2Scale(block.getTexcoords(dir), 1.0f / TEXCOORDS_DIVISOR);
							Vector2 texcoord = Vector2Add(texcoordScaled, texcoordOffset);


							newVertices[cmi][newVertexCounts[cmi] * 3 + 0] = vertex.x;
							newVertices[cmi][newVertexCounts[cmi] * 3 + 1] = vertex.y;
							newVertices[cmi][newVertexCounts[cmi] * 3 + 2] = pos.z + CUBE_VERTICES[allTriangleOffsets[i][j][k]].z * (shortenZ ? 0.8f : 1.0f);

							newColors[cmi][newVertexCounts[cmi] * 4 + 0] = color.r;
							newColors[cmi][newVertexCounts[cmi] * 4 + 1] = color.g;
							newColors[cmi][newVertexCounts[cmi] * 4 + 2] = color.b;
							newColors[cmi][newVertexCounts[cmi] * 4 + 3] = color.a;

							newTexcoords[cmi][newVertexCounts[cmi] * 2 + 0] = texcoord.x;
							newTexcoords[cmi][newVertexCounts[cmi] * 2 + 1] = texcoord.y;

							newVertexCounts[cmi]++;
						}
					}
				}
			}
		}
	}


	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		this->isBlankModels[i] = newVertexCounts[i] == 0;
	}

	bool allBlank = this->allBlankModels();
	if (!allBlank) {

		for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
			newMeshes[i].vertexCount = newVertexCounts[i];
			newMeshes[i].triangleCount = newVertexCounts[i] / 3;

			// 3 floats per vertex, 4 unsigned char per color, 2 floats per texcoord
			newMeshes[i].vertices = (float *)malloc(newMeshes[i].vertexCount * 3 * sizeof(float));
			newMeshes[i].colors = (unsigned char *)malloc(newMeshes[i].vertexCount * 4 * sizeof(unsigned char));
			newMeshes[i].texcoords = (float *)malloc(newMeshes[i].vertexCount * 2 * sizeof(float));

			std::memcpy(newMeshes[i].vertices,	newVertices[i],		newMeshes[i].vertexCount * 3 * sizeof(float));
			std::memcpy(newMeshes[i].colors,	newColors[i],		newMeshes[i].vertexCount * 4 * sizeof(unsigned char));
			std::memcpy(newMeshes[i].texcoords, newTexcoords[i],	newMeshes[i].vertexCount * 2 * sizeof(float));

			UnloadMesh(this->oldMeshes[i]);
			UploadMesh(&newMeshes[i], false);

			this->models[i] = LoadModelFromMesh(newMeshes[i]);
			this->models[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = atlas.texture; // way to do this for all models?

			this->oldMeshes[i] = newMeshes[i];
		}
	}

	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		free(newVertices[i]);
		free(newColors[i]);
		free(newTexcoords[i]);
	}


	this->dirty = false;
}

bool Chunk::inBounds(int x, int y, int z) { // static
	return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}
bool Chunk::allBlankModels() const {
	for (size_t i = 0; i < TOTAL_CHUNK_MESHES; i++) {
		if (!this->isBlankModels[i]) {
			return false;
		}
	}
	return true;
}

tuple<size_t, size_t, size_t> Chunk::handleRayCollision(RayCollision rayCollision) const {
	Vector3 point = Vector3Subtract(rayCollision.point, this->getWorldPos());

	float smallestDistance = INFINITY;

	size_t closestX = 0;
	size_t closestY = 0;
	size_t closestZ = 0;

	// effectively (int)floor( ) as point.? > 0
	int pointX = (int)point.x;
	int pointY = (int)point.y;
	int pointZ = (int)point.z;

	size_t startX = MAX(pointX - 1, 0);
	size_t startY = MAX(pointY - 1, 0);
	size_t startZ = MAX(pointZ - 1, 0);

	size_t endX = MIN(pointX + 2, CHUNK_SIZE);
	size_t endY = MIN(pointY + 2, CHUNK_SIZE);
	size_t endZ = MIN(pointZ + 2, CHUNK_SIZE);


	for (size_t x = startX; x < endX; x++) {
		for (size_t y = startY; y < endY; y++) {
			for (size_t z = startZ; z < endZ; z++) {
				
				Block block = this->getBlockAt(x, y, z);

				if (!block.solid) {
					continue;
				}

				Vector3 spot = {
					(float)x,
					(float)y,
					(float)z
				};

				Vector3 pos = Vector3Add(spot, { 0.5f, 0.5f, 0.5f });
				float distance = Vector3Length(Vector3Subtract(pos, point));
				if (distance < smallestDistance) {
					smallestDistance = distance;

					closestX = x;
					closestY = y;
					closestZ = z;
				}
			}
		}
	}

	return std::make_tuple(closestX, closestY, closestZ);
}

void Chunk::destroyBlockAt(tuple<size_t, size_t, size_t> blockIdx, World& world) {
	size_t x = std::get<0>(blockIdx);
	size_t y = std::get<1>(blockIdx);
	size_t z = std::get<2>(blockIdx);

	this->setBlockAt(x, y, z, AIR_BLOCK);
	
	world.dirtyNeighbors(this->position, blockIdx);
}
void Chunk::placeBlockAt(tuple<size_t, size_t, size_t> blockIdx, Vector3 rayNormal, Block block, World& world) {
	// rayNormal is V{ 0, 0, 0 } but with a single 1 or -1
	int newBlockX = (int)std::get<0>(blockIdx) + (int)rayNormal.x;
	int newBlockY = (int)std::get<1>(blockIdx) + (int)rayNormal.y;
	int newBlockZ = (int)std::get<2>(blockIdx) + (int)rayNormal.z;

	if (Chunk::inBounds(newBlockX, newBlockY, newBlockZ)) {
		this->setBlockAt(newBlockX, newBlockY, newBlockZ, block);
		world.dirtyNeighbors(this->position, std::make_tuple(newBlockX, newBlockY, newBlockZ));
	}
	else {
		int newChunkX = std::get<0>(this->position) + (int)rayNormal.x;
		int newChunkY = std::get<1>(this->position) + (int)rayNormal.y;
		int newChunkZ = std::get<2>(this->position) + (int)rayNormal.z;

		tuple<int, int, int> newChunkPos = std::make_tuple(newChunkX, newChunkY, newChunkZ);

		optional<reference_wrapper<Chunk>> possibleChunk = world.getChunkAt(newChunkPos);
		if (possibleChunk.has_value()) {
			Chunk& newChunk = possibleChunk.value();

			size_t chunkBlockX = (size_t)EUCMOD_SIMPLE(newBlockX, CHUNK_SIZE);
			size_t chunkBlockY = (size_t)EUCMOD_SIMPLE(newBlockY, CHUNK_SIZE);
			size_t chunkBlockZ = (size_t)EUCMOD_SIMPLE(newBlockZ, CHUNK_SIZE);

			newChunk.setBlockAt(chunkBlockX, chunkBlockY, chunkBlockZ, block);
			world.dirtyNeighbors(newChunk.position, std::make_tuple(chunkBlockX, chunkBlockY, chunkBlockZ));
		}
	}
}
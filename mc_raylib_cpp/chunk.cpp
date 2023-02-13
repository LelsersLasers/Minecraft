#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <cstring>		// memcpy
#include <cmath>		// sqrtf

#include <vector>
#include <tuple>
#include <optional>

#include "include/block.h"
#include "include/world.h"
#include "include/blockType.h"
#include "include/dir.h"
#include "include/consts.h"
#include "include/PerlinNoiseUtil.h"

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

	this->blank = true;
	this->transparentBlank = true;

	this->model = { 0 };
	this->oldMesh = { 0 };

	this->transparentModel = { 0 };
	this->transparentOldMesh = { 0 };

	this->distanceFromCamera = 0.0f;
}
Chunk::~Chunk() {
	UnloadModel(this->model);
	UnloadModel(this->transparentModel);
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

void Chunk::generateBlocks(PerlinNoise& pn) {
	this->blocks.clear();

	int worldChunkX = std::get<0>(this->position) * CHUNK_SIZE;
	int worldChunkY = std::get<1>(this->position) * CHUNK_SIZE;
	int worldChunkZ = std::get<2>(this->position) * CHUNK_SIZE;

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		int worldX = worldChunkX + (int)x;
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			int worldY = worldChunkY + (int)y;

			double height = PerlinNoise3DWithOctaves(
				pn,
				(double)worldX / PERLIN_NOISE_DIVISOR,
				(double)worldY / PERLIN_NOISE_DIVISOR,
				1.0,
				OCTAVES
			);
			int maxHeight = CHUNK_SIZE * WORLD_SIZE - 5;
			int scaledHeight = (int)((double)maxHeight * height) + 5;

			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				int worldZ = worldChunkZ + (int)z;


				if (worldZ == 0) {
					this->blocks.push_back(BEDROCK_BLOCK);
				} else if (worldZ <= scaledHeight - RAND(3, 6)) {
					this->blocks.push_back(STONE_BLOCK);
				} else if (worldZ <= scaledHeight - 1) {
					this->blocks.push_back(DIRT_BLOCK);
				} else if (worldZ == scaledHeight) {
					this->blocks.push_back(GRASS_BLOCK);
				} else if (worldZ <= WATER_LEVEL) {
					this->blocks.push_back(WATER_BLOCK);
				} else {
					this->blocks.push_back(AIR_BLOCK);
				}

				// if (worldZ == 0) {
				// 	this->blocks.push_back(BEDROCK_BLOCK);
				// } else if (worldZ <= CHUNK_SIZE + 2) {
				// 	this->blocks.push_back(STONE_BLOCK);
				// } else if (worldZ <= CHUNK_SIZE + 5) {
				// 	this->blocks.push_back(DIRT_BLOCK);
				// } else if (worldZ == CHUNK_SIZE + 6) {
				// 	this->blocks.push_back(GRASS_BLOCK);
				// } else if (worldX == 0 || worldY == CHUNK_SIZE - 1) {
				// 	this->blocks.push_back(DIRT_BLOCK);
				// } else if (worldZ <= CHUNK_SIZE + 9) {
				// 	this->blocks.push_back(WATER_BLOCK);
				// } else {
				// 	this->blocks.push_back(AIR_BLOCK);
				// }

				// this->blocks.push_back(GRASS_BLOCK);
				// this->blocks.push_back(Block(getRandomBlockType()));

			}
		}
	}

	this->dirty = true;
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

void Chunk::generateModel(World& world) {

	Mesh mesh = { 0 };
	Mesh transparentMesh = { 0 };

	// int posX = std::get<0>(this->position);
	// int posY = std::get<1>(this->position);
	// int posZ = std::get<2>(this->position);
	// mesh.vaoId = posX * CHUNK_SIZE * CHUNK_SIZE + posY + posZ * CHUNK_SIZE;

	// max vertices: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 3 floats per vertex
	float* vertices = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 3 * sizeof(float));
	float* transparentVertices = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 3 * sizeof(float));

	// max colors: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 4 floats per vertex (rgba)
	unsigned char* colors = (unsigned char *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 4 * sizeof(unsigned char));
	unsigned char* transparentColors = (unsigned char *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 4 * sizeof(unsigned char));

	int vertexCount = 0;
	int transparentVertexCount = 0;

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {

				Block block = this->getBlockAt(x, y, z);
				if (block.blockType == BlockType::AIR) {
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

				for (size_t i = 0; i < 6; i++) { // 6 faces per block
					Dir dir = allDirEnums[i];
					tuple<int, int, int> dirTuple = allDirTuples[i];

					Block neighbor = this->getBlockInDirection(x, y, z, dirTuple, world);
					if (!neighbor.transparent || (block.blockType == BlockType::WATER && neighbor.blockType == BlockType::WATER)) {
						continue;
					}

					for (size_t j = 0; j < 2; j++) { // 2 triangles per face
						for (size_t k = 0; k < 3; k++) { // 3 vertices per triangle

							Vector3 vertex = Vector3Add(pos, CUBE_VERTICES[allTriangleOffsets[i][j][k]]);
							Color color = block.getColor(dir);

							if (block.transparent) {
								transparentVertices[transparentVertexCount * 3 + 0] = vertex.x;
								transparentVertices[transparentVertexCount * 3 + 1] = vertex.y;
								// transparentVertices[transparentVertexCount * 3 + 2] = vertex.z;
								transparentVertices[transparentVertexCount * 3 + 2] = pos.z + CUBE_VERTICES[allTriangleOffsets[i][j][k]].z * (shortenZ ? 0.8f : 1.0f);

								transparentColors[transparentVertexCount * 4 + 0] = color.r;
								transparentColors[transparentVertexCount * 4 + 1] = color.g;
								transparentColors[transparentVertexCount * 4 + 2] = color.b;
								transparentColors[transparentVertexCount * 4 + 3] = color.a;

								transparentVertexCount++;
							}
							else {
								vertices[vertexCount * 3 + 0] = vertex.x;
								vertices[vertexCount * 3 + 1] = vertex.y;
								vertices[vertexCount * 3 + 2] = vertex.z;

								colors[vertexCount * 4 + 0] = color.r;
								colors[vertexCount * 4 + 1] = color.g;
								colors[vertexCount * 4 + 2] = color.b;
								colors[vertexCount * 4 + 3] = color.a;

								vertexCount++;
							}
						}
					}
				}
			}
		}
	}


	this->blank = vertexCount == 0;
	this->transparentBlank = transparentVertexCount == 0;


	if (!this->blank || !this->transparentBlank) {

		mesh.vertexCount = vertexCount;
		mesh.triangleCount = vertexCount / 3;

		transparentMesh.vertexCount = transparentVertexCount;
		transparentMesh.triangleCount = transparentVertexCount / 3;

		// 3 floats per vertex, 4 colors (unsigned char) per vertex
		mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
		mesh.colors = (unsigned char *)malloc(mesh.vertexCount * 4 * sizeof(unsigned char));

		transparentMesh.vertices = (float *)malloc(transparentMesh.vertexCount * 3 * sizeof(float));
		transparentMesh.colors = (unsigned char *)malloc(transparentMesh.vertexCount * 4 * sizeof(unsigned char));


		std::memcpy(mesh.vertices, vertices, mesh.vertexCount * 3 * sizeof(float));
		std::memcpy(mesh.colors,   colors,   mesh.vertexCount * 4 * sizeof(unsigned char));

		std::memcpy(transparentMesh.vertices, transparentVertices, transparentMesh.vertexCount * 3 * sizeof(float));
		std::memcpy(transparentMesh.colors,   transparentColors,   transparentMesh.vertexCount * 4 * sizeof(unsigned char));


		UnloadMesh(this->oldMesh);
		UploadMesh(&mesh, false);

		UnloadMesh(this->transparentOldMesh);
		UploadMesh(&transparentMesh, false);


		this->model = LoadModelFromMesh(mesh);
		this->oldMesh = mesh;

		this->transparentModel = LoadModelFromMesh(transparentMesh);
		this->transparentOldMesh = transparentMesh;
	}


	free(vertices);
	free(colors);

	free(transparentVertices);
	free(transparentColors);


	this->dirty = false;
}

bool Chunk::inBounds(int x, int y, int z) { // static
	return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
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

				if (block.transparent) {
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
#include "raylib.h"

// #include <iostream>

#include <stdlib.h>
#include <cstring> // memcpy

#include <vector>
#include <tuple>

#include "include/block.h"
#include "include/world.h"
#include "include/blockType.h"
#include "include/dir.h"
#include "include/Vector3Util.h"
#include "include/consts.h"

using std::vector;
using std::tuple;

#include "include/chunk.h"

Chunk::Chunk(const tuple<int, int, int>& position) {
	this->position = position;

	this->blocks = vector<Block>();
	this->blocks.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
	
	// this->mesh = { 0 };
	// UploadMesh(&this->mesh, true);
	this->model = { 0 };
	this->dirty = true;

	this->oldMesh = { 0 };
}
Chunk::~Chunk() {
	// TODO
	// UnloadModel(this->model);
}

Block Chunk::getBlockAt(size_t x, size_t y, size_t z) const {
	return this->blocks[x * CHUNK_SIZE * CHUNK_SIZE + y + z * CHUNK_SIZE];
}
Vector3 Chunk::getWorldPos() const {
	return (Vector3) {
		(float)std::get<0>(this->position) * CHUNK_SIZE,
		(float)std::get<1>(this->position) * CHUNK_SIZE,
		(float)std::get<2>(this->position) * CHUNK_SIZE,
	};
}

void Chunk::generateBlocks() {
	this->blocks.clear();

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				
				BlockType blockType = getRandomBlockType();
				Block block(blockType);
				this->blocks.push_back(block);

			}
		}
	}

	this->dirty = true;
}

void Chunk::generateModel(const World& world) {

	Mesh mesh = { 0 };

	// int posX = std::get<0>(this->position);
	// int posY = std::get<1>(this->position);
	// int posZ = std::get<2>(this->position);
	// mesh.vaoId = posX * CHUNK_SIZE * CHUNK_SIZE + posY + posZ * CHUNK_SIZE;

	// max vertices: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 3 floats per vertex
	float* vertices = (float *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 3 * sizeof(float));

	// max colors: 6 faces per block, 2 triangles per face, 3 vertices per triangle, 4 floats per vertex (rgba)
	unsigned char* colors = (unsigned char *)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 2 * 3 * 4 * sizeof(unsigned char));

	int vertexCount = 0;

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {

				Block block = this->getBlockAt(x, y, z);
				if (block.transparent) {
					continue;
				}

				Vector3 pos = Vector3FromInts(x, y, z);

				for (size_t i = 0; i < 6; i++) { // 6 faces per block
					// Dir dir = allDirEnums[i];
					tuple<int, int, int> dirTuple = allDirTuples[i];
					// size_t triangleOffsets[2][3] = allTriangleOffsets[i];

					int nx = x + std::get<0>(dirTuple);
					int ny = y + std::get<1>(dirTuple);
					int nz = z + std::get<2>(dirTuple);

					Block neighbor(BlockType::AIR); // TODO: use pointer

					if (Chunk::inBounds(nx, ny, nz)) {
						neighbor = this->getBlockAt(nx, ny, nz);
					}
					else {
						neighbor = world.getBlockAt(
							std::make_tuple(
								std::get<0>(this->position) + std::get<0>(dirTuple),
								std::get<1>(this->position) + std::get<1>(dirTuple),
								std::get<2>(this->position) + std::get<2>(dirTuple)
							),
							EUCMOD(nx, CHUNK_SIZE),
							EUCMOD(ny, CHUNK_SIZE),
							EUCMOD(nz, CHUNK_SIZE)
						);
					}

					if (!neighbor.transparent) {
						continue;
					}

					for (size_t j = 0; j < 2; j++) { // 2 triangles per face
						for (size_t k = 0; k < 3; k++) { // 3 vertices per triangle
							Vector3 vertex = pos + CUBE_VERTICES[allTriangleOffsets[i][j][k]];
							Color color = block.getColor();

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

	mesh.vertexCount = vertexCount;
	// std::cout << "vertexCount: " << vertexCount << std::endl;
	mesh.triangleCount = vertexCount / 3;

	mesh.vertices = (float *)malloc(mesh.vertexCount * 3 * sizeof(float));
	mesh.colors = (unsigned char *)malloc(mesh.vertexCount * 4 * sizeof(unsigned char));

	std::memcpy(mesh.vertices, vertices, mesh.vertexCount * 3 * sizeof(float));
	std::memcpy(mesh.colors, colors, mesh.vertexCount * 4 * sizeof(unsigned char));

	free(vertices);
	free(colors);

	UnloadMesh(this->oldMesh);
	UploadMesh(&mesh, false);

	this->model = LoadModelFromMesh(mesh);
	this->oldMesh = mesh;

	this->dirty = false;
}

bool Chunk::inBounds(int x, int y, int z) { // static
	return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}
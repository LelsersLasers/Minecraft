#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "include/block.h"
#include "include/world.h"
#include "include/blockType.h"
#include "include/triangle.h"
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

	this->triangles = vector<Triangle>();
	
	// this->mesh = { 0 };
	// UploadMesh(&this->mesh, true);
	this->model = { 0 };
	this->dirty = true;

	this->oldMesh = { 0 };
}
Chunk::~Chunk() {
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

void Chunk::generateTriangles(const World& world) {
	this->triangles.clear();

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {

				Block block = this->getBlockAt(x, y, z);
				if (block.transparent) {
					continue;
				}

				Vector3 pos = Vector3FromInts(x, y, z);

				for (size_t i = 0; i < 6; i++) {
					// Dir dir = allDirEnums[i];
					tuple<int, int, int> dirTuple = allDirTuples[i];
					// size_t triangleOffsets[2][3] = allTriangleOffsets[i];

					int nx = x + std::get<0>(dirTuple);
					int ny = y + std::get<1>(dirTuple);
					int nz = z + std::get<2>(dirTuple);

					
					Block neighbor(AIR); // TODO: use pointer

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

					for (int j = 0; j < 2; j++) {
						this->triangles.push_back(
							Triangle(
								pos + CUBE_VERTICES[allTriangleOffsets[i][j][0]],
								pos + CUBE_VERTICES[allTriangleOffsets[i][j][1]],
								pos + CUBE_VERTICES[allTriangleOffsets[i][j][2]],
								block.getColor()
							)
						);
					}

				}

			}
		}
	}

	this->dirty = false;
}

void Chunk::generateModel() {

	Mesh mesh = { 0 };

	// int posX = std::get<0>(this->position);
	// int posY = std::get<1>(this->position);
	// int posZ = std::get<2>(this->position);
	// mesh.vaoId = posX * CHUNK_SIZE * CHUNK_SIZE + posY + posZ * CHUNK_SIZE;
	
	// 3 vertices per triangle, 3 floats per vertex
	mesh.vertices = (float *)malloc(this->triangles.size() * 3 * 3 * sizeof(float));
	
	// 3 vertices per triangle, 4 unsigned chars per color
	mesh.colors = (unsigned char *)malloc(this->triangles.size() * 3 * 4 * sizeof(unsigned char));
	
	mesh.vertexCount = this->triangles.size() * 3;
	mesh.triangleCount = this->triangles.size();


	for (size_t i = 0; i < this->triangles.size(); i++) { // for each triangle
		Triangle triangle = this->triangles[i];
		for (size_t j = 0; j < 3; j++) { // for each vertex
			mesh.vertices[i * 9 + j * 3 + 0] = triangle.vertices[j].x;
			mesh.vertices[i * 9 + j * 3 + 1] = triangle.vertices[j].y;
			mesh.vertices[i * 9 + j * 3 + 2] = triangle.vertices[j].z;

			mesh.colors[i * 12 + j * 4 + 0] = triangle.color.r;
			mesh.colors[i * 12 + j * 4 + 1] = triangle.color.g;
			mesh.colors[i * 12 + j * 4 + 2] = triangle.color.b;
			mesh.colors[i * 12 + j * 4 + 3] = triangle.color.a;
		}
	}

	UnloadMesh(this->oldMesh);
	UploadMesh(&mesh, false);

	this->model = LoadModelFromMesh(mesh);
	this->oldMesh = mesh;
}

bool Chunk::inBounds(int x, int y, int z) { // static
	return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}
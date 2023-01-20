#include "raylib.h"

#include <stdlib.h>

#include <vector>
#include <tuple>

#include "include/block.h"
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
	this->model = { 0 };
	this->dirty = true;
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

void Chunk::generateTriangles() {
	this->triangles.clear();

	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {

				Block block = this->getBlockAt(x, y, z);
				if (block.transparent) {
					continue;
				}

				Vector3 pos = this->getWorldPos() + Vector3FromInts(x, y, z);

				for (size_t i = 0; i < 6; i++) {
					// Dir dir = allDirEnums[i];
					tuple<int, int, int> dirTuple = allDirTuples[i];
					// size_t triangleOffsets[2][3] = allTriangleOffsets[i];

					int nx = x + std::get<0>(dirTuple);
					int ny = y + std::get<1>(dirTuple);
					int nz = z + std::get<2>(dirTuple);

					
					Block neighbor(AIR);

					if (Chunk::inBounds(nx, ny, nz)) {
						neighbor = this->getBlockAt(nx, ny, nz);
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
	
	// 3 vertices per triangle, 3 floats per vertex
	mesh.vertices = (float *)malloc(this->triangles.size() * 3 * 3 * sizeof(float));
	mesh.vertexCount = this->triangles.size() * 3;
	mesh.triangleCount = this->triangles.size();


	for (size_t i = 0; i < this->triangles.size(); i++) {
		Triangle triangle = this->triangles[i];
		for (size_t j = 0; j < 3; j++) {
			mesh.vertices[i * 9 + j * 3 + 0] = triangle.vertices[j].x;
			mesh.vertices[i * 9 + j * 3 + 1] = triangle.vertices[j].y;
			mesh.vertices[i * 9 + j * 3 + 2] = triangle.vertices[j].z;
		}
	}


	UploadMesh(&mesh, false);

	this->model = LoadModelFromMesh(mesh);

}

bool Chunk::inBounds(int x, int y, int z) { // static
	return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}
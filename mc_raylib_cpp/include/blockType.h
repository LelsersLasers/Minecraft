#ifndef BLOCK_TYPE_GUARD
#define BLOCK_TYPE_GUARD

#include "raylib.h"

#include <string>

#include "dir.h"
#include "ChunkModelInfo.h"

enum BlockType {
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	STONE = 3,
	BEDROCK = 4,
	WATER = 5,
	SAND = 6,
	LOG = 7,
	LEAVES = 8,
	SAND_STONE = 9,
};

#define BLOCK_MAX (BlockType::SAND_STONE)

Vector2 getTexcoordsAir(Dir dir);
Vector2 getTexcoordsGrass(Dir dir);
Vector2 getTexcoordsDirt(Dir dir);
Vector2 getTexcoordsStone(Dir dir);
Vector2 getTexcoordsBedrock(Dir dir);
Vector2 getTexcoordsWater(Dir dir);
Vector2 getTexcoordsSand(Dir dir);
Vector2 getTexcoordsLog(Dir dir);
Vector2 getTexcoordsLeaves(Dir dir);
Vector2 getTexcoordsSandStoneBlock(Dir dir);

Vector2 (*getTexcoordsFn(const BlockType& blockType))(Dir dir);

bool getTransparent(const BlockType& blockType);
bool getSolid(const BlockType& blockType);
ChunkModelInfo getChunkModelInfo(const BlockType& blockType);

// BlockType getRandomBlockType();

std::string getBlockName(const BlockType& blockType);

#endif
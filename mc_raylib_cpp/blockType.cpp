#include "raylib.h"
#include "raymath.h"

#include "stdlib.h"

#include "include/blockType.h"
#include "include/dir.h"
#include "include/ChunkModelInfo.h"


Vector2 getTexcoordsAir(Dir _dir)		{ return (Vector2){ -1, -1 }; }
Vector2 getTexcoordsGrass(Dir dir) {
	switch (dir) {
		case Dir::Top:
			return (Vector2){ 0.0f, 0.0f };
		case Dir::Bottom:
			return (Vector2){ 2.0f, 0.0f };
		default: // sides
			return (Vector2){ 3.0f, 0.0f };
	}
}
Vector2 getTexcoordsDirt(Dir dir)		{ return (Vector2){ 2.0f, 0.0f }; }
Vector2 getTexcoordsStone(Dir dir)		{ return (Vector2){ 1.0f, 0.0f }; }
Vector2 getTexcoordsBedrock(Dir _dir)	{ return (Vector2){ 5.0f, 2.0f }; }
Vector2 getTexcoordsWater(Dir _dir)		{ return (Vector2){ 3.0f, 4.0f }; }
Vector2 getTexcoordsSand(Dir dir)		{ return (Vector2){ 2.0f, 1.0f }; }
Vector2 getTexcoordsLog(Dir dir) {
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return (Vector2){ 5.0f, 1.0f };
		default: // sides
			return (Vector2){ 4.0f, 1.0f };
	}
}
Vector2 getTexcoordsLeaves(Dir dir)		{ return (Vector2){ 4.0f, 3.0f }; }

Vector2 (*getTexcoordsFn(const BlockType& blockType))(Dir dir) {
	switch (blockType) {
		case BlockType::AIR:
			return getTexcoordsAir;
		case BlockType::GRASS:
			return getTexcoordsGrass;
		case BlockType::DIRT:
			return getTexcoordsDirt;
		case BlockType::STONE:
			return getTexcoordsStone;
		case BlockType::BEDROCK:
			return getTexcoordsBedrock;
		case BlockType::WATER:
			return getTexcoordsWater;
		case BlockType::SAND:
			return getTexcoordsSand;
		case BlockType::LOG:
			return getTexcoordsLog;
		case BlockType::LEAVES:
			return getTexcoordsLeaves;
        default: // should not be reached
            return getTexcoordsAir; 
	}
}

bool getTransparent(const BlockType& blockType) {
	switch (blockType) {
		case BlockType::AIR:
		case BlockType::WATER:
		case BlockType::LEAVES:
			return true;
		case BlockType::GRASS:
		case BlockType::DIRT:
		case BlockType::STONE:
		case BlockType::BEDROCK:
		case BlockType::SAND:
		case BlockType::LOG:
			return false;
        default: // should not be reached
            return false;
	}
}
bool getSolid(const BlockType& blockType) {
	switch (blockType) {
		case BlockType::AIR:
			return false;
		case BlockType::GRASS:
			return true;
		case BlockType::DIRT:
			return true;
		case BlockType::STONE:
			return true;
		case BlockType::BEDROCK:
			return true;
		case BlockType::WATER:
			return false;
		case BlockType::SAND:
			return true;
		case BlockType::LOG:
			return true;
		case BlockType::LEAVES:
			return true;
		default: // should not be reached
			return false;
	}
}
ChunkModelInfo getChunkModelInfo(const BlockType& blockType) {
	switch (blockType) {
		case BlockType::AIR: // doesn't actually do anything
			return ChunkModelInfo::FULL_BLOCKS;
		case BlockType::GRASS:
		case BlockType::DIRT:
		case BlockType::STONE:
		case BlockType::BEDROCK:
		case BlockType::SAND:
		case BlockType::LOG:
			return ChunkModelInfo::FULL_BLOCKS;
		case BlockType::WATER:
			return ChunkModelInfo::WATER_BLOCKS;
		case BlockType::LEAVES:
			return ChunkModelInfo::LEAVES_BLOCKS;
		default: // should not be reached
			return ChunkModelInfo::FULL_BLOCKS;
	}
}

BlockType getRandomBlockType() {
	return (BlockType)(rand() % (BLOCK_MAX + 1));
}


std::string getBlockName(const BlockType& BlockType) {
	switch (BlockType) {
		case BlockType::AIR:
			return "Air";
		case BlockType::GRASS:
			return "Grass";
		case BlockType::DIRT:
			return "Dirt";
		case BlockType::STONE:
			return "Stone";
		case BlockType::BEDROCK:
			return "Bedrock";
		case BlockType::WATER:
			return "Water";
		case BlockType::SAND:
			return "Sand";
		case BlockType::LOG:
			return "Log";
		case BlockType::LEAVES:
			return "Leaves";
		default: // should not be reached
			return "Air";
	}
}
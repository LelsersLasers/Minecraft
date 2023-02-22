#include "raylib.h"
#include "raymath.h"

#include "stdlib.h"

#include "include/blockType.h"
#include "include/dir.h"


Vector2 getTexcoordsAir(Dir _dir)		{ return (Vector2){ -1, -1 }; }
Vector2 getTexcoordsGrass(Dir dir) {
	switch (dir) {
		case Dir::Top:
			return (Vector2){ 0, 0 };
		case Dir::Bottom:
			return (Vector2){ 2, 0 };
		default: // sides
			return (Vector2){ 3, 0 };
	}
}
Vector2 getTexcoordsDirt(Dir dir)		{ return (Vector2){ 2, 0 }; }
Vector2 getTexcoordsStone(Dir dir)		{ return (Vector2){ 1, 0 }; }
Vector2 getTexcoordsBedrock(Dir _dir)	{ return (Vector2){ 5, 2 }; }
Vector2 getTexcoordsWater(Dir _dir)		{ return (Vector2){ 3, 4 }; }
Vector2 getTexcoordsSand(Dir dir)		{ return (Vector2){ 2, 1 }; }
Vector2 getTexcoordsLog(Dir dir) {
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return (Vector2){ 5, 1 };
		default: // sides
			return (Vector2){ 4, 1 };
	}
}
Vector2 getTexcoordsLeaves(Dir dir)		{ return (Vector2){ 4, 3 }; }

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
			return true;
		case BlockType::GRASS:
			return false;
		case BlockType::DIRT:
			return false;
		case BlockType::STONE:
			return false;
		case BlockType::BEDROCK:
			return false;
		case BlockType::WATER:
			return true;
		case BlockType::SAND:
			return false;
		case BlockType::LOG:
			return false;
		case BlockType::LEAVES:
			return true;
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
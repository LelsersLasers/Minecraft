#ifndef BLOCK_TYPE_GUARD
#define BLOCK_TYPE_GUARD

#include "raylib.h"

#include <string>

#include "dir.h"

enum BlockType {
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	STONE = 3,
	BEDROCK = 4,
	WATER = 5,
	SAND = 6,
	LOG = 7,
};

#define BLOCK_MAX (LOG)

Color getColorAir(Dir dir);
Color getColorGrass(Dir dir);
Color getColorDirt(Dir dir);
Color getColorStone(Dir dir);
Color getColorBedrock(Dir dir);
Color getColorWater(Dir dir);
Color getColorSand(Dir dir);
Color getColorLog(Dir dir);

Color (*getColorFn(const BlockType& blockType))(Dir dir);

bool getTransparent(const BlockType& blockType);

BlockType getRandomBlockType();

std::string getBlockName(const BlockType& blockType);

#endif
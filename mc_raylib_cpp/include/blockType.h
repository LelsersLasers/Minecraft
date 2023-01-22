#ifndef BLOCK_TYPE_GUARD
#define BLOCK_TYPE_GUARD

#include "raylib.h"

#include "dir.h"

enum BlockType {
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	STONE = 3,
	BEDROCK = 4,
};

#define BLOCK_MAX (BEDROCK)

Color getColorAir(Dir dir);
Color getColorGrass(Dir dir);
Color getColorDirt(Dir dir);
Color getColorStone(Dir dir);
Color getColorBedrock(Dir dir);

Color (*getColorFn(const BlockType& blockType))(Dir dir);

bool getTransparent(const BlockType& blockType);

BlockType getRandomBlockType();



#endif
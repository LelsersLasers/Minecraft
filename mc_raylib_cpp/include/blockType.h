#ifndef BLOCK_TYPE_GUARD
#define BLOCK_TYPE_GUARD

#include "raylib.h"

enum BlockType {
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	STONE = 3,
	BEDROCK = 4,
};

#define BLOCK_MAX (BEDROCK)

Color getColorAir();
Color getColorGrass();
Color getColorDirt();
Color getColorStone();
Color getColorBedrock();

Color (*getColorFn(const BlockType& blockType))();

bool getTransparent(const BlockType& blockType);

BlockType getRandomBlockType();



#endif
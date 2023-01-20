#ifndef BLOCK_TYPE_GUARD
#define BLOCK_TYPE_GUARD

#include "raylib.h"

enum BlockType {
	AIR = 0,
	GRASS = 1,
};

#define BLOCK_MAX (GRASS)

Color getColorAir();
Color getColorGrass();

Color (*getColorFn(const BlockType& blockType))();

bool getTransparent(const BlockType& blockType);

BlockType getRandomBlockType();



#endif
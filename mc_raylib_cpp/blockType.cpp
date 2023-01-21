#include "raylib.h"

#include "stdlib.h"

#include "include/blockType.h"



Color getColorAir()		{ return BLANK; }
Color getColorGrass()	{ return GREEN; }
Color getColorDirt()	{ return BROWN; }
Color getColorStone()	{ return GRAY; }
Color getColorBedrock()	{ return DARKGRAY; }

Color (*getColorFn(const BlockType& blockType))() {
	switch (blockType) {
		case BlockType::AIR:
			return getColorAir;
		case BlockType::GRASS:
			return getColorGrass;
		case BlockType::DIRT:
			return getColorDirt;
		case BlockType::STONE:
			return getColorStone;
		case BlockType::BEDROCK:
			return getColorBedrock;
        default: // should not be reached
            return getColorAir; 
	}
}

bool getTransparent(const BlockType& blockType) {
	switch (blockType) {
		case BlockType::AIR:
			return true;
		// case BlockType::GRASS:
		// 	return false;
		// case BlockType::DIRT:
		// 	return false;
		// case BlockType::STONE:
		// 	return false;
		// case BlockType::BEDROCK:
		// 	return false;
        default: // BlockType::AIR is the only transparent block rn
            return false;
	}
}

BlockType getRandomBlockType() {
	return (BlockType)(rand() % (BLOCK_MAX + 1));
	// return BlockType::GRASS;
}

#include "raylib.h"

#include "stdlib.h"

#include "include/blockType.h"



Color getColorAir() {
	return BLANK;
}
Color getColorGrass() {
	return GREEN;
}

Color (*getColorFn(const BlockType& blockType))() {
	switch (blockType) {
		case BlockType::AIR:
			return getColorAir;
		case BlockType::GRASS:
			return getColorGrass;
        default: // should not be reached
            return getColorAir; 
	}
}

bool getTransparent(const BlockType& blockType) {
	switch (blockType) {
		case BlockType::AIR:
			return true;
		case BlockType::GRASS:
			return false;
        default: // should not be reached
            return true;
	}
}

BlockType getRandomBlockType() {
	return (BlockType)(rand() % (BLOCK_MAX + 1));
	// return BlockType::GRASS;
}

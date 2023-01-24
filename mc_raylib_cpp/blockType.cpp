#include "raylib.h"

#include "stdlib.h"

#include "include/blockType.h"
#include "include/dir.h"

#include "include/ColorUtil.h"


Color getColorAir(Dir _dir)		{ return BLANK; }
Color getColorGrass(Dir dir) {
	// return GREEN;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return GREEN;
		case Dir::Right:
			return GREEN * 0.9;
		case Dir::Left:
			return GREEN * 0.7;
		case Dir::Forward:
			return GREEN * 0.8;
		case Dir::Backward:
			return GREEN * 0.6;
		default: // should not be reached
			return GREEN;
	}
}
Color getColorDirt(Dir _dir)	{ return BROWN; }
Color getColorStone(Dir _dir)	{ return GRAY; }
Color getColorBedrock(Dir _dir)	{ return DARKGRAY; }

Color (*getColorFn(const BlockType& blockType))(Dir dir) {
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
	// if (rand() % 2 == 0) {
	// 	return BlockType::GRASS;
	// } else {
	// 	return BlockType::AIR;
	// }
}

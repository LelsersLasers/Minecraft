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
		case Dir::Left:
			return GREEN * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return GREEN * 0.8;
		default: // should not be reached
			return GREEN;
	}
}
Color getColorDirt(Dir dir) {
	// return BROWN;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return BROWN;
		case Dir::Right:
		case Dir::Left:
			return BROWN * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return BROWN * 0.8;
		default: // should not be reached
			return BROWN;
	}
}
Color getColorStone(Dir dir) {
	// return GRAY;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return GRAY;
		case Dir::Right:
		case Dir::Left:
			return GRAY * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return GRAY * 0.8;
		default: // should not be reached
			return GRAY;
	}
}
Color getColorBedrock(Dir _dir)	{ return DARKGRAY; }
Color getColorWater(Dir _dir)	{ return ColorAlpha(BLUE, 0.5); }

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
		case BlockType::WATER:
			return getColorWater;
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
		case BlockType::DIRT:
			return false;
		case BlockType::STONE:
			return false;
		case BlockType::BEDROCK:
			return false;
		case BlockType::WATER:
			return true;
        default: // should not be reached
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
		default: // should not be reached
			return "Air";
	}
}
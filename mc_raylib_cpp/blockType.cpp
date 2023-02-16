#include "raylib.h"

#include "stdlib.h"

#include "include/blockType.h"
#include "include/dir.h"

#include "include/ColorUtil.h"


Color getColorAir(Dir _dir)		{ return BLANK; }
Color getColorGrass(Dir dir) {
	Color baseColor = GREEN;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}
Color getColorDirt(Dir dir) {
	Color baseColor = BROWN;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}
Color getColorStone(Dir dir) {
	Color baseColor = GRAY;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}
Color getColorBedrock(Dir _dir)	{ return DARKGRAY; }
Color getColorWater(Dir _dir)	{ return ColorAlpha(BLUE, 0.8); }
Color getColorSand(Dir dir) {
	Color baseColor = YELLOW;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}
Color getColorLog(Dir dir) {
	Color baseColor = DARKBROWN;
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor * 0.5;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}
Color getColorLeaves(Dir dir) {
	Color baseColor = ColorAlpha(RED, 0.3);
	switch (dir) {
		case Dir::Top:
		case Dir::Bottom:
			return baseColor * 0.5;
		case Dir::Right:
		case Dir::Left:
			return baseColor * 0.9;
		case Dir::Forward:
		case Dir::Backward:
			return baseColor * 0.8;
		default: // should not be reached
			return BLANK;
	}
}

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
		case BlockType::SAND:
			return getColorSand;
		case BlockType::LOG:
			return getColorLog;
		case BlockType::LEAVES:
			return getColorLeaves;
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
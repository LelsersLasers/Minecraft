#include "include/block.h"
#include "include/blockType.h"
#include "include/consts.h"

#include "include/biomeType.h"

// TODO: why can't I use the block consts in block.h?

Block getTopBlock(const BiomeType& biomeType) {
	switch (biomeType) {
		case BiomeType::HILLS:
			return Block(BlockType::GRASS);
		case BiomeType::NEAR_WATER:
			return Block(BlockType::SAND);
		default: // should not be reached
			return Block(BlockType::AIR);
	}
}

Block getSecondBlock(const BiomeType& biomeType) {
	switch (biomeType) {
		case BiomeType::HILLS:
			return Block(BlockType::DIRT);
		case BiomeType::NEAR_WATER:
			return Block(BlockType::SAND_STONE);
		default: // should not be reached
			return Block(BlockType::AIR);
	}
}
int getSecondBlockLevelMin(const BiomeType& biomeType) {
	switch (biomeType) {
		case BiomeType::HILLS:
			return 3;
		case BiomeType::NEAR_WATER:
			return 4;
		default: // should not be reached
			return -1;
	}
}
int getSecondBlockLevelMax(const BiomeType& biomeType) {
	switch (biomeType) {
		case BiomeType::HILLS:
			return 5;
		case BiomeType::NEAR_WATER:
			return 6;
		default: // should not be reached
			return -1;
	}
}

std::string getBiomeName(const BiomeType& biomeType) {
	switch (biomeType) {
		case BiomeType::HILLS:
			return "Hills";
		case BiomeType::NEAR_WATER:
			return "Near Water";
		default: // should not be reached
			return "Unknown";
	}
}

BiomeType getBiomeFromHeight(int height) {
	if (height < WATER_LEVEL + 2) {
		return BiomeType::NEAR_WATER;
	} else {
		return BiomeType::HILLS;
	}
}
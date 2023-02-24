#ifndef BIOME_TYPE_GUARD
#define BIOME_TYPE_GUARD

#include <string>

#include "block.h"


enum BiomeType {
	HILLS = 0 , 
	NEAR_WATER = 1,
};

#define BIOME_MAX (BiomeType::NEAR_WATER)


Block getTopBlock(const BiomeType& biomeType);

Block getSecondBlock(const BiomeType& biomeType);
int getSecondBlockLevelMin(const BiomeType& biomeType);
int getSecondBlockLevelMax(const BiomeType& biomeType);

std::string getBiomeName(const BiomeType& biomeType);

BiomeType getBiomeFromHeight(int height);


#endif
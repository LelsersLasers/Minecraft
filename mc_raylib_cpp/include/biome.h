#ifndef BIOME_GUARD
#define BIOME_GUARD

#include "block.h"
#include "biomeType.h"


class Biome {

	public:
		BiomeType biomeType;

		Block topBlock = GRASS_BLOCK;

		Block secondBlock = DIRT_BLOCK;
		int secondBlockLevelMin;
		int secondBlockLevelMax;

		Biome(const BiomeType& biomeType);

		static Biome biomeFromType(const BiomeType& biomeType);
};


const Biome HILLS_BIOME(BiomeType::HILLS);
const Biome NEAR_WATER_BIOME(BiomeType::NEAR_WATER);


#endif
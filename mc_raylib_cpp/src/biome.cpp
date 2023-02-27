#include "include/biomeType.h"
#include "include/consts.h"
#include "include/block.h"

#include "include/biome.h"


Biome::Biome(const BiomeType& biomeType) {
	this->biomeType = biomeType;

	this->topBlock = getTopBlock(biomeType);

	this->secondBlock = getSecondBlock(biomeType);
	this->secondBlockLevelMin = getSecondBlockLevelMin(biomeType);
	this->secondBlockLevelMax = getSecondBlockLevelMax(biomeType);

}

Biome Biome::biomeFromType(const BiomeType& biomeType) {
    switch (biomeType) {
        case BiomeType::HILLS:
			return HILLS_BIOME;
		case BiomeType::NEAR_WATER:
			return NEAR_WATER_BIOME;
		default: // should not be reached
			return HILLS_BIOME;
	}
}

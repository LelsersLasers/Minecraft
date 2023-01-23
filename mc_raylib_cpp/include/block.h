#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include "raylib.h"

#include "blockType.h"
#include "dir.h"


class Block {

	public:
		BlockType blockType;
		bool transparent;
		Color (*getColor)(Dir dir);


		Block(const BlockType& blockType);

	
};


const Block AIR_BLOCK(BlockType::AIR);
// const Block GRASS_BLOCK(BlockType::GRASS);
// const Block DIRT_BLOCK(BlockType::DIRT);
// const Block STONE_BLOCK(BlockType::STONE);
// const Block BEDROCK_BLOCK(BlockType::BEDROCK);



#endif
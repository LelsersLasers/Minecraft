#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include "raylib.h"

#include "blockType.h"
#include "dir.h"
#include "ChunkModelInfo.h"


class Block {

	public:
		BlockType blockType;
		bool transparent;
		bool solid;
		ChunkModelInfo chunkModelInfo;
		Vector2 (*getTexcoords)(Dir dir);


		Block(const BlockType& blockType);

	
};


const Block AIR_BLOCK(BlockType::AIR);
const Block GRASS_BLOCK(BlockType::GRASS);
const Block DIRT_BLOCK(BlockType::DIRT);
const Block STONE_BLOCK(BlockType::STONE);
const Block BEDROCK_BLOCK(BlockType::BEDROCK);
const Block WATER_BLOCK(BlockType::WATER);
const Block SAND_BLOCK(BlockType::SAND);
const Block LOG_BLOCK(BlockType::LOG);
const Block LEAVES_BLOCK(BlockType::LEAVES);
const Block SAND_STONE_BLOCK(BlockType::SAND_STONE);



#endif
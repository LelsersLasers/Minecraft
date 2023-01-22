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






#endif
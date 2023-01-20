#ifndef BLOCK_GUARD
#define BLOCK_GUARD

#include "raylib.h"

#include "blockType.h"


class Block {

	public:
		BlockType blockType;
		bool transparent;
		Color (*getColor)();


		Block(const BlockType& blockType);

	
};






#endif
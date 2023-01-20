#include "raylib.h"

#include "include/blockType.h"

#include "include/block.h"


Block::Block(const BlockType& blockType) {
	this->blockType = blockType;
	this->transparent = getTransparent(blockType);
	this->getColor = getColorFn(blockType);
}
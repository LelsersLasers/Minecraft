#ifndef BLOCK_PLACE_REQUEST_GUARD
#define BLOCK_PLACE_REQUEST_GUARD


#include <tuple>
#include "block.h"


struct BlockPlaceRequest {
	tuple<int, int, int> chunkPos;
	
	size_t x;
	size_t y;
	size_t z;

	Block block;
};


#endif
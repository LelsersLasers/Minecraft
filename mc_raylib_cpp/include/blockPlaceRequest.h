#ifndef BLOCK_PLACE_REQUEST_GUARD
#define BLOCK_PLACE_REQUEST_GUARD

#include <tuple>
#include <vector>

#include "block.h"
#include "blockType.h"

using std::tuple;
using std::vector;

struct BlockPlaceRequest {
	tuple<int, int, int> chunkPos;
	
	size_t x;
	size_t y;
	size_t z;

	Block block;

    vector<BlockType> canOverwrite;
};


#endif

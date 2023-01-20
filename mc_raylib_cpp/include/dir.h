#ifndef DIR_GUARD
#define DIR_GUARD

#include <tuple>
using std::tuple;

enum Dir {
	Top = 0,
    Bottom = 1,
    Right = 2,
    Left = 3,
    Forward = 4,
    Backward = 5,
};

Dir allDirEnums[6] = {Top, Bottom, Right, Left, Forward, Backward};

tuple<int, int, int> allDirTuples[6] = {
	{0, 0, 1},
	{0, 0, -1},
	{0, 1, 0},
	{0, -1, 0},
	{1, 0, 0},
	{-1, 0, 0},
};

size_t allTriangleOffsets[6][2][3] = {
    {{4, 5, 7}, {5, 6, 7}},
    {{0, 3, 1}, {1, 3, 2}},
    {{3, 7, 2}, {2, 7, 6}},
    {{0, 1, 4}, {1, 5, 4}},
    {{1, 2, 5}, {5, 2, 6}},
    {{0, 4, 3}, {4, 7, 3}},
};



#endif
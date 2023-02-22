#ifndef DIR_GUARD
#define DIR_GUARD

#include <cstddef> // size_t

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

const Dir allDirEnums[6] = {Top, Bottom, Right, Left, Forward, Backward};

const tuple<int, int, int> allDirTuples[6] = {
	// {0, 0, 1},
	// {0, 0, -1},
	// {0, 1, 0},
	// {0, -1, 0},
	// {1, 0, 0},
	// {-1, 0, 0},
    std::make_tuple(0, 0, 1),
    std::make_tuple(0, 0, -1),
    std::make_tuple(0, 1, 0),
    std::make_tuple(0, -1, 0),
    std::make_tuple(1, 0, 0),
    std::make_tuple(-1, 0, 0),
};

const size_t allTriangleOffsets[6][2][3] = {
    {{4, 5, 7}, {5, 6, 7}}, // top
    {{0, 3, 1}, {1, 3, 2}}, // bottom
    {{3, 7, 2}, {2, 7, 6}}, // right
    {{0, 1, 4}, {1, 5, 4}}, // left
    {{1, 2, 5}, {5, 2, 6}}, // forward
    {{0, 4, 3}, {4, 7, 3}}, // backward
};

const size_t allTexcoordsTriangleOffsets[6][2][3] = {
    {{0, 1, 3}, {1, 2, 3}}, // top
    {{0, 3, 1}, {1, 3, 2}}, // bottom
    {{3, 0, 2}, {2, 0, 1}}, // right
    {{0, 1, 3}, {1, 2, 3}}, // left
    {{1, 2, 0}, {0, 2, 3}}, // forward
    {{0, 3, 1}, {3, 2, 1}}, // backward
};

// {
//     {{0, 1, 3}, {1, 2, 3}}, // top
//     {{0, 3, 1}, {1, 3, 2}}, // bottom
//     {{3, 7, 2}, {2, 7, 6}}, // right
//     {{0, 1, 4}, {1, 5, 4}}, // left
//     {{1, 2, 5}, {5, 2, 6}}, // forward
//     {{0, 4, 3}, {4, 7, 3}}, // backward
// };



#endif
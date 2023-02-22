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
    {{4, 5, 7}, {5, 6, 7}}, // top      abc, bdc
    {{0, 3, 1}, {1, 3, 2}}, // bottom   abc, cbd
    {{3, 7, 2}, {2, 7, 6}}, // right    abc, cbd
    {{0, 1, 4}, {1, 5, 4}}, // left     abc, bdc
    {{1, 2, 5}, {5, 2, 6}}, // forward  abc, cbd
    {{0, 4, 3}, {4, 7, 3}}, // backward abc, bdc
};

const size_t allTexcoordsTriangleOffsets[6][2][3] = {
    {{0, 1, 3}, {1, 2, 3}}, // top
    {{0, 3, 1}, {1, 3, 2}}, // bottom
    {{3, 0, 2}, {2, 0, 1}}, // right
    {{0, 1, 3}, {1, 2, 3}}, // left
    {{0, 1, 3}, {3, 1, 2}}, // forward
    {{0, 3, 1}, {3, 2, 1}}, // backward
};

const Color allDirColorModifiers[6] = {
    {255, 255, 255, 255}, // top        full color
    {255, 255, 255, 255}, // bottom     full color
    {205, 205, 205, 255}, // right      90% color
    {205, 205, 205, 255}, // left       90% color
    {150, 150, 150, 255}, // forward    80% color
    {150, 150, 150, 255}, // backward   80% color
};


#endif
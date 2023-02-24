#ifndef CONSTS_GUARD
#define CONSTS_GUARD


#include "raylib.h"

#include <stdlib.h>


// #define BRANCHLESS_EUCMOD(a, b)  (((a) < 0) * ((((a) % (b)) + (b)) % (b)) + ((a) >= 0) * ((a) % (b)))

#define EUCMOD(a,b) ((a) < 0 ? ((((a) % (b)) + (b)) % (b)) : ((a) % (b))) // range of a is (-inf, inf)
#define EUCMOD_SIMPLE(a,b) (((a) + (b)) % (b)) // range of a is [-b, inf)

// #define BRANCHLESS_MAX(a, b) (((a) > (b)) * (a) + ((a) <= (b)) * (b))
// #define BRANCHLESS_MIN(a, b) (((a) < (b)) * (a) + ((a) >= (b)) * (b))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define RAND(min, max) (rand() % ((max) - (min)) + (min)) // range is [min, max)
#define RAND_CHANCE(chance) (((float)rand() / (float)RAND_MAX) < (chance)) // returns bool

#define TUP_TO_STR(tup) std::to_string(std::get<0>(tup)) + "," + std::to_string(std::get<1>(tup)) + "," + std::to_string(std::get<2>(tup))


#define PI 3.14159265358979323846f // should this also be 'const float'?

const int WINDOW_WIDTH_START = 1280;
const int WINDOW_HEIGHT_START = 720;

const float MOUSE_SENSITIVITY = 1.0f / 400.0f;

const int CHUNK_SIZE = 16; // blocks per chunk ^3
const int WORLD_SIZE = 16; // approx. max world size vertically for chunk generation (in chunks)


const int OCTAVES = 8;
const double NOISE_DIVISOR = CHUNK_SIZE * 1.5;

const float NOISE_RESOLUTION = 8.0f;

const int LOWEST_SURFACE_Z = CHUNK_SIZE;

const int VIEW_DIST = 8; // in chunks

const int WATER_LEVEL = CHUNK_SIZE * WORLD_SIZE / 2;
const int LOWEST_CHUNK_Z = 0;

const float TREE_CHANCE = 0.01f;

const Vector3 CUBE_VERTICES[] = {
	(Vector3){0.0f, 0.0f, 0.0f}, // 0
	(Vector3){1.0f, 0.0f, 0.0f}, // 1
	(Vector3){1.0f, 1.0f, 0.0f}, // 2
	(Vector3){0.0f, 1.0f, 0.0f}, // 3
	(Vector3){0.0f, 0.0f, 1.0f}, // 4
	(Vector3){1.0f, 0.0f, 1.0f}, // 5
	(Vector3){1.0f, 1.0f, 1.0f}, // 6
	(Vector3){0.0f, 1.0f, 1.0f}, // 7
};

// note: Y values are flipped
const Vector2 CUBE_TEXCOORDS[] =  {
	(Vector2){0.0f, 1.0f}, // top left		// 0
	(Vector2){1.0f, 1.0f}, // top right		// 1
	(Vector2){1.0f, 0.0f}, // bottom left		// 2
	(Vector2){0.0f, 0.0f}, // bottom right	// 3
};

const float TEXCOORDS_DIVISOR = 16.0f;


const float REACH = 5.0f;


#endif
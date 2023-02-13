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

#define TUP_TO_STR(tup) std::to_string(std::get<0>(tup)) + "," + std::to_string(std::get<1>(tup)) + "," + std::to_string(std::get<2>(tup))


#define PI 3.14159265358979323846f // should this also be 'const float'?

const int WINDOW_WIDTH_START = 1280;
const int WINDOW_HEIGHT_START = 720;

const float MOUSE_SENSITIVITY = 1.0 / 400.0;

const int CHUNK_SIZE = 16; // blocks per chunk ^3
const int WORLD_SIZE = 16; // chunks per world ^3


const int OCTAVES = 8;
const double PERLIN_NOISE_DIVISOR = CHUNK_SIZE * 10.0;

const int VIEW_DIST = 8; // in chunks

const int WATER_LEVEL = CHUNK_SIZE * WORLD_SIZE / 2;
const int LOWEST_CHUNK_Z = 0;

const Vector3 CUBE_VERTICES[] = {
	(Vector3){0.0, 0.0, 0.0},
	(Vector3){1.0, 0.0, 0.0},
	(Vector3){1.0, 1.0, 0.0},
	(Vector3){0.0, 1.0, 0.0},
	(Vector3){0.0, 0.0, 1.0},
	(Vector3){1.0, 0.0, 1.0},
	(Vector3){1.0, 1.0, 1.0},
	(Vector3){0.0, 1.0, 1.0}, 
};


const int REACH = 5;


#endif
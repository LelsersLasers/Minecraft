#ifndef CONSTS_GUARD
#define CONSTS_GUARD


#include "raylib.h"

#include <stdlib.h>


// #define BRANCHLESS_EUCMOD(a, b)  (((a) < 0) * ((((a) % (b)) + (b)) % (b)) + ((a) >= 0) * ((a) % (b)))

#define EUCMOD(a,b) ((a) < 0 ? ((((a) % (b)) + (b)) % (b)) : ((a) % (b))) // range of a is (-inf, inf)
#define EUCMOD_SIMPLE(a,b) (((a) + (b)) % (b)) // range of a is [-b, inf)

#define BRANCHLESS_MAX(a, b) (((a) > (b)) * (a) + ((a) <= (b)) * (b))
#define BRANCHLESS_MIN(a, b) (((a) < (b)) * (a) + ((a) >= (b)) * (b))

// range: [min, max)
#define RAND(min, max) (rand() % ((max) - (min)) + (min))


#define PI 3.14159265358979323846f // should this also be 'const float'?

const int WINDOW_WIDTH_START = 1280;
const int WINDOW_HEIGHT_START = 720;

const float MOUSE_SENSITIVITY = 1.0 / 400.0;

const int CHUNK_SIZE = 16; // blocks per chunk ^3
const int WORLD_SIZE = 7; // chunks per world ^3

const double PERLIN_NOISE_DIVISOR = CHUNK_SIZE  * 5.0;

// const int VIEW_DIST = 6; // in chunks

const int WATER_LEVEL = 64;

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
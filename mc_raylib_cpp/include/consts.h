#ifndef CONSTS_GUARD
#define CONSTS_GUARD


#include "raylib.h"

#define PI 3.14159265358979323846f // should this also be 'const float'?

const int WINDOW_WIDTH_START = 1280;
const int WINDOW_HEIGHT_START = 720;

const float MOUSE_SENSITIVITY = 1.0 / 400.0;

const int CHUNK_SIZE = 8; // blocks per chunk ^3

const int WORLD_SIZE = 2; // chunks per world ^3

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


#endif
#ifndef TRIANGLE_GUARD
#define TRIANGLE_GUARD

#include "raylib.h"


class Triangle {
	public:
		Vector3 vertices[3];
		Color color;

		Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color);
};


#endif
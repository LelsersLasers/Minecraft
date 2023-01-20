#include "raylib.h"

#include "include/triangle.h"


Triangle::Triangle(Vector3 v1, Vector3 v2, Vector3 v3, Color color) {
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	this->color = color;
}
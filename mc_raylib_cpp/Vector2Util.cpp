#include "raylib.h"

#include <cmath>

#include "include/Vector2Util.h"

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
	return (Vector2){
		lhs.x + rhs.x,
		lhs.y + rhs.y,
	};
}
Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
	return (Vector2){
		lhs.x - rhs.x,
		lhs.y - rhs.y,
	};
}

Vector2& operator+=(Vector2& lhs, const Vector2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Vector2 operator*(const Vector2& lhs, const float& rhs) {
	return (Vector2){
		lhs.x * rhs,
		lhs.y * rhs,
	};
}

float length(const Vector2& vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
Vector2 normalize(const Vector2& vec) {
	float len = length(vec);
	if (len == 0.0) {
		return Vector2Zero();
	}
	else {
		return vec * (1.0 / len);
	}
}

Vector2 Vector2Zero() {
	return (Vector2){ 0.0, 0.0 };
}
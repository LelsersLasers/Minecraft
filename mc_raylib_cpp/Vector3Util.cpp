#include "raylib.h"

#include <cmath>

#include "include/Vector3Util.h"

Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
	return (Vector3){
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z,
	};
}
Vector3 operator-(const Vector3& lhs, const Vector3 &rhs) {
	return (Vector3){
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z,
	};
}

Vector3& operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
Vector3& operator-=(Vector3& lhs, const Vector3& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

Vector3 operator*(const Vector3& lhs, const float& rhs) {
	return (Vector3){
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs,
	};
}

bool operator==(const Vector3& lhs, const Vector3& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
bool operator!=(const Vector3& lhs, const Vector3& rhs) {
	return !(lhs == rhs);
}

float length(const Vector3& vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
Vector3 normalize(const Vector3& vec) {
	float len = length(vec);
	if (len == 0.0) {
		return Vector3Zero();
	}
	else {
		return vec * (1.0 / len);
	}
}

Vector3 Vector3Zero() {
	return (Vector3){ 0.0, 0.0, 0.0 };
}
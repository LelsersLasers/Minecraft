#ifndef VECTOR3_UTIL_GUARD
#define VECTOR3_UTIL_GUARD


#include "raylib.h"

Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& lhs, const Vector3& rhs);

Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
Vector3& operator-=(Vector3& lhs, const Vector3& rhs);

Vector3 operator*(const Vector3 &lhs, const float& rhs);

bool operator==(const Vector3& lhs, const Vector3& rhs);
bool operator!=(const Vector3& lhs, const Vector3& rhs);


float length(const Vector3& vec);
Vector3 normalize(const Vector3& vec);

Vector3 Vector3Zero();

Vector3 Vector3FromInts(int x, int y, int z);
Vector3 Vector3Uniform(float x);

#endif
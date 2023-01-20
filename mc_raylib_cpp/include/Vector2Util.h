#ifndef VECTOR2_UTIL_GUARD
#define VECTOR2_UTIL_GUARD


#include "raylib.h"

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

Vector2& operator+=(Vector2& lhs, const Vector2& rhs);

Vector2 operator*(const Vector2& lhs, const float& rhs);

float length(const Vector2& vec);
Vector2 normalize(const Vector2& vec);

Vector2 Vector2Zero();

#endif
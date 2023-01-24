#ifndef COLOR_UTIL_GUARD
#define COLOR_UTIL_GUARD

#include "raylib.h"

Color operator*(const Color& lhs, const float& rhs);

Color ColorWithTransparency(const Color& color, const float& transparency);



#endif
#include "raylib.h"


#include "include/ColorUtil.h"

Color operator*(const Color& lhs, const float& rhs) {
	return (Color){
		(unsigned char)((float)lhs.r * rhs),
		(unsigned char)((float)lhs.g * rhs),
		(unsigned char)((float)lhs.b * rhs),
		lhs.a
	};
}
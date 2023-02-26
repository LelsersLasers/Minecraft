#include "raylib.h"




#include "include/atlas.h"


Atlas::Atlas(const char* pathToAtlas) {
	Image img = LoadImage(pathToAtlas);
	this->texture = LoadTextureFromImage(img);
	UnloadImage(img);
}
Atlas::~Atlas() {
	UnloadTexture(this->texture);
}
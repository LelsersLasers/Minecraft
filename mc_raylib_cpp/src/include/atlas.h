#ifndef ATLAS_GUARD
#define ATLAS_GUARD

#include "raylib.h"


#include "noCopy.h"


class Atlas : public NoCopy {
	public:

		Texture2D texture;



		Atlas(const char* pathToAtlas);
		~Atlas();
};



#endif
#include "raylib.h"

// #include <math.h>
#include <time.h>

#include <iostream>

#include <vector>

#include "include/consts.h"

#include "include/cameraController.h"

// using std::vector;


int main() {

	bool cursor = false;
	bool wireframe = false;
	bool faces = false;

	srand(time(NULL));

    InitWindow(WINDOW_WIDTH_START, WINDOW_HEIGHT_START, "Minecraft ig");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

	DisableCursor();

	CameraController cameraController; // uses default defined constructor


    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();
        cameraController.update();

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            BeginMode3D(cameraController.camera);
            {
                DrawCube((Vector3){ 0.0, 0.0, 0.0 }, 1.0, 1.0, 1.0, RED);
            }
        }
        EndDrawing();
    }




	return 0;
}
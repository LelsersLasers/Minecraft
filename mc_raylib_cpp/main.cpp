#include "raylib.h"

// #include <math.h>
#include <time.h>

#include <iostream>

#include <vector>

#include "include/consts.h"

#include "include/Vector3Util.h"
// #include "include/Vector2Util.h"

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
		{
			Vector3 moveVec = Vector3Zero();

			if (IsKeyDown(KEY_W)) {
				moveVec += cameraController.calcForward();
			}
			if (IsKeyDown(KEY_S)) {
				moveVec -= cameraController.calcForward();
			}
			if (IsKeyDown(KEY_A)) {
				moveVec -= cameraController.calcRight();
			}
			if (IsKeyDown(KEY_D)) {
				moveVec += cameraController.calcRight();
			}

			if (moveVec != Vector3Zero()) {
				moveVec = normalize(moveVec) * delta * 20.0;
				cameraController.moveBy(moveVec);
			}
		}
		

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            BeginMode3D(cameraController.camera);
            {
                DrawCube((Vector3){ 2.0, 2.0, 2.0 }, 1.0, 1.0, 1.0, RED);
            }
			EndMode3D();
        }
        EndDrawing();


		std::cout << "delta: " << delta << std::endl;

		float fps = 1.0 / delta;
		std::cout << "fps: " << fps << std::endl;
		
		Vector3 v = cameraController.calcForward();
		std::cout << v.x << " " << v.y << "" << v.z << std::endl;
    }




	CloseWindow();
	return 0;
}
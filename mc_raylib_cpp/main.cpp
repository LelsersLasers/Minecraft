#include "raylib.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <string>

#include "include/consts.h"

#include "include/Vector3Util.h"
// #include "include/Vector2Util.h"

#include "include/cameraController.h"
#include "include/block.h"
#include "include/blockType.h"

using std::vector;


int main() {

	bool cursor = false;
	bool wireframe = false;
	bool faces = false;

	srand(time(NULL));

    InitWindow(WINDOW_WIDTH_START, WINDOW_HEIGHT_START, "Minecraft ig");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

	DisableCursor();

	CameraController cameraController; // uses default defined constructor


    // Block testBlock(getRandomBlockType());
	vector<Block> blocks;
	for (int x = 0; x < 10; x++) {
		blocks.push_back(Block(getRandomBlockType()));
	}



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

		{
			if (IsKeyPressed(KEY_C)) {
				cursor = !cursor;
				if (cursor) {
					EnableCursor();
				} else {
					DisableCursor();
				}
			}
			if (IsKeyPressed(KEY_F)) {
				wireframe = !wireframe;
			}
			if (IsKeyPressed(KEY_G)) {
				faces = !faces;
			}
		}
		

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(cameraController.camera);
            {
				for (size_t x = 0; x < blocks.size(); x++) {
					DrawCube((Vector3){ (float)x, 0, 0 }, 1.0, 1.0, 1.0, blocks[x].getColor());
				}
            }
			EndMode3D();


			int windowWidth = GetScreenWidth();
			int windowHeight = GetScreenHeight();
			DrawLine(
				windowWidth / 2 - 10,
				windowHeight / 2,
				windowWidth / 2 + 10,
				windowHeight / 2,
				BLACK
			);
			DrawLine(
				windowWidth / 2,
				windowHeight / 2 - 10,
				windowWidth / 2,
				windowHeight / 2 + 10,
				BLACK
			);

			DrawRectangle(10, 10, 220, 130, SKYBLUE);
			DrawRectangleLines(10, 10, 220, 130, BLUE);
			DrawText("Info:", 20, 20, 10, BLACK);

			std::string fpsText = "- FPS: " + std::to_string((int)(1.0 / delta));
			std::string deltaText = "- Delta: " + std::to_string(delta);

			DrawText(fpsText.c_str(), 40, 40, 10, DARKGRAY);
			DrawText(deltaText.c_str(), 40, 60, 10, DARKGRAY);
        }
        EndDrawing();
    }




	CloseWindow();
	return 0;
}

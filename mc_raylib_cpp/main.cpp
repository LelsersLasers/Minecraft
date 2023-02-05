#include "raylib.h"

#include <time.h>
#include <stdlib.h>
// #include <iostream>
#include <climits>

#include <vector>
#include <tuple>
#include <string>
#include <optional>

#include "include/consts.h"

#include "include/Vector3Util.h"
// #include "include/Vector2Util.h"

#include "include/cameraController.h"
#include "include/block.h"
#include "include/blockType.h"
#include "include/chunk.h"
#include "include/world.h"
#include "include/raycastRequest.h"

using std::vector;
using std::tuple;
using std::optional;


int main() {

	bool cursor = false;
	bool wireframe = false;
	bool transparentWireframe = false;
	bool chunkOutlines = false;
	bool faces = true;

	srand(time(NULL));

    InitWindow(WINDOW_WIDTH_START, WINDOW_HEIGHT_START, "Minecraft ig");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

	DisableCursor();


	// index = enum of block type
	Block blockTypes[BLOCK_MAX + 1] = {
		AIR_BLOCK,
		GRASS_BLOCK,
		DIRT_BLOCK,
		STONE_BLOCK,
		BEDROCK_BLOCK,
		WATER_BLOCK,
		SAND_BLOCK
	};
	// parrallel array to blockTypes
	KeyboardKey blockKeys[BLOCK_MAX + 1] = {
		KEY_ONE,
		KEY_TWO,
		KEY_THREE,
		KEY_FOUR,
		KEY_FIVE,
		KEY_SIX,
		KEY_SEVEN
	};
	Block selectedBlock = blockTypes[BlockType::GRASS]; // uses enum as int to index array


	CameraController cameraController; // uses default defined constructor


	PerlinNoise pn(rand() % UINT_MAX);
	// PerlinNoise pn;

	World world; // uses default defined constructor
	world.generateChunks(pn);
	world.updateChunkModels();
	world.cameraMoved(cameraController);



    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();

		{
			if (IsKeyDown(KEY_UP)) {
				cameraController.pitch += delta * PI / 2.0;
			}
			if (IsKeyDown(KEY_DOWN)) {
				cameraController.pitch -= delta * PI / 2.0;
			}
			if (IsKeyDown(KEY_LEFT)) {
				cameraController.yaw += delta * PI / 2.0;
			}
			if (IsKeyDown(KEY_RIGHT)) {
				cameraController.yaw -= delta * PI / 2.0;
			}
		}
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

				world.cameraMoved(cameraController);
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
			if (IsKeyPressed(KEY_H)) {
				transparentWireframe = !transparentWireframe;
			}
			if (IsKeyPressed(KEY_J)) {
				chunkOutlines = !chunkOutlines;
			}
		}


		{
			for (size_t i = 0; i < BLOCK_MAX + 1; i++) {
				if (IsKeyPressed(blockKeys[i])) {
					selectedBlock = blockTypes[i];
				}
			}
		}

		// iterates over chunks
		world.updateChunkModels();


        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(cameraController.camera);
            {
				for (size_t idx = 0; idx < world.chunkOrder.size(); idx++) {

					tuple<int, int, int> i = world.chunkOrder[idx]; // draw back to front
					string key = TUP_TO_STR(i);

					Chunk& chunk = world.chunks.at(key);

					if (chunkOutlines) {
						DrawCubeWiresV(
							chunk.getWorldPos() + Vector3Uniform((float)CHUNK_SIZE / 2.0),
							Vector3Uniform((float)CHUNK_SIZE),
							PINK
						);
					}
					if (faces) {
						DrawModel(chunk.model, chunk.getWorldPos(), 1.0, WHITE);
						DrawModel(chunk.transparentModel, chunk.getWorldPos(), 1.0, WHITE);
					}
					if (wireframe) {
						DrawModelWires(chunk.model, chunk.getWorldPos(), 1.0, BLACK);
					}
					if (transparentWireframe) {
						DrawModelWires(chunk.transparentModel, chunk.getWorldPos(), 1.0, BLACK);
					}
				}

				RaycastRequest raycastRequest = RaycastRequest::NONE;
				if (IsKeyPressed(KEY_Q)) {
					raycastRequest = RaycastRequest::DESTROY_BLOCK;
				} else if (IsKeyPressed(KEY_E)) {
					raycastRequest = RaycastRequest::PLACE_BLOCK;
				}
				
				optional<Vector3> outlinedBlock = world.handleRaycastRequest(cameraController, raycastRequest, selectedBlock);
				if (outlinedBlock.has_value()) {
					DrawCubeWiresV(outlinedBlock.value(), Vector3Uniform(1.0), RED);
				}


            }
			EndMode3D();


			int windowWidth = GetScreenWidth();
			int windowHeight = GetScreenHeight();


			if (world.cameraIsSubmerged(cameraController) && faces) {
				DrawRectangle(0, 0, windowWidth, windowHeight, ColorAlpha(BLUE, 0.6));
			}


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
			std::string deltaText = "- Delta: " + std::to_string(delta * 1000);

			std::string selectedBlockText = "- Selected: " + getBlockName(selectedBlock.blockType);

			DrawText(fpsText.c_str(), 40, 40, 10, DARKGRAY);
			DrawText(deltaText.c_str(), 40, 60, 10, DARKGRAY);
			DrawText(selectedBlockText.c_str(), 40, 80, 10, DARKGRAY);
        }
        EndDrawing();
    }




	CloseWindow();
	return 0;
}

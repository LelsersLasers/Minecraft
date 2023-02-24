#include "raylib.h"
#include "raymath.h"

#include <time.h>
#include <stdlib.h>
// #include <iostream>
#include <climits>

#include <vector>
#include <tuple>
#include <string>
#include <optional>

#include "include/consts.h"

#include "include/cameraController.h"
#include "include/atlas.h"
#include "include/block.h"
#include "include/blockType.h"
#include "include/chunk.h"
#include "include/world.h"
#include "include/raycastRequest.h"
#include "include/ChunkModelInfo.h"

using std::vector;
using std::tuple;
using std::optional;


int main() {

	bool cursor = false;
	bool wireframe = false;
	bool chunkOutlines = false;
	bool faces = true;

	bool autoMove = false;

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
		SAND_BLOCK,
		LOG_BLOCK,
		LEAVES_BLOCK,
		SAND_STONE_BLOCK
	};
	// parrallel array to blockTypes
	KeyboardKey blockKeys[BLOCK_MAX + 1] = {
		KEY_ONE,
		KEY_TWO,
		KEY_THREE,
		KEY_FOUR,
		KEY_FIVE,
		KEY_SIX,
		KEY_SEVEN,
		KEY_EIGHT,
		KEY_NINE,
		KEY_ZERO
	};
	Block selectedBlock = blockTypes[BlockType::GRASS]; // uses enum as int to index array


	CameraController cameraController; // uses default defined constructor

	Atlas atlas("assets/terrain2.png"); // must be square png

	PerlinNoise pn(rand() % UINT_MAX);

	World world; // uses default defined constructor
	world.cameraMoved(cameraController, pn);
	world.updateChunkModels(atlas);



    while (!WindowShouldClose()) {
        const float delta = GetFrameTime();

		{
			if (IsKeyDown(KEY_UP)) {
				cameraController.pitch += delta * PI / 2.0f;
			}
			if (IsKeyDown(KEY_DOWN)) {
				cameraController.pitch -= delta * PI / 2.0f;
			}
			if (IsKeyDown(KEY_LEFT)) {
				cameraController.yaw += delta * PI / 2.0f;
			}
			if (IsKeyDown(KEY_RIGHT)) {
				cameraController.yaw -= delta * PI / 2.0f;
			}
		}
        cameraController.update();
		{
			Vector3 moveVec = Vector3Zero();

			if (IsKeyDown(KEY_W) || autoMove) {
				moveVec = Vector3Add(moveVec, cameraController.calcForward());
			}
			if (IsKeyDown(KEY_S)) {
				moveVec = Vector3Subtract(moveVec, cameraController.calcForward());
				autoMove = false;
			}
			if (IsKeyDown(KEY_A)) {
				moveVec = Vector3Subtract(moveVec, cameraController.calcRight());
				autoMove = false;
			}
			if (IsKeyDown(KEY_D)) {
				moveVec = Vector3Add(moveVec, cameraController.calcRight());
				autoMove = false;
			}

			if (!Vector3Equals(moveVec, Vector3Zero())) {
				moveVec = Vector3Scale(Vector3Normalize(moveVec), delta * 20.0f);

				tuple<int, int, int> oldCameraChunk = cameraController.getChunkPos();
				cameraController.moveBy(moveVec);
				tuple<int, int, int> newCameraChunk = cameraController.getChunkPos();

				if (oldCameraChunk != newCameraChunk) {
					world.cameraMoved(cameraController, pn);
				}
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
				chunkOutlines = !chunkOutlines;
			}
			if (IsKeyPressed(KEY_R)) {
				autoMove = !autoMove;
			}
		}


		{
			for (size_t i = 0; i < BLOCK_MAX + 1; i++) {
				if (IsKeyPressed(blockKeys[i])) {
					selectedBlock = blockTypes[i];
				}
			}
		}

		world.generateChunk(pn, atlas); // generate 1 or 0 chunks

		world.updateChunkModels(atlas);

		if (world.shouldSortChunksToRender) {
			world.sortKeysToRender();
		}


        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(cameraController.camera);
            {
				for (size_t i = 0; i < world.chunksToRender.size(); i++) {
					Chunk& chunk = world.chunksToRender[i];

					if (chunkOutlines) {
						DrawCubeWiresV(
							Vector3Add(chunk.getWorldPos(), Vector3Scale(Vector3One(), (float)CHUNK_SIZE / 2.0f)),
							Vector3Scale(Vector3One(), (float)CHUNK_SIZE),
							PINK
						);
					}
					for (size_t j = 0; j < TOTAL_CHUNK_MESHES; j++) {
						if (faces) {
							DrawModel(chunk.models[j], chunk.getWorldPos(), 1.0f, WHITE);
						}
						if (wireframe) {
							DrawModelWires(chunk.models[j], chunk.getWorldPos(), 1.0f, BLACK);
						}
					}
				}

				RaycastRequest raycastRequest = RaycastRequest::NONE;
				if (IsKeyPressed(KEY_Q) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					raycastRequest = RaycastRequest::DESTROY_BLOCK;
				} else if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
					raycastRequest = RaycastRequest::PLACE_BLOCK;
				}

				optional<Vector3> possibleOutlinedBlock = world.handleRaycastRequest(cameraController, raycastRequest, selectedBlock);
				if (possibleOutlinedBlock.has_value()) {
					Vector3 outlinedBlock = possibleOutlinedBlock.value();
					DrawCubeWiresV(outlinedBlock, Vector3One(), RED);
				}


            }
			EndMode3D();


			int windowWidth = GetScreenWidth();
			int windowHeight = GetScreenHeight();


			if (world.cameraIsSubmerged(cameraController) && faces) {
				DrawRectangle(0, 0, windowWidth, windowHeight, ColorAlpha(BLUE, 0.6f));
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

			DrawRectangle(10, 10, 220, 170, SKYBLUE);
			DrawRectangleLines(10, 10, 220, 170, BLUE);
			DrawText("Info:", 20, 20, 10, BLACK);

			std::string selectedBlockText = "- Selected block: " + getBlockName(selectedBlock.blockType);
			std::string fpsText = "- FPS: " + std::to_string((int)(1.0 / delta));
			std::string deltaText = "- Delta: " + std::to_string(delta * 1000.0f) + " ms";
			std::string cameraPosText = "- Camera: " + std::to_string((int)cameraController.camera.position.x) + ", " + std::to_string((int)cameraController.camera.position.y) + ", " + std::to_string((int)cameraController.camera.position.z);
			std::string chunksToGenerateText = "- Chunks to generate: " + std::to_string(world.chunksToGenerate.size());
			std::string chunksToRenderText = "- Chunks to render: " + std::to_string(world.chunksToRender.size());
			std::string totalChunksText = "- Total chunks: " + std::to_string(world.chunks.size());

			DrawText(selectedBlockText.c_str(),		40, 40,  10, DARKGRAY);
			DrawText(fpsText.c_str(), 				40, 60,  10, DARKGRAY);
			DrawText(deltaText.c_str(),				40, 80,  10, DARKGRAY);
			DrawText(cameraPosText.c_str(),			40, 100, 10, DARKGRAY);
			DrawText(chunksToGenerateText.c_str(),	40, 120, 10, DARKGRAY);
			DrawText(chunksToRenderText.c_str(),	40, 140, 10, DARKGRAY);
			DrawText(totalChunksText.c_str(),		40, 160, 10, DARKGRAY);
        }
        EndDrawing();
    }




	CloseWindow();
	return 0;
}

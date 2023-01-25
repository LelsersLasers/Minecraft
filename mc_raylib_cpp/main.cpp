#include "raylib.h"

#include "raymath.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <tuple>
#include <string>

#include "include/consts.h"

#include "include/Vector3Util.h"
// #include "include/Vector2Util.h"

#include "include/cameraController.h"
#include "include/block.h"
#include "include/blockType.h"
#include "include/chunk.h"
#include "include/world.h"

using std::vector;
using std::tuple;


int main() {

	bool cursor = false;
	bool wireframe = false;
	bool transparentWireframe = false;
	bool faces = false;

	srand(time(NULL));

    InitWindow(WINDOW_WIDTH_START, WINDOW_HEIGHT_START, "Minecraft ig");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

	DisableCursor();

	CameraController cameraController; // uses default defined constructor


	World world; // uses default defined constructor
	world.generateChunks();
	world.updateChunkModels();



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
		}
		

		// iterates over chunks
		world.updateChunkModels();


        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(cameraController.camera);
            {

				vector<RayCollision> rayCollisions;
				vector<Chunk*> chunkCollisions;

				for (size_t i = 0; i < world.chunks.size(); i++) {

					if (faces) {
						DrawModel(world.chunks[i].model, world.chunks[i].getWorldPos(), 1.0, WHITE);
						DrawModel(world.chunks[i].transparentModel, world.chunks[i].getWorldPos(), 1.0, WHITE);
					}
					if (wireframe) {
						DrawModelWires(world.chunks[i].model, world.chunks[i].getWorldPos(), 1.0, BLACK);
						
						DrawCubeWiresV(
							world.chunks[i].getWorldPos() + Vector3Uniform((float)CHUNK_SIZE / 2.0),
							Vector3Uniform((float)CHUNK_SIZE),
							PINK
						);
					}
					if (transparentWireframe) {
						DrawModelWires(world.chunks[i].transparentModel, world.chunks[i].getWorldPos(), 1.0, BLACK);
					}


					{
						Ray qRay = {
							cameraController.camera.position,
							cameraController.calcForward()
						};
						Vector3 pos = world.chunks[i].getWorldPos();
						Matrix qTransform = MatrixTranslate(pos.x, pos.y, pos.z);
						RayCollision qRayCollision = GetRayCollisionMesh(qRay, world.chunks[i].model.meshes[0], qTransform);
						if (qRayCollision.hit && qRayCollision.distance < REACH) {
							rayCollisions.push_back(qRayCollision);
							chunkCollisions.push_back(&world.chunks[i]);
						}
					}


				}

				if (rayCollisions.size() > 0) {
					RayCollision closestRayCollision = rayCollisions[0];
					Chunk* closestChunkCollision = chunkCollisions[0];

					for (size_t i = 1; i < rayCollisions.size(); i++) {
						if (rayCollisions[i].distance < closestRayCollision.distance) {
							closestRayCollision = rayCollisions[i];
							closestChunkCollision = chunkCollisions[i];
						}
					}

					tuple<size_t, size_t, size_t> bestBlockTuple = closestChunkCollision->handleRayCollision(closestRayCollision);
					Vector3 bestBlockOutlinePos = (Vector3){
						(float)std::get<0>(bestBlockTuple),
						(float)std::get<1>(bestBlockTuple),
						(float)std::get<2>(bestBlockTuple)
					} + Vector3Uniform(0.5);
					DrawCubeWiresV(bestBlockOutlinePos + closestChunkCollision->getWorldPos(), Vector3Uniform(1.0), RED);

					if (IsKeyPressed(KEY_Q)) {
						closestChunkCollision->destroyBlockAt(bestBlockTuple, world);
					}
					if (IsKeyPressed(KEY_E)) {
						closestChunkCollision->placeBlockAt(bestBlockTuple, closestRayCollision.normal, STONE_BLOCK, world);
					}
				}


            }
			EndMode3D();


			int windowWidth = GetScreenWidth();
			int windowHeight = GetScreenHeight();


			if (world.cameraIsSubmerged(cameraController)) {
				DrawRectangle(0, 0, windowWidth, windowHeight, ColorAlpha(BLUE, 0.4));
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

			DrawText(fpsText.c_str(), 40, 40, 10, DARKGRAY);
			DrawText(deltaText.c_str(), 40, 60, 10, DARKGRAY);
        }
        EndDrawing();
    }




	CloseWindow();
	return 0;
}

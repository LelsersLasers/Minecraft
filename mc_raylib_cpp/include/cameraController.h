#ifndef CAMERA_CONTROLLER_GUARD
#define CAMERA_CONTROLLER_GUARD


#include "raylib.h"


class CameraController {

	public:
		Camera3D camera;

		Vector2 mousePosition;
		float mouseSensitivity;

		float yaw;
		float pitch;


		CameraController();

		void moveBy(const Vector3 &vec);

		Vector3 calcForward() const;
		Vector3 calcRight() const;

		void update();
};


#endif
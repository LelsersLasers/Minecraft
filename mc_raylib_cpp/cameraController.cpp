
#include "raylib.h"

#include <cmath>

#include "include/consts.h"

#include "include/cameraController.h"


CameraController::CameraController() {

	this->camera = { 0 };
	this->camera.position =	(Vector3){ 0.0, 0.0, 0.0 };
	this->camera.target =	(Vector3){ 1.0, 0.0, 0.0 };
	this->camera.up = 		(Vector3){ 0.0, 0.0, 1.0 };
	this->camera.fovy =		60.0;
	this->camera.projection = CAMERA_PERSPECTIVE;

	this->mousePosition = (Vector2){
		WINDOW_WIDTH_START / 2.0,
		WINDOW_HEIGHT_START / 2.0,
	};
	this->mouseSensitivity = MOUSE_SENSITIVITY;

	this->yaw = 0.0;
	this->pitch = 0.0;
}

void CameraController::moveBy(const Vector3 &vec) {
	this->camera.position = (Vector3){
		this->camera.position.x + vec.x,
		this->camera.position.y + vec.y,
		this->camera.position.z + vec.z,
	};
	this->camera.target = (Vector3){
		this->camera.target.x + vec.x,
		this->camera.target.y + vec.y,
		this->camera.target.z + vec.z,
	};
}

Vector3 CameraController::calcForward() const {
	float yawSin = sin(this->yaw);
	float yawCos = cos(this->yaw);

	float pitchSin = sin(this->pitch);
	float pitchCos = cos(this->pitch);

	return (Vector3) {
		yawCos * pitchCos,
		yawSin * pitchCos,
		pitchSin,
	};
}
Vector3 CameraController::calcRight() const {
	float rightYaw = this->yaw - PI / 2.0;
	float yawSin = sin(rightYaw);
	float yawCos = cos(rightYaw);

	float pitchSin = sin(this->pitch);
	float pitchCos = cos(this->pitch);

	return (Vector3) {
		yawCos * pitchCos,
		yawSin * pitchCos,
		pitchSin,
	};
}

void CameraController::update() {
	Vector2 newMousePosition = GetMousePosition();
	Vector2 mouseDelta = (Vector2){
		this->mousePosition.x - newMousePosition.x,
		this->mousePosition.y - newMousePosition.y,
	};
	this->mousePosition = newMousePosition;

	this->yaw += mouseDelta.x * this->mouseSensitivity;
	this->pitch += mouseDelta.y * this->mouseSensitivity;

	if (this->pitch >= PI / 2.0) {
		this->pitch = PI / 2.0 - 0.01;
	}
	else if (this->pitch <= -PI / 2.0) {
		this->pitch = -PI / 2.0 + 0.01;
	}

	Vector3 forwardVec = this->calcForward();
	this->camera.target = (Vector3){
		this->camera.position.x + forwardVec.x,
		this->camera.position.y + forwardVec.y,
	};
}
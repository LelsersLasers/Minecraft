use raylib::prelude::*;

use std::f32::consts::PI;

use crate::consts as mn;

pub struct CameraController {
    pub camera: Camera3D,

    pub mouse_position: Vector2,
    pub mouse_sensitivity: f32,

    pub yaw: f32,   // left/right
    pub pitch: f32, // up/down
}
impl CameraController {
    pub fn new(camera: Camera3D) -> Self {
        Self {
            camera,

            mouse_position: Vector2::new(
                mn::WINDOW_WIDTH_START as f32 / 2.0,
                mn::WINDOW_HEIGHT_START as f32 / 2.0,
            ),
            mouse_sensitivity: mn::MOUSE_SENSITIVITY,

            yaw: 0.0,
            pitch: 0.0,
        }
    }
    pub fn move_by(&mut self, vec: Vector3) {
        self.camera.position += vec;
        self.camera.target += vec;
    }
    pub fn calc_forward(&self) -> Vector3 {
        let (yaw_sin, yaw_cos) = self.yaw.sin_cos();
        let (pitch_sin, pitch_cos) = self.pitch.sin_cos();

        Vector3::new(yaw_cos * pitch_cos, yaw_sin * pitch_cos, pitch_sin).normalized()
    }
    pub fn calc_right(&self) -> Vector3 {
        // TODO: maybe just use x/y of yaw instead?
        let (yaw_sin, yaw_cos) = (self.yaw - std::f32::consts::PI / 2.0).sin_cos();
        let (pitch_sin, pitch_cos) = self.pitch.sin_cos();

        Vector3::new(yaw_cos * pitch_cos, yaw_sin * pitch_cos, pitch_sin).normalized()
    }
    pub fn update(&mut self, rl: &mut RaylibHandle) {
        let mouse_position = rl.get_mouse_position();

        let mouse_delta = self.mouse_position - mouse_position;
        self.mouse_position = mouse_position;

        self.yaw += mouse_delta.x * self.mouse_sensitivity;
        self.pitch += mouse_delta.y * self.mouse_sensitivity;

        if self.pitch >= PI / 2.0 {
            self.pitch = PI / 2.0 - 0.001;
        }
        if self.pitch <= -PI / 2.0 {
            self.pitch = -PI / 2.0 + 0.001;
        }

        self.camera.target = self.camera.position + self.calc_forward();
    }
}

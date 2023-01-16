use raylib::prelude::*;

pub const WINDOW_WIDTH_START: i32 = 1280;
pub const WINDOW_HEIGHT_START: i32 = 720;

pub const MOUSE_SENSITIVITY: f32 = 1.0 / 400.0;

pub const CHUNK_SIZE: usize = 4; // blocks per chunk ^3

pub const WORLD_SIZE: usize = 2; // chunks per world ^3

pub const CUBE_VERTICES: [Vector3; 8] = [
    Vector3::new(0.0, 0.0, 0.0), // 0
    Vector3::new(1.0, 0.0, 0.0), // 1
    Vector3::new(1.0, 1.0, 0.0), // 2
    Vector3::new(0.0, 1.0, 0.0), // 3
    Vector3::new(0.0, 0.0, 1.0), // 4
    Vector3::new(1.0, 0.0, 1.0), // 5
    Vector3::new(1.0, 1.0, 1.0), // 6
    Vector3::new(0.0, 1.0, 1.0), // 7
];

use raylib::prelude::*;

#[derive(Clone, Copy)]
pub struct Triangle {
    pub vertices: [Vector3; 3],
    pub color: Color,
}
impl Triangle {
    pub fn new(v1: Vector3, v2: Vector3, v3: Vector3, color: Color) -> Self {
        Self {
            vertices: [v1, v2, v3],
            color,
        }
    }
}

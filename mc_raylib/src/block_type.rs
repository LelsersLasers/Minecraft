use raylib::prelude::*;

#[derive(Clone, Copy)]
pub enum BlockType {
    Air,
    Grass,
    Dirt,
    Stone,
    Bedrock,
}
impl BlockType {
    pub fn get_color_fn(&self) -> fn() -> Color {
        match self {
            BlockType::Air => || Color::BLANK,
            BlockType::Grass => || Color::GREEN,
            BlockType::Dirt => || Color::BROWN,
            BlockType::Stone => || Color::DARKGRAY,
            BlockType::Bedrock => || Color::BLACK,
        }
    }
    pub fn get_transparent_fn(&self) -> fn() -> bool {
        match self {
            BlockType::Air => || true,
            BlockType::Grass => || false,
            BlockType::Dirt => || false,
            BlockType::Stone => || false,
            BlockType::Bedrock => || false,
        }
    }
}

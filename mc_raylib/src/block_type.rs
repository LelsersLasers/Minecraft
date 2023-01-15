use rand::distributions::{Distribution, Standard};
use rand::prelude::*;

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
    pub fn get_random_block_type() -> Self {
        let mut rng = rand::thread_rng();
        rng.gen()
    }
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
impl Distribution<BlockType> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> BlockType {
        match rng.gen_range(0..4) {
            0 => BlockType::Air,
            1 => BlockType::Grass,
            2 => BlockType::Dirt,
            3 => BlockType::Stone,
            4 => BlockType::Bedrock,
            _ => BlockType::Air, // idk why this is needed
        }
    }
}

use rand::distributions::{Distribution, Standard};
use rand::prelude::*;

use raylib::prelude::*;

use crate::dir::Dir;

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
    pub fn get_color_fn(&self) -> fn(&Dir) -> Color {
        match self {
            BlockType::Air => |_dir| Color::BLANK,
            BlockType::Grass => |_dir| {
                Color::GREEN
                // match dir {
                //     Dir::Top => Color::GREEN,
                //     Dir::Bottom => Color::BROWN, //
                //     Dir::Right => Color::PINK,
                //     Dir::Left => Color::PURPLE, //
                //     Dir::Forward => Color::BLUE, //
                //     Dir::Backward => Color::RED,
                // }
            },
            BlockType::Dirt => |_dir| Color::BROWN,
            BlockType::Stone => |_dir| Color::GRAY,
            BlockType::Bedrock => |_dir| Color::BLACK,
        }
    }
    pub fn get_transparent(&self) -> bool {
        match self {
            BlockType::Air => true,
            BlockType::Grass => false,
            BlockType::Dirt => false,
            BlockType::Stone => false,
            BlockType::Bedrock => false,
        }
    }
}
impl Distribution<BlockType> for Standard {
    fn sample<R: Rng + ?Sized>(&self, rng: &mut R) -> BlockType {
        match rng.gen_range(0..5) {
            0 => BlockType::Air,
            1 => BlockType::Grass,
            2 => BlockType::Dirt,
            3 => BlockType::Stone,
            4 => BlockType::Bedrock,
            _ => BlockType::Air, // idk why this is needed
                                 // _ => BlockType::Grass,
        }
    }
}

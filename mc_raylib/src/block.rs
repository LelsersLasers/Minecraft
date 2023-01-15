use raylib::prelude::*;

use crate::block_type::BlockType;

pub struct Block {
    pub block_type: BlockType,
    pub get_color: fn() -> Color,
    pub get_transparent: fn() -> bool,
}
impl Block {
    pub fn new(block_type: BlockType) -> Self {
        Self {
            block_type,
            get_color: block_type.get_color_fn(),
            get_transparent: block_type.get_transparent_fn(),
        }
    }
}
impl Default for Block {
    fn default() -> Self {
        let default_block_type = BlockType::Air;
        Self {
            block_type: default_block_type,
            get_color: default_block_type.get_color_fn(),
            get_transparent: default_block_type.get_transparent_fn(),
        }
    }
}

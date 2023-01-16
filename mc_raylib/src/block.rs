use raylib::prelude::*;

use crate::block_type::BlockType;
use crate::dir::Dir;

#[derive(Clone, Copy)]
pub struct Block {
    pub block_type: BlockType,
    pub get_color: fn(&Dir) -> Color,
    pub transparent: bool,
}
impl Block {
    pub fn new(block_type: BlockType) -> Self {
        Self {
            block_type,
            get_color: block_type.get_color_fn(),
            transparent: block_type.get_transparent(),
        }
    }
}
impl Default for Block {
    fn default() -> Self {
        let default_block_type = BlockType::Air;
        Self {
            block_type: default_block_type,
            get_color: default_block_type.get_color_fn(),
            transparent: default_block_type.get_transparent(),
        }
    }
}

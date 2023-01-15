// use rand::prelude::*;
use raylib::prelude::*;

use crate::block::Block;
use crate::block_type::BlockType;

use crate::consts as mn;

pub struct Chunk {
    pub blocks: Vec<Block>,
    pub position: Vector3, // offset from origin in chunks
}
impl Chunk {
    pub fn new(position: Vector3) -> Self {
        Self {
            blocks: Vec::with_capacity(mn::CHUNK_SIZE * mn::CHUNK_SIZE * mn::CHUNK_SIZE),
            position,
        }
    }
    pub fn get_block_at(&self, x: usize, y: usize, z: usize) -> &Block {
        &self.blocks[x * mn::CHUNK_SIZE * mn::CHUNK_SIZE + y + z * mn::CHUNK_SIZE]
    }
    pub fn generate(&mut self) {
        // let mut rng = rand::thread_rng();
        for x in 0..mn::CHUNK_SIZE {
            for y in 0..mn::CHUNK_SIZE {
                for z in 0..mn::CHUNK_SIZE {
                    let mut block_type = BlockType::Air;
                    // if x == 0 || x == mn::CHUNK_SIZE - 1 || z == 0 || z == mn::CHUNK_SIZE - 1 {
                    // } else if (1..=4).contains(&y) {
                    //     block_type = BlockType::Dirt;
                    // } else if y == 5 {
                    //     block_type = BlockType::Grass;
                    // }

                    if x != 0 && x != mn::CHUNK_SIZE - 1 && y!= 0 && y != mn::CHUNK_SIZE - 1 && z != 0 && z != mn::CHUNK_SIZE - 1 {
                        block_type = BlockType::get_random_block_type()
                    }
                    // let block_type = BlockType::get_random_block_type();
                    let block = Block::new(block_type);
                    self.blocks.push(block);
                }
            }
        }
    }
}

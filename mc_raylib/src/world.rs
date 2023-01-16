// use crate::block::Block;
use crate::chunk::Chunk;

use crate::consts as mn;

pub struct World {
    pub chunks: Vec<Chunk>,
}
impl World {
    pub fn new() -> Self {
        Self {
            chunks: Vec::with_capacity(mn::WORLD_SIZE * mn::WORLD_SIZE * mn::WORLD_SIZE),
        }
    }
    pub fn generate_chunks(&mut self) {
        for x in 0..mn::WORLD_SIZE {
            for y in 0..mn::WORLD_SIZE {
                for z in 0..mn::WORLD_SIZE {
                    let mut chunk = Chunk::new((x as i32, y as i32, z as i32));
                    chunk.generate_blocks();
                    self.chunks.push(chunk);
                }
            }
        }
    }
    pub fn update_chunk_triangles(&mut self) {
        for chunk in self.chunks.iter_mut() {
            if chunk.dirty {
                chunk.generate_triangles();
            }
        }
    }
    // pub fn get_chunk_at(&self, chunk_pos: (i32, i32, i32)) -> &Chunk {
    //     let x = chunk_pos.0 as usize;
    //     let y = chunk_pos.1 as usize;
    //     let z = chunk_pos.2 as usize;
    //     // self.blocks[x * mn::CHUNK_SIZE * mn::CHUNK_SIZE + y + z * mn::CHUNK_SIZE]
    //     &self.chunks[x * mn::WORLD_SIZE * mn::WORLD_SIZE + y + z * mn::WORLD_SIZE]
    // }
    // pub fn get_block_at(&self, chunk_pos: (i32, i32, i32), x: i32, y: i32, z: i32) -> Block {
    //     let chunk = self.get_chunk_at(chunk_pos);
    //     chunk.get_block_at(x as usize, y as usize, z as usize)
    // }
}

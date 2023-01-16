use crate::block::Block;
use crate::block_type::BlockType;
use crate::chunk::Chunk;
use crate::triangle::Triangle;

use crate::consts as mn;

pub struct World {
    // parrallel arrays
    pub chunks: Vec<Chunk>,
    pub triangles: Vec<Vec<Triangle>>,
}
impl World {
    pub fn new() -> Self {
        Self {
            chunks: Vec::with_capacity(mn::WORLD_SIZE * mn::WORLD_SIZE * mn::WORLD_SIZE),
            triangles: Vec::new(),
        }
    }
    pub fn generate_chunks(&mut self) {
        self.chunks.clear();
        for x in 0..mn::WORLD_SIZE {
            for y in 0..mn::WORLD_SIZE {
                for z in 0..mn::WORLD_SIZE {
                    let mut chunk = Chunk::new((x as i32, y as i32, z as i32));
                    chunk.generate_blocks();
                    self.chunks.push(chunk);
                }
            }
        }
        self.triangles = self
            .chunks
            .iter()
            .map(|c| c.generate_triangles(&self))
            .collect();
    }
    pub fn update_chunk_triangles(&mut self) {
        for i in 0..self.chunks.len() {
            if self.chunks[i].dirty {
                self.triangles[i] = self.chunks[i].generate_triangles(&self);
            }
        }
    }
    pub fn get_chunk_at(&self, chunk_pos: (i32, i32, i32)) -> &Chunk {
        let x = chunk_pos.0 as usize;
        let y = chunk_pos.1 as usize;
        let z = chunk_pos.2 as usize;
        &self.chunks[x * mn::WORLD_SIZE * mn::WORLD_SIZE + y + z * mn::WORLD_SIZE]
    }
    pub fn get_block_at(&self, chunk_pos: (i32, i32, i32), x: i32, y: i32, z: i32) -> Block {
        if World::in_bounds(chunk_pos) {
            let chunk = self.get_chunk_at(chunk_pos);
            chunk.get_block_at(x as usize, y as usize, z as usize)
        } else {
            Block::new(BlockType::Air)
        }
    }
    pub fn in_bounds(idx: (i32, i32, i32)) -> bool {
        (0..mn::WORLD_SIZE as i32).contains(&idx.0)
            && (0..mn::WORLD_SIZE as i32).contains(&idx.1)
            && (0..mn::WORLD_SIZE as i32).contains(&idx.2)
    }
}

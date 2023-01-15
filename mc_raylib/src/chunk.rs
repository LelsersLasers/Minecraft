// use rand::prelude::*;
use raylib::prelude::*;

use crate::block::Block;
use crate::block_type::BlockType;
use crate::dir::Dir;
use crate::triangle::Triangle;

use crate::consts as mn;

pub struct Chunk {
    pub blocks: Vec<Block>,
    pub position: Vector3, // offset from origin in chunks

    pub triangles: Vec<Triangle>,
    pub dirty: bool, // if true, the chunk needs to be re-rendered
}
impl Chunk {
    pub fn new(position: Vector3) -> Self {
        Self {
            blocks: Vec::with_capacity(mn::CHUNK_SIZE * mn::CHUNK_SIZE * mn::CHUNK_SIZE),
            position,
            triangles: Vec::new(),
            dirty: true,
        }
    }
    pub fn get_block_at(&self, x: usize, y: usize, z: usize) -> Block {
        self.blocks[x * mn::CHUNK_SIZE * mn::CHUNK_SIZE + y + z * mn::CHUNK_SIZE]
    }
    pub fn generate_blocks(&mut self) {
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

                    if x != 0
                        && x != mn::CHUNK_SIZE - 1
                        && y != 0
                        && y != mn::CHUNK_SIZE - 1
                        && z != 0
                        && z != mn::CHUNK_SIZE - 1
                    {
                        block_type = BlockType::get_random_block_type()
                    }
                    // let block_type = BlockType::get_random_block_type();
                    let block = Block::new(block_type);
                    self.blocks.push(block);
                }
            }
        }
        self.dirty = true;
    }
    pub fn generate_triangles(&mut self) {
        self.triangles.clear();

        for x in 0..mn::CHUNK_SIZE {
            for y in 0..mn::CHUNK_SIZE {
                for z in 0..mn::CHUNK_SIZE {
                    let pos = self.position + Vector3::new(x as f32, y as f32, z as f32);

                    let block = self.get_block_at(x, y, z);
                    if block.transparent {
                        continue;
                    }

                    for ((dir_x, dir_y, dir_z), triangle_offset) in
                        Dir::tuples().iter().zip(Dir::triangle_offsets())
                    {
                        let neighbor_idx = (x as i32 + dir_x, y as i32 + dir_y, z as i32 + dir_z);

                        // not in chunk -> skip (TODO: check in neighbor chunks)
                        if neighbor_idx.0 < 0
                            || neighbor_idx.0 >= mn::CHUNK_SIZE as i32
                            || neighbor_idx.1 < 0
                            || neighbor_idx.1 >= mn::CHUNK_SIZE as i32
                            || neighbor_idx.2 < 0
                            || neighbor_idx.2 >= mn::CHUNK_SIZE as i32
                        {
                            continue;
                        }

                        let neighbor = self.get_block_at(
                            neighbor_idx.0 as usize,
                            neighbor_idx.1 as usize,
                            neighbor_idx.2 as usize,
                        );

                        let neighbor_transparent = neighbor.transparent;

                        if !neighbor_transparent {
                            continue;
                        }

                        for to in triangle_offset {
                            self.triangles.push(Triangle::new(
                                pos + mn::CUBE_VERTICES[to[0]],
                                pos + mn::CUBE_VERTICES[to[1]],
                                pos + mn::CUBE_VERTICES[to[2]],
                                (block.get_color)(),
                            ));
                        }
                    }
                }
            }
        }
        self.dirty = false;
    }
}

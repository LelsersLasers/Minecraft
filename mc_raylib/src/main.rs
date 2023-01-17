// use rand::prelude::*;
use raylib::prelude::*;

mod dir;
// use crate::dir::Dir;

mod block_type;
// use crate::block_type::BlockType;

mod block;
// use crate::block::Block;

mod triangle;
// use crate::triangle::Triangle;

mod chunk;
// use crate::chunk::Chunk;

mod world;
use crate::world::World;

mod camera_controller;
use crate::camera_controller::CameraController;

mod consts;
use crate::consts as mn; // mn = magic numbers

use std::f32::consts::PI;

// struct Player {
//     position: Vector3,
// }
// impl Player {
//     fn new(position: Vector3) -> Self {
//         Self { position }
//     }
//     fn update(&mut self) {}
// }

fn main() {
    let (mut rl, thread) = raylib::init()
        .size(mn::WINDOW_WIDTH_START, mn::WINDOW_HEIGHT_START)
        .title("Minecraft ig")
        .build();

    rl.set_window_state(WindowState::default().set_window_resizable(true));

    let mut cursor_enabled = false;
    rl.disable_cursor();

    let mut wire_frame = false;
    let mut faces = false;

    let mut camera_controller = CameraController::new(Camera3D::perspective(
        Vector3::new(0.0, 0.0, 0.0), // Camera position
        Vector3::new(1.0, 0.0, 0.0), // Camera looking at point
        Vector3::new(0.0, 0.0, 1.0), // Z is vertical axis
        60.0,                        // fov in degrees
    ));

    let mut world = World::new();
    world.generate_chunks();
    world.update_chunk_triangles();

    let mut last_time = instant::now();
    let mut delta_time = 1.0 / 60.0;

    while !rl.window_should_close() {
        camera_controller.update(&mut rl);

        {
            let mut move_vec = Vector3::zero();

            if rl.is_key_down(KeyboardKey::KEY_W) {
                move_vec += camera_controller.calc_forward();
            }
            if rl.is_key_down(KeyboardKey::KEY_S) {
                move_vec -= camera_controller.calc_forward();
            }
            if rl.is_key_down(KeyboardKey::KEY_D) {
                move_vec += camera_controller.calc_right();
            }
            if rl.is_key_down(KeyboardKey::KEY_A) {
                move_vec -= camera_controller.calc_right();
            }

            if move_vec != Vector3::zero() {
                camera_controller.move_by(move_vec.normalized() * delta_time * 20.0);
            }
        }

        {
            if rl.is_key_down(KeyboardKey::KEY_UP) {
                camera_controller.pitch += delta_time * PI / 2.0;
            }
            if rl.is_key_down(KeyboardKey::KEY_DOWN) {
                camera_controller.pitch -= delta_time * PI / 2.0;
            }
            if rl.is_key_down(KeyboardKey::KEY_RIGHT) {
                camera_controller.yaw -= delta_time * PI / 2.0;
            }
            if rl.is_key_down(KeyboardKey::KEY_LEFT) {
                camera_controller.yaw += delta_time * PI / 2.0;
            }
        }

        if rl.is_key_pressed(KeyboardKey::KEY_SPACE) {
            cursor_enabled = !cursor_enabled;
            if cursor_enabled {
                rl.enable_cursor();
            } else {
                rl.disable_cursor();
            }
        }
        if rl.is_key_pressed(KeyboardKey::KEY_F) {
            wire_frame = !wire_frame;
        }
        if rl.is_key_pressed(KeyboardKey::KEY_G) {
            faces = !faces;
        }
        if rl.is_key_pressed(KeyboardKey::KEY_R) {
            world.generate_chunks();
            world.update_chunk_triangles();
        }

        // must do immutable borrow of rl before mutable borrow (when creating rdh)
        let window_width = rl.get_screen_width();
        let window_height = rl.get_screen_height();

        let mut rdh = rl.begin_drawing(&thread);

        rdh.clear_background(Color::RAYWHITE);

        {
            let mut rm3 = rdh.begin_mode3D(camera_controller.camera);

            let mut tri_count = 0;

            for chunk in &world.chunks {
                // chunk outline
                rm3.draw_cube_wires(
                    chunk.get_world_pos() + Vector3::one() * (mn::CHUNK_SIZE as f32 / 2.0),
                    mn::CHUNK_SIZE as f32,
                    mn::CHUNK_SIZE as f32,
                    mn::CHUNK_SIZE as f32,
                    Color::PINK,
                );

                // for x in 0..mn::CHUNK_SIZE {
                //     for y in 0..mn::CHUNK_SIZE {
                //         for z in 0..mn::CHUNK_SIZE {
                //             let pos = chunk.get_world_pos() + Vector3::new(x as f32, y as f32, z as f32) + Vector3::one() * 0.5;
                //             let block = chunk.get_block_at(x, y, z);
                //             if !block.transparent {
                //                 rm3.draw_cube(pos, 1.0, 1.0, 1.0, (block.get_color)());
                //                 // rm3.draw_cube_wires(pos, 1.0, 1.0, 1.0, Color::PURPLE);
                //             }
                //         }
                //     }
                // }

                tri_count += chunk.triangles.len();

                if faces {
                    for triangle in &chunk.triangles {
                        rm3.draw_triangle3D(
                            triangle.vertices[0],
                            triangle.vertices[1],
                            triangle.vertices[2],
                            triangle.color,
                        );
                    }
                }
                if wire_frame {
                    // note: doesn't show backface culling
                    for triangle in &chunk.triangles {
                        for i in 0..3 {
                            rm3.draw_line_3D(
                                triangle.vertices[i],
                                triangle.vertices[(i + 1) % 3],
                                Color::BLACK,
                            );
                        }
                    }
                }
            }
            println!("triangles: {}", tri_count);
        }

        // crosshair
        rdh.draw_line(
            window_width as i32 / 2 - 10,
            window_height as i32 / 2,
            window_width as i32 / 2 + 10,
            window_height as i32 / 2,
            Color::BLACK,
        );
        rdh.draw_line(
            window_width as i32 / 2,
            window_height as i32 / 2 - 10,
            window_width as i32 / 2,
            window_height as i32 / 2 + 10,
            Color::BLACK,
        );

        let current_time = instant::now();
        delta_time = (current_time - last_time) as f32 / 1000.0;
        last_time = current_time;

        rdh.draw_rectangle(10, 10, 220, 130, Color::SKYBLUE);
        rdh.draw_rectangle_lines(10, 10, 220, 130, Color::BLUE);
        rdh.draw_text("Info:", 20, 20, 10, Color::BLACK);

        let yaw_text = format!("- yaw: {}", camera_controller.yaw);
        let pitch_text = format!("- pitch: {}", camera_controller.pitch);
        let forward_text = format!("- forward: {:?}", camera_controller.calc_forward());

        let fps_text = format!("- FPS: {:.0}", 1.0 / delta_time);
        let delta_text = format!("- Delta: {:.3} ms", delta_time * 1000.0);

        rdh.draw_text(&yaw_text, 40, 40, 10, Color::DARKGRAY);
        rdh.draw_text(&pitch_text, 40, 60, 10, Color::DARKGRAY);

        rdh.draw_text(&fps_text, 40, 80, 10, Color::DARKGRAY);
        rdh.draw_text(&delta_text, 40, 100, 10, Color::DARKGRAY);

        rdh.draw_text(&forward_text, 40, 120, 10, Color::DARKGRAY);
    }
}

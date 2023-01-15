use rand::prelude::*;
use raylib::prelude::*;

const WINDOW_WIDTH: i32 = 1280;
const WINDOW_HEIGHT: i32 = 720;

struct Block {
    position: Vector3,
    color: Color,
}
impl Block {
    fn new(position: Vector3, color: Color) -> Self {
        Self { position, color }
    }
}
// impl Default for Block {
//     fn default() -> Self {
//         Self {
//             position: Vector3::new(0.0, 0.0, 0.0),
//             color: Color::RED,
//         }
//     }
// }

// struct Player {
//     position: Vector3,
// }
// impl Player {
//     fn new(position: Vector3) -> Self {
//         Self { position }
//     }
//     fn update(&mut self) {}
// }

struct CameraController {
    camera: Camera3D,

    mouse_position: Vector2,
    mouse_sensitivity: f32,

    yaw: f32,       // left/right
    pitch: f32,     // up/down
}
impl CameraController {
    fn new(camera: Camera3D) -> Self {
        Self {
            camera,
            
            mouse_position: Vector2::new(WINDOW_WIDTH as f32 / 2.0, WINDOW_HEIGHT as f32 / 2.0),
            mouse_sensitivity: 1.0 / 400.0,

            yaw: 0.0,
            pitch: 0.0,
        }
    }
    fn move_by(&mut self, vec: Vector3) {
        self.camera.position += vec;
        self.camera.target += vec;
    }
    fn calc_forward(&self) -> Vector3 {
        let (yaw_sin, yaw_cos) = self.yaw.sin_cos();
        let (pitch_sin, pitch_cos) = self.pitch.sin_cos();

        Vector3::new(
            yaw_cos * pitch_cos,
            yaw_sin * pitch_cos,
            pitch_sin,
        ).normalized()
    }
    // fn calc_right(&self) -> Vector3 {
    //     todo!()
    // }
    fn update(&mut self, rl: &mut RaylibHandle) {
        let mouse_position = rl.get_mouse_position();

        let mouse_delta = self.mouse_position - mouse_position;
        self.mouse_position = mouse_position;

        self.yaw += mouse_delta.x * self.mouse_sensitivity;
        self.pitch += mouse_delta.y * self.mouse_sensitivity;

        self.camera.target = self.camera.position + self.calc_forward();
    }
}

fn main() {
    let (mut rl, thread) = raylib::init()
        .size(WINDOW_WIDTH, WINDOW_HEIGHT)
        .title("Minecraft ig")
        .build();

    let ws = WindowState::default().set_window_resizable(true);
    rl.set_window_state(ws);

    rl.set_target_fps(60);

    let mut cursor_enabled = false;
    rl.disable_cursor();

    let mut camera_controller = CameraController::new(Camera3D::perspective(
        Vector3::new(0.0, 0.0, 0.0),     // Camera position
        Vector3::new(1.0, 0.0, 0.0),        // Camera looking at point
        Vector3::new(0.0, 0.0, 1.0),        // Z is vertical axis
        60.0,                               // fov in degrees
    ));


    let mut blocks: Vec<Block> = Vec::new();
    let mut rng = rand::thread_rng();
    for x in -10..10 {
        for y in -10..10 {
            let block = Block::new(
                Vector3::new(x as f32, y as f32, rng.gen_range(0.0..10.0)),
                Color::RED,
            );
            blocks.push(block);
        }
    }

    while !rl.window_should_close() {
        // rl.update_camera(&mut camera);

        camera_controller.update(&mut rl);

        if rl.is_key_down(KeyboardKey::KEY_W) {
            let dir = camera_controller.calc_forward();
            camera_controller.move_by(dir * 0.1);
        }
        if rl.is_key_down(KeyboardKey::KEY_S) {
            let dir = camera_controller.calc_forward();
            camera_controller.move_by(-dir * 0.1);
        }

        if rl.is_key_pressed(KeyboardKey::KEY_SPACE) {
            cursor_enabled = !cursor_enabled;
            if cursor_enabled {
                rl.enable_cursor();
            } else {
                rl.disable_cursor();
            }
        }

        let mut rdh = rl.begin_drawing(&thread);

        rdh.clear_background(Color::RAYWHITE);

        {
            let mut rm3 = rdh.begin_mode3D(camera_controller.camera);
            rm3.draw_cube(
                Vector3::new(2.0, 0.0, 0.0),
                1.0,
                1.0,
                1.0,
                Color::RED,
            );

            for block in blocks.iter() {
                rm3.draw_cube(
                    block.position,
                    1.0,
                    1.0,
                    1.0,
                    block.color,
                );
                rm3.draw_cube_wires(
                    block.position,
                    1.0,
                    1.0,
                    1.0,
                    Color::BLACK,
                );
            }

            // d2.draw_plane(
            //     Vector3::new(0.0, 0.0, 0.0),
            //     Vector2::new(32.0, 32.0),
            //     Color::LIGHTGRAY,
            // );
            // d2.draw_cube(Vector3::new(-16.0, 2.5, 0.0), 1.0, 5.0, 32.0, Color::BLUE);
            // d2.draw_cube(Vector3::new(16.0, 2.5, 0.0), 1.0, 5.0, 32.0, Color::LIME);
            // d2.draw_cube(Vector3::new(0.0, 2.5, 16.0), 32.0, 5.0, 1.0, Color::GOLD);

            // for column in columns.iter() {
            //     d2.draw_cube(column.position, 2.0, column.height, 2.0, column.color);
            //     d2.draw_cube_wires(column.position, 2.0, column.height, 2.0, Color::MAROON);
            // }
        }

        // crosshair
        rdh.draw_line(
            WINDOW_WIDTH as i32 / 2 - 10,
            WINDOW_HEIGHT as i32 / 2,
            WINDOW_WIDTH as i32 / 2 + 10,
            WINDOW_HEIGHT as i32 / 2,
            Color::BLACK,
        );
        rdh.draw_line(
            WINDOW_WIDTH as i32 / 2,
            WINDOW_HEIGHT as i32 / 2 - 10,
            WINDOW_WIDTH as i32 / 2,
            WINDOW_HEIGHT as i32 / 2 + 10,
            Color::BLACK,
        );

        rdh.draw_rectangle(10, 10, 220, 70, Color::SKYBLUE);
        rdh.draw_rectangle_lines(10, 10, 220, 70, Color::BLUE);
        rdh.draw_text(
            "Camera info:",
            20,
            20,
            10,
            Color::BLACK,
        );

        let pitch_text = format!("- pitch: {}", camera_controller.pitch);
        let yaw_text = format!("- yaw: {}", camera_controller.yaw);

        rdh.draw_text(&pitch_text, 40, 40, 10, Color::DARKGRAY);
        rdh.draw_text(&yaw_text, 40, 60, 10, Color::DARKGRAY);
    }
}
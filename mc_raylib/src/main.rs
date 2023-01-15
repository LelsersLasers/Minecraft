use rand::prelude::*;
use raylib::prelude::*;

const WINDOW_WIDTH: i32 = 1280;
const WINDOW_HEIGHT: i32 = 720;

#[derive(Clone, Copy)]
enum BlockType {
    Grass,
    Dirt,
    Stone,
    Bedrock,
}
impl BlockType {
    fn get_color_fn(&self) -> fn() -> Color {
        match self {
            BlockType::Grass => || Color::GREEN,
            BlockType::Dirt => || Color::BROWN,
            BlockType::Stone => || Color::DARKGRAY,
            BlockType::Bedrock => || Color::BLACK,
        }
    }
}

struct Block {
    block_type: BlockType,
    get_color: fn() -> Color,
}
impl Block {
    fn new(block_type: BlockType) -> Self {
        Self {
            block_type,
            get_color: block_type.get_color_fn(),
        }
    }
}
impl Default for Block {
    fn default() -> Self {
        Self {
            block_type: BlockType::Grass,
            get_color: BlockType::Grass.get_color_fn()
        }
    }
}

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

    yaw: f32,   // left/right
    pitch: f32, // up/down
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

        Vector3::new(yaw_cos * pitch_cos, yaw_sin * pitch_cos, pitch_sin).normalized()
    }
    fn calc_right(&self) -> Vector3 {
        // TODO: maybe just use x/y of yaw instead?
        let (yaw_sin, yaw_cos) = (self.yaw - std::f32::consts::PI / 2.0).sin_cos();
        let (pitch_sin, pitch_cos) = self.pitch.sin_cos();

        Vector3::new(yaw_cos * pitch_cos, yaw_sin * pitch_cos, pitch_sin).normalized()
    }
    fn update(&mut self, rl: &mut RaylibHandle) {
        let mouse_position = rl.get_mouse_position();

        let mouse_delta = self.mouse_position - mouse_position;
        self.mouse_position = mouse_position;

        self.yaw += mouse_delta.x * self.mouse_sensitivity;
        self.pitch += mouse_delta.y * self.mouse_sensitivity;

        if self.pitch >= std::f32::consts::PI / 2.0 {
            self.pitch = std::f32::consts::PI / 2.0 - 0.001;
        }
        if self.pitch <= -std::f32::consts::PI / 2.0 {
            self.pitch = -std::f32::consts::PI / 2.0 + 0.001;
        }

        self.camera.target = self.camera.position + self.calc_forward();
    }
}

fn main() {
    let (mut rl, thread) = raylib::init()
        .size(WINDOW_WIDTH, WINDOW_HEIGHT)
        .title("Minecraft ig")
        .build();

    rl.set_window_state(WindowState::default().set_window_resizable(true));

    let mut cursor_enabled = false;
    rl.disable_cursor();

    let mut camera_controller = CameraController::new(Camera3D::perspective(
        Vector3::new(0.0, 0.0, 0.0), // Camera position
        Vector3::new(1.0, 0.0, 0.0), // Camera looking at point
        Vector3::new(0.0, 0.0, 1.0), // Z is vertical axis
        60.0,                        // fov in degrees
    ));

    let mut blocks: Vec<Block> = Vec::new();
    // let mut rng = rand::thread_rng();
    // for x in -10..10 {
    //     for y in -10..10 {
    //         let block = Block::new(BlockType::Dirt);
    //         blocks.push(block);
    //     }
    // }
    blocks.push(Block::new(BlockType::Grass));

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
                camera_controller.move_by(move_vec * delta_time * 10.0);
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

        let mut rdh = rl.begin_drawing(&thread);

        rdh.clear_background(Color::RAYWHITE);

        {
            let mut rm3 = rdh.begin_mode3D(camera_controller.camera);
            rm3.draw_cube(Vector3::new(2.0, 0.0, 0.0), 1.0, 1.0, 1.0, Color::RED);

            for block in blocks.iter() {
                rm3.draw_cube(
                    Vector3::zero(),
                    1.0,
                    1.0,
                    1.0,
                    (block.get_color)()
                );

                // rm3.draw_cube_wires(block.position, 1.0, 1.0, 1.0, Color::BLACK);
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

        rdh.draw_rectangle(10, 10, 220, 90, Color::SKYBLUE);
        rdh.draw_rectangle_lines(10, 10, 220, 90, Color::BLUE);
        rdh.draw_text("Info:", 20, 20, 10, Color::BLACK);

        let yaw_text = format!("- yaw: {}", camera_controller.yaw);
        let pitch_text = format!("- pitch: {}", camera_controller.pitch);

        rdh.draw_text(&yaw_text, 40, 40, 10, Color::DARKGRAY);
        rdh.draw_text(&pitch_text, 40, 60, 10, Color::DARKGRAY);

        let current_time = instant::now();
        delta_time = (current_time - last_time) as f32 / 1000.0;
        last_time = current_time;

        let fps_text = format!("- FPS: {:.0}", 1.0 / delta_time);
        rdh.draw_text(&fps_text, 40, 80, 10, Color::DARKGRAY);
    }
}

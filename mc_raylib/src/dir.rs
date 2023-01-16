pub enum Dir {
    Top,
    Bottom,
    Right,
    Left,
    Forward,
    Backward,
}
// note: all these functions return are parrallel arrays
impl Dir {
    pub fn dirs() -> [Dir; 6] {
        [
            Dir::Top,
            Dir::Bottom,
            Dir::Right,
            Dir::Left,
            Dir::Forward,
            Dir::Backward,
        ]
    }
    pub fn tuples() -> [(i32, i32, i32); 6] {
        [
            (0, 0, 1),
            (0, 0, -1),
            (0, 1, 0),
            (0, -1, 0),
            (1, 0, 0),
            (-1, 0, 0),
        ]
    }
    pub fn triangle_offsets() -> [[[usize; 3]; 2]; 6] {
        [
            [[4, 5, 7], [5, 6, 7]],
            [[0, 3, 1], [1, 3, 2]],
            [[3, 7, 2], [2, 7, 6]],
            [[0, 1, 4], [1, 5, 4]],
            [[1, 2, 5], [5, 2, 6]],
            [[0, 4, 3], [4, 7, 3]],
        ]
    }
}

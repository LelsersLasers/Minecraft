pub enum Dir {
    Top,
    Bottom,
    Right,
    Left,
    Forward,
    Backward,
}
impl Dir {
    pub fn get_tuple(&self) -> (i32, i32, i32) {
        match self {
            Dir::Top => (0, 0, 1),
            Dir::Bottom => (0, 0, -1),
            Dir::Right => (0, 1, 0),
            Dir::Left => (0, -1, 0),
            Dir::Forward => (1, 0, 0),
            Dir::Backward => (-1, 0, 0),
        }
    }
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
    // pub fn get_all_tuple_dirs() -> [(i32, i32, i32); 6] {
    //     [
    //         Dir::Top.get_tuple(),
    //         Dir::Bottom.get_tuple(),
    //         Dir::Right.get_tuple(),
    //         Dir::Left.get_tuple(),
    //         Dir::Forward.get_tuple(),
    //         Dir::Backward.get_tuple(),
    //     ]
    // }
}

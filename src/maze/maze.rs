use rand::{thread_rng, prelude::SliceRandom};

#[derive(Clone, Copy)]
struct Direction {
    bit: i32,
    dx: i32,
    dy: i32,
    opposite: *const Direction,
}

unsafe impl Sync for Direction {

}

static NORTH: Direction = Direction {
    bit: 1,
    dx: 0,
    dy: -1,
    opposite: &SOUTH,
};

static SOUTH: Direction = Direction {
    bit: 2,
    dx: 0,
    dy: 1,
    opposite: &NORTH,
};

static EAST: Direction = Direction {
    bit: 4,
    dx: 1,
    dy: 0,
    opposite: &WEST,
};

static WEST: Direction = Direction {
    bit: 8,
    dx: -1,
    dy: 0,
    opposite: &EAST,
};

const N_DIRECTIONS: usize = 4;

// Directions array
static DIRECTIONS: [Direction; N_DIRECTIONS] = [NORTH, SOUTH, EAST, WEST];

static mut X: i32 = 0;
static mut Y: i32 = 1;

pub fn create_maze(x: i32, y: i32) -> Vec<Vec<i32>> {
    let mut maze: Vec<Vec<i32>> = vec![vec![0; x as usize]; y as usize];
    generate_maze(&mut maze, 0, 0);
    return maze;
}

pub fn print_maze(maze: Vec<Vec<i32>>, x: i32, y: i32) {
    for i in 0..y {

        // Drawing northern edge
        for j in 0..x {
            print!("{}", if maze[j as usize][i as usize] & 1 == 0 { "+---" } else { "+   " });
        }
        println!();

        // Drawing western edge
        for j in 0..x {
            print!("{}", if maze[j as usize][i as usize] & 8 == 0 { "|   " } else { "   " });
        }
        println!();
    }

    // Drawing southern edge
    for _ in 0..x {
        print!("{}", "+---");
    }
    println!("{}", "+");
}

fn generate_maze(maze: &mut Vec<Vec<i32>>, cx: i32, cy: i32) {
    let dirs = rand_directions();
    
    for i in 0..dirs.len() {
        let nx = cx + dirs[i].dx;
        let ny = cy + dirs[i].dy;

        unsafe {
            if between(nx, X) && between(ny, Y) && maze[ny as usize][nx as usize] == 0 {
                maze[cx as usize][cy as usize] |= dirs[i].bit;
                maze[nx as usize][ny as usize] |= (*dirs[i].opposite).bit;
                generate_maze(maze, nx, ny);
            }
        }
    }
}

fn between(v: i32, upper: i32) -> bool {
    return v >= 0 && v < upper;
}

fn rand_directions() -> Vec<Direction> {
    let mut dirs = DIRECTIONS.to_vec();
    dirs.shuffle(&mut thread_rng());
    return dirs;
}

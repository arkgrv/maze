mod maze;

use std::io;
use maze::maze::{create_maze, print_maze};

fn read_number(message: String) -> i32 {
    println!("{}", message);
    let mut number = String::new();
    io::stdin().read_line(&mut number).expect("Failed to read line");
    number.trim().parse::<i32>().expect("Please type a number")
}

fn main() {

    let w = read_number("Please input width: ".to_string());
    let h = read_number("Please input heigth: ".to_string());

    let maze = create_maze(w, h);

    print_maze(maze, w, h);
}

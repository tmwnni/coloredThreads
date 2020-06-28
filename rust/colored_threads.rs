/*
    Colored Threads

    My first rust program, go easy on me please
*/
use colored::*;
use rand::Rng;
use std::{thread, time};

fn recursive_thing(number: u32, color: &str) {
    colored_message(color, format!("{} thread, number is: {}", color, number));
    let rand_miliseconds = rand::thread_rng().gen_range(1, 10);
    let sleep_time = time::Duration::from_millis(rand_miliseconds * 1000);
    thread::sleep(sleep_time);

    match number {
        0 => colored_message(color, format!("{} thread complete!", color)),
        _ => recursive_thing(number - 1, color),
    }
}

fn colored_message(color: &str, message: String) {
    println!("{}", message.color(color))
}

fn main() {
    let colors = ["Green", "Blue", "Yellow", "Red", "Cyan"];

    let handles: Vec<thread::JoinHandle<_>> = colors
        .iter()
        .map(|&color| {
            std::thread::spawn(move || {
                recursive_thing(10, color)
            })
        })
        .collect();

    for thread in handles {
        thread
            .join()
            .expect("Thread failed for some reason or something");
    }

    println!("All threads complete!");
}

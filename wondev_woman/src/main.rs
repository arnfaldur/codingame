#![allow(unused)]

extern crate rand;

use std::io;
use std::fmt;
use std::ops::*;
use rand::prelude::*;
use std::time::Duration;
use std::time::Instant;

macro_rules! parse_input {
    ($x:expr, $t:ident) => ($x.trim().parse::<$t>().unwrap())
}

#[derive(Copy, Clone, Debug)]
struct Point(i8, i8);

struct GameManager {
    game: Game,
    wins: i32,
    loss: i32,
}

#[derive(Copy, Clone, Debug)]
struct Game {
    board: Board,
    size: i32,
    unitspp: i32,
    p1score: i32,
    p2score: i32,
    p1done: bool,
    p2done: bool,
    player: bool,
}

#[derive(Copy, Clone)]
struct Board {
    field: [i8; BOARD_SIZE],
    friends: [Point; UNITS],
    foes: [Point; UNITS],
}

const UNITS: usize = 2;
const POSSIBILITIES: usize = 176;
const BOARD_SIZE: usize = 64;

#[derive(Copy, Clone)]
enum ActionType { MoveBuild, PushBuild }

#[derive(Copy, Clone)]
struct Action {
    atype: ActionType,
    index: usize,
    move_dir: Dir,
    build_dir: Dir,
}

#[derive(Copy, Clone)]
struct Stack<T: Copy> {
    elements: [T; POSSIBILITIES],
    head: usize,
}

impl<T> Stack<T> where T: Copy {
    fn push(&mut self, element: T) {
        if !self.is_full() {
            self.elements[self.head] = element;
            self.head += 1;
        }
    }
    fn pop(&mut self) -> Option<T> {
        if self.head == 0 {
            return None;
        } else {
            self.head -= 1;
            return Some(self.elements[self.head]);
        }
    }
    fn is_full(&self) -> bool {
        return self.head == POSSIBILITIES;
    }
    fn is_empty(&self) -> bool {
        return self.head == 0;
    }
    fn as_slice(&self) -> &[T] {
        return &self.elements[..self.head];
    }
    fn shuffle(&mut self) {
        let mut rng = thread_rng();
        rng.shuffle(&mut self.elements[..self.head]);
    }
}

impl<T> Iterator for Stack<T> where T: Copy {
    type Item = T;
    fn next(&mut self) -> Option<Self::Item> {
        if self.head == 0 {
            return None;
        } else {
            self.head -= 1;
            return Some(self.elements[self.head]);
        }
    }
    fn size_hint(&self) -> (usize, Option<usize>) {
        return (self.head, Some(self.head));
    }
}

impl<T> ExactSizeIterator for Stack<T> where T: Copy {
    fn len(&self) -> usize {
        return self.head;
    }
}

struct Moves {
    moves: [Dir; 8],
    head: usize,
}

impl Moves {
    fn new() -> Moves {
        return Moves { moves: [Dir::NW; 8], head: 0 };
    }
    fn push(&mut self, dir: Dir) {
        if !self.is_full() {
            self.moves[self.head] = dir;
            self.head += 1;
        }
    }
    fn pop(&mut self) -> Option<Dir> {
        if self.head == 0 {
            return None;
        } else {
            self.head -= 1;
            return Some(self.moves[self.head]);
        }
    }
    fn is_full(&self) -> bool {
        return self.head == 8;
    }
    fn is_empty(&self) -> bool {
        return self.head == 0;
    }
    fn as_slice(&self) -> &[Dir] {
        return &self.moves[..self.head];
    }
    fn shuffle(&mut self) {
        let mut rng = thread_rng();
        rng.shuffle(&mut self.moves[..self.head]);
    }
}

impl Iterator for Moves {
    type Item = Dir;
    fn next(&mut self) -> Option<Self::Item> {
        if self.head == 0 {
            return None;
        } else {
            self.head -= 1;
            return Some(self.moves[self.head]);
        }
    }
}

type Actions = Stack<Action>;

impl Actions {
    fn new() -> Actions {
        return Actions { elements: [Action::empty(); POSSIBILITIES], head: 0 };
    }
}

#[derive(Copy, Clone)]
enum Dir { NW, N, NE, E, SE, S, SW, W }

const ALL_DIRS: [Dir; 8] = [Dir::NW, Dir::N, Dir::NE, Dir::E, Dir::SE, Dir::S, Dir::SW, Dir::W];

impl Action {
    fn new(index: usize, move_dir: Dir, build_dir: Dir) -> Action {
        return Action {
            atype: ActionType::MoveBuild,
            index,
            move_dir,
            build_dir,
        };
    }
    fn empty() -> Action {
        return Action { atype: ActionType::MoveBuild, index: 9, move_dir: Dir::NW, build_dir: Dir::NW };
    }
}

impl Game {
    fn new() -> Game {
        return Game { board: Board::new(), size: 7, unitspp: 1, p1score: 0, p2score: 0, p1done: false, p2done: false, player: true };
    }

    fn reset(&mut self, board: Board) {
        self.board = board;
        self.p1done = false;
        self.p1score = 0;
        self.p2done = false;
        self.p2score = 0;
        self.player = true;
    }

    fn done(&self) -> bool {
        return self.p1done && self.p2done;
    }
    fn winner(&self) -> i8 {
        return if self.p1score > self.p2score { 1 } else { if self.p2score > self.p1score { 2 } else { 0 } };
    }

    fn act(&mut self, action: Action) {
        //print!("{}", self.board);
        if action.index == 9 {
            if self.player {
                self.p1done = true;
            } else {
                self.p2done = true;
            }
            return;
        }
        if self.player {
            self.board.friends[action.index] += action.move_dir.as_point();
            let here = self.board.friends[action.index] + action.build_dir.as_point();
            self.board[here] += 1;
            if self.board[self.board.friends[action.index]] == 3 { self.p1score += 1 }
            if !self.p2done { self.player = !self.player; }
//            for i in self.board.all_around(self.board.friends[action.index], true) {
//                eprint!("{}, ", i) ;
//            }
//            eprintln!("");
            if self.board.all_around(self.board.friends[0], true).is_empty() && self.board.all_around(self.board.friends[1], true).is_empty() {
                self.p1done = true;
            }
        } else {
            self.board.foes[action.index] += action.move_dir.as_point();
            let here = self.board.foes[action.index] + action.build_dir.as_point();
            self.board[here] += 1;
            if self.board[self.board.foes[action.index]] == 3 { self.p2score += 1 }
            if !self.p1done { self.player = !self.player; }
            if self.board.all_around(self.board.foes[0], true).is_empty() && self.board.all_around(self.board.foes[1], true).is_empty() {
                self.p2done = true;
            }
        }
    }

    //TODO: use
    fn unact(&mut self, action: Action) {
        let here = self.board.friends[action.index] + action.build_dir.as_point();
        self.board[here] -= 1;
        self.board.friends[action.index] -= action.move_dir.as_point();
    }

    fn get_random_move(&self) -> Option<Action> {
        let mut rng = thread_rng();
//        for (i, &boi) in (if self.player { self.board.friends } else { self.board.foes }).iter().enumerate() {
//            for dir in self.board.all_around(boi, true) {
//
//            }
//        }
        let actions = self.all_actions();
        return if actions.is_empty() { None } else { Some(*rng.choose(actions.as_slice()).unwrap()) };
    }

    fn do_random_move(&mut self) {
        let mut rng = thread_rng();
        let actions = self.all_actions();
        if let Some(choice) = rng.choose(&actions.as_slice()) {
            self.act(*choice);
        } else if self.player {
            self.player = !self.player;
            self.p1done = true;
        } else {
            self.player = !self.player;
            self.p2done = true;
        }
    }

    fn all_actions(&self) -> Actions {
        let mut result = Actions::new();
        let mut imagination = self.board;
        for (i, &boi) in (if self.player { self.board.friends } else { self.board.foes }).iter().enumerate() {
            for move_dir in self.board.all_around(boi, true) {
                (if self.player { &mut imagination.friends } else { &mut imagination.foes })[i] += move_dir.as_point();
                for build_dir in imagination.all_around(boi + move_dir.as_point(), false) {
//                    eprintln!("{}", self.board);
//                    eprintln!("{:?} is fine", boi + move_dir.as_point() + build_dir.as_point());
                    result.push(Action::new(i, move_dir, build_dir));
                }
                (if self.player { &mut imagination.friends } else { &mut imagination.foes })[i] -= move_dir.as_point();
            }
        }
        return result;
    }

    fn random_finish(&mut self) {
        while !self.done() {
            self.do_random_move();
            if self.p1done && self.p1score < self.p2score {
                self.p2done = true;
            } else if self.p2done && self.p2score < self.p1score {
                self.p1done = true;
            }
        }
    }
}

impl Board {
    fn all_around(&self, point: Point, walk: bool) -> Moves {
        let mut result = Moves::new();
        let mut dirs: usize = 0;
        let here = self[point];
        for &dir in ALL_DIRS.iter() {
            let yonder = point + dir.as_point();
            let there = self[yonder];
//            if there >= 0 && there < 4 && (!walk || (there - here <= 1 && !self.is_occupied(yonder))) {
            if !self.is_occupied(yonder) && there >= 0 && there < 4 && (!walk || there - here <= 1) {
                result.push(dir);
            }
        }
        return result;
    }
    fn is_occupied(&self, point: Point) -> bool {
        return self.friends[0] == point || self.friends[1] == point || self.foes[0] == point || self.foes[1] == point;
    }
    fn new() -> Board {
        return Board { field: [-1; BOARD_SIZE], friends: [Point(-1, -1); UNITS], foes: [Point(-1, -1); UNITS] };
    }
    fn test_board1() -> Board {
        return Board {
            field: [
                -1, -1, -1, 00, -1, -1, -1, -1,
                -1, -1, 00, 00, 00, -1, -1, -1,
                -1, 00, 00, 00, 00, 00, -1, -1,
                00, 00, 00, 00, 00, 00, 00, -1,
                -1, 00, 00, 00, 00, 00, -1, -1,
                -1, -1, 00, 00, 00, -1, -1, -1,
                -1, -1, -1, 00, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
            ],
            friends: [Point(3, 2), Point(3, 4), ],
            foes: [Point(2, 3), Point(4, 3)],
        };
    }
    fn test_board2() -> Board {
        return Board {
            field: [
                00, -1, -1, 00, -1, 00, -1, -1,
                -1, 00, -1, -1, 00, -1, 00, -1,
                -1, -1, 00, -1, 00, 00, -1, -1,
                00, -1, -1, 00, -1, -1, 00, -1,
                -1, 00, 00, -1, 00, -1, -1, -1,
                00, -1, 00, -1, -1, 00, -1, -1,
                -1, 00, -1, 00, -1, -1, 00, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
            ],
            friends: [Point(2, 2), Point(2, 4)],
            foes: [Point(4, 2), Point(4, 4)],
        };
    }
    fn place_units_randomly(&mut self) {
        let mut rng = thread_rng();
        let mut moves: Stack<i8> = Stack { elements: [0; POSSIBILITIES], head: 0 };
        for (i, &e) in self.field.iter().enumerate() {
            if e > -1 && e < 4 {
                moves.push(i as i8);
            }
        }
        moves.shuffle();
        for friend in &mut self.friends {
            *friend = Point::from(moves.pop().unwrap());
        }
        for foe in &mut self.foes {
            *foe = Point::from(moves.pop().unwrap());
        }
    }
}

impl GameManager {
    fn find_best_move(&self) -> Action {
        let actions = self.game.all_actions();
        let mut scores = Stack { elements: [0; POSSIBILITIES], head: actions.len() };
        let time = Instant::now();
        let mut count = 0;
        while time.elapsed().subsec_millis() < 45 {
            for (i, action) in actions.enumerate() {
                let mut subgame = self.game;
                let pla = subgame.player;
                subgame.act(action);
                subgame.random_finish();
                let mut score = subgame.p2score - subgame.p1score;
                if pla { score = subgame.p1score - subgame.p2score; }
                scores.elements[i] += score;
//                match subgame.winner() {
//                    1 => scores.elements[i] += score,
//                    2 => scores.elements[i] -= score,
//                    _ => ()
//                }
                if time.elapsed().subsec_millis() > 45 { break; }
            }
            count += 1;
        }
        eprintln!("{} iterations", count);
        let mut best = i32::min_value();
        let mut result = Action::empty();
        for (i, score) in scores.enumerate() {
            eprintln!("{}: {} -> {}", i, score, actions.elements[i]);
            if score > best {
                best = score;
                result = actions.elements[i];
            }
        }
        return result;
    }
    fn calc_score(&mut self) {
        if self.game.done() {
            match self.game.winner() {
                1 => self.wins += 1,
                2 => self.loss += 1,
                _ => ()
            }
        }
    }
}

fn main() {
    let testing = true;

    let mut manager = GameManager { game: Game::new(), wins: 0, loss: 0 };
    if testing {
        for iter in 0..10 {
            manager.game.reset(Board::test_board1());
//            manager.game = Game { board: Board{field: [-1,-1,-1,4,-1,-1,-1,-1,-1,-1,0,4,4,-1,-1,-1,-1,0,0,4,4,4,-1,-1,0,2,2,4,3,4,2,-1,-1,3,1,4,4,4,-1,-1,-1,-1,4,1,4,-1,-1,-1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,],friends:[Point(4, 2)],foes:[Point(3, 4)]}, size: 7, unitspp: 1, p1score: 2, p2score: 26, p1done: false, p2done: false, player: false };
            //manager.game.board.place_units_randomly();
            while !manager.game.done() {
//                manager.game.do_random_move();
//                let bad = manager.find_best_move();
//                manager.game.act(bad);
//                for dir in manager.game.board.all_around(boi, false) {
//                    eprintln!("{:?}", boi + dir.as_point());
//                }
//                for action in manager.game.all_actions() {
//                    eprintln!("{}", action);
//                }
                eprint!("{}", manager.game);
                eprintln!("{:?}", manager.game.board.friends[0]);
                let good = manager.find_best_move();
                manager.game.act(good);
//                eprint!("{}", manager.game);
//                for action in manager.game.all_actions() {
//                    eprintln!("{}", action);
//                }
//                eprintln!("good: {}", good);
                //           print!("{}",manager.game.board);
            }
            manager.calc_score();
        }
        println!("w:{} l:{}", manager.wins, manager.loss);
    } else {
        manager.game.init_inputs();
        while !manager.game.done() {
            manager.game.turn_inputs();
//            for action in manager.game.all_actions() {
//                eprintln!("{}", action);
//            }
            let good = manager.find_best_move();
            manager.game.act(good);
            eprintln!("{:?}", manager.game);
            //eprintln!("{:?}", manager.game);
            if good.index == 9 {
                println!("no more moves");
            } else {
                println!("{}", good);
            }
        }
    }
}

impl Dir {
    fn as_point(&self) -> Point {
        match self {
            Dir::NW => Point(-1, -1),
            Dir::N => Point(-1, 0),
            Dir::NE => Point(-1, 1),
            Dir::E => Point(0, 1),
            Dir::SE => Point(1, 1),
            Dir::S => Point(1, 0),
            Dir::SW => Point(1, -1),
            Dir::W => Point(0, -1)
        }
    }
}

impl Point {
    fn from(index: i8) -> Point {
        return Point(index / 8, index % 8);
    }
    fn as_index(&self) -> usize {
        return ((self.0 * 8 + self.1) & 0b111111) as usize;
    }
}

impl Index<Point> for Board {
    type Output = i8;
    fn index(&self, point: Point) -> &Self::Output {
        return &self.field[point.as_index()];
    }
}

impl IndexMut<Point> for Board {
    fn index_mut(&mut self, point: Point) -> &mut Self::Output {
        return &mut self.field[point.as_index()];
    }
}

impl Add for Point {
    type Output = Point;
    fn add(self, Point(y, x): Point) -> Point {
        return Point(self.0 + y, self.1 + x);
    }
}

impl AddAssign for Point {
    fn add_assign(&mut self, Point(y, x): Point) {
        self.0 += y;
        self.1 += x;
    }
}

impl Sub for Point {
    type Output = Point;
    fn sub(self, Point(y, x): Point) -> Point {
        return Point(self.0 - y, self.1 - x);
    }
}

impl SubAssign for Point {
    fn sub_assign(&mut self, Point(y, x): Point) {
        self.0 -= y;
        self.1 -= x;
    }
}

impl PartialEq for Point {
    fn eq(&self, Point(y, x): &Point) -> bool {
        return self.0 == *y && self.1 == *x;
    }
}

impl fmt::Display for Game {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.board);
        return writeln!(f, "{}ig: {}  {}mall: {}", if self.player { "B" } else { "b" }, self.p1score, if self.player { "s" } else { "S" }, self.p2score);
    }
}

impl fmt::Display for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "  ");
        for i in 0..7 {
            write!(f, "{} ", i);
        }
        writeln!(f, "");
        for i in 0..7 {
            write!(f, "{} ", i);
            for j in 0..7 {
                let isfriend = self.friends.iter().any(|p| *p == Point(i, j));
                let isfoe = self.foes.iter().any(|p| *p == Point(i, j));
                let mut tile = self[Point(i, j)];
                tile += if isfriend { 65 } else { if isfoe { 97 } else { 0 } };
                if tile == -1 {
                    write!(f, ". ");
                } else if isfriend || isfoe {
                    write!(f, "{} ", tile as u8 as char);
                } else {
                    write!(f, "{} ", tile);
                }
            }
            writeln!(f, "");
        }
        return write!(f, "");
    }
}

impl fmt::Debug for Board {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Board{{field: [");
        for &tile in self.field.iter() {
            write!(f, "{},", tile);
        }
        return write!(f, "],friends:{:?},foes:{:?}}}", self.friends, self.foes);
    }
}

impl fmt::Display for Dir {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", match self {
            Dir::NW => "NW",
            Dir::N => "N",
            Dir::NE => "NE",
            Dir::E => "E",
            Dir::SE => "SE",
            Dir::S => "S",
            Dir::SW => "SW",
            Dir::W => "W"
        })
    }
}

impl fmt::Display for Action {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "MOVE&BUILD {} {} {}", self.index, self.move_dir, self.build_dir)
    }
}

impl Game {
    fn init_inputs(&mut self) {
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        self.size = parse_input!(input_line, i32);
        input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        self.unitspp = parse_input!(input_line, i32);
    }

    //noinspection RsAssignToImmutable
    fn turn_inputs(&mut self) {
        for i in 0..self.size as i8 {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let row = input_line.trim().chars().collect::<Vec<_>>();
            for j in 0..self.size as i8 {
                if row[j as usize] == '.' {
                    self.board[Point(i, j)] = -1;
                } else {
                    self.board[Point(i, j)] = row[j as usize] as i8 - 48;
                }
            }
        }
        for friend in 0..self.board.friends.len() {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let inputs = input_line.split(" ").collect::<Vec<_>>();
            self.board.friends[friend] = Point(parse_input!(inputs[1], i8), parse_input!(inputs[0], i8));
        }
        for foe in 0..self.board.foes.len() {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
            let inputs = input_line.split(" ").collect::<Vec<_>>();
            self.board.foes[foe] = Point(parse_input!(inputs[1], i8), parse_input!(inputs[0], i8));
        }
        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let legal_actions = parse_input!(input_line, i32);
        for i in 0..legal_actions as usize {
            let mut input_line = String::new();
            io::stdin().read_line(&mut input_line).unwrap();
        }
        self.player = true;
        if self.board[self.board.foes[0]] == 3 && !self.p2done {
            self.p2score += 1;
        }
    }
}
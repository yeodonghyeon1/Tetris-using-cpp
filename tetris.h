#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <conio.h>
#include <algorithm>
#include <atomic>

// #include "blockAction.h"
using namespace std;
#define UP 72  // Keyboard arrow up
#define DOWN 80 // Keyboard arrow down
#define LEFT 75 // Keyboard arrow left
#define RIGHT 77 // Keyboard arrow right


// Block creation position or current block position
struct block_window {
    int insert_window_front;
    int insert_window_back;
    int insert_window_up;
    int insert_window_down;
};

// Real-time game state
struct game_state {
    int x, y; // Map size setting
    bool bind_block; // Block fixable possibility
    bool game_state; // Game end possibility
    bool successfully_bind_block; // Block fix success possibility
    std::atomic_bool running; // Thread run runtime
};

// Main Tetris class
class Tetris {
public:

    int roation_handler(int lotation_number); // Block rotation handler
    void game_over_handler(); // Game over handler

    Tetris();
    void set_x(int new_x); // Set x coordinate
    void set_y(int new_y); // Set y coordinate
    void run(); // Tetris run
    ~Tetris() = default; // Default class termination

private:
    std::shared_ptr<block_window> bw; // Shared pointer for block_window struct
    std::shared_ptr<game_state> state; // Shared pointer for game_state struct
    std::shared_ptr<Block> current_block; // Shared pointer for current block
    std::shared_ptr<std::vector<std::vector<int>>> map; // Shared pointer for map data
    std::shared_ptr<std::mutex> mtx; // Shared pointer for thread mutex
    void init(); // Initial setup
    std::shared_ptr<std::vector<std::vector<int>>> gridmap(); // Shared pointer for map data
    void show_map(std::shared_ptr<std::vector<std::vector<int>>> map); // Map render
};

#endif
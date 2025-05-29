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
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

struct block_window {
    int insert_window_front;
    int insert_window_back;
    int insert_window_up;
    int insert_window_down;
};

struct game_state {
    int x, y;
    bool bind_block;
    bool game_state;
    bool successfully_bind_block;
    std::atomic_bool running;
};

class Tetris {
public:

    int roation_handler(int lotation_number);
    Tetris();
    void set_x(int new_x);
    void set_y(int new_y);
    void run();
    ~Tetris();

private:
    std::shared_ptr<block_window> bw;
    std::shared_ptr<game_state> state;
    std::shared_ptr<Block> current_block;
    std::shared_ptr<std::vector<std::vector<int>>> map;
    std::shared_ptr<std::mutex> mtx;
    std::thread t1;
    void init();
    void clear_block();
    void down_block_and_bind();
    std::shared_ptr<Block> selete_block(int block_number);
    void insert_block(std::shared_ptr<std::vector<std::vector<int>>> map);
    void game_over();
    std::shared_ptr<std::vector<std::vector<int>>> gridmap();
    void show_map(std::shared_ptr<std::vector<std::vector<int>>> map);
};

#endif
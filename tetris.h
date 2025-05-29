#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <vector>
#include <thread>
#include "block.h"
#include <mutex>
#include <memory>
#include <conio.h>
#include <algorithm>
#include <atomic>
#include "tetris.h"
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


class Tetris {
public:
    bool successfully_bind_block;
    std::shared_ptr<Block> current_block;
    std::shared_ptr<std::vector<std::vector<int>>> map;
    bool game_state;


    std::shared_ptr<block_window> bw;

    Tetris();
    void set_x(int new_x);
    void set_y(int new_y);
    void run();
    ~Tetris();

private:
    int x, y;
    std::shared_ptr<std::mutex> mtx;
    std::atomic_bool running;
    std::thread t1;
    bool bind_block;
    void clear_block();
    void down_block_and_bind();
    std::shared_ptr<Block> selete_block(int block_number);
    void insert_block(std::shared_ptr<std::vector<std::vector<int>>> map);
    void game_over();
    std::shared_ptr<std::vector<std::vector<int>>> gridmap();
    void show_map(std::shared_ptr<std::vector<std::vector<int>>> map);
};

#endif
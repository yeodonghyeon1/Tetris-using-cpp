#ifndef KEY_ACTION_H
#define KEY_ACTION_H
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#include <vector>
#include <memory>
#include "block.h"
#include <thread> 
#include <atomic>
#include "blockAction.h"

using namespace std;

class Tetris;

class KeyAction {
public:
    KeyAction(Tetris* te, int x, int y, std::shared_ptr<vector<vector<int>>> map, bool& bind_block, std::atomic_bool& running, std::shared_ptr<block_window> bw, std::shared_ptr<Block>& current_block, std::shared_ptr<std::mutex>& mtx);
    void key_event();
    ~KeyAction();


private:
    int x,y;
    std::shared_ptr<Block>& current_block_ref;
    std::shared_ptr<block_window> bw;
    std::thread t1;
    std::shared_ptr<vector<vector<int>>> map;
    std::shared_ptr<std::mutex>& mtx;
    bool& bind_block;
    std::atomic_bool& running;
    Tetris* te;
    // bool& game_state;
};

#endif
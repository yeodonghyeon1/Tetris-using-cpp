#ifndef PLAY_ACTION_H
#define PLAY_ACTION_H

#include <vector>
#include <memory>
#include "block.h"
#include <atomic>
#include "tetris.h"
using namespace std;

class PlayAction {
public:
    PlayAction(std::shared_ptr<game_state> state, std::shared_ptr<vector<vector<int>>> map, std::shared_ptr<block_window> bw,  std::shared_ptr<std::mutex>& mtx);

    void clear_block();
    void down_block_and_bind();
    void game_over();

    ~PlayAction();
private:
    std::shared_ptr<game_state> state;
    std::shared_ptr<vector<vector<int>>> map;
    std::shared_ptr<block_window> bw;
    std::shared_ptr<std::mutex>& mtx;
    std::thread t1;

};

#endif
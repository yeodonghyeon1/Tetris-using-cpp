#ifndef PLAY_ACTION_H
#define PLAY_ACTION_H

#include <vector>
#include <memory>
#include "block.h"
#include <atomic>

using namespace std;

class PlayAction {
public:
    PlayAction(std::atomic_bool& running, bool& game_state);

    void clear_block();
    void down_block_and_bind();
    void game_over();
private:
    std::atomic_bool& running;
    bool& game_state;

};

#endif
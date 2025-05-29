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
    PlayAction(std::shared_ptr<game_state> state);

    void clear_block();
    void down_block_and_bind();
    void game_over();
private:
    std::shared_ptr<game_state> state;


};

#endif
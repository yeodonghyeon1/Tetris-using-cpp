#ifndef BLOCK_ACTION_H
#define BLOCK_ACTION_H

#include <vector>
#include <memory>
#include <algorithm>
#include "block.h"
#include "tetris.h"

using namespace std;


// Action class corresponding to the block
// Block rotation
// Block selection
// Block insertion
class ActionBlock {
public:
    ActionBlock(Tetris* ts, std::shared_ptr<game_state> state, std::shared_ptr<vector<vector<int>>> map, std::shared_ptr<Block>& current_block, std::shared_ptr<block_window> bw);
    bool block_rotation(int lotation_number); // block rotation
    std::shared_ptr<Block> selete_block(int block_number); // block selection
    void insert_block(std::shared_ptr<std::vector<std::vector<int>>> map); // block insertion

private:
    Tetris* ts;
    std::shared_ptr<game_state> state;
    std::shared_ptr<block_window> bw;
    std::shared_ptr<vector<vector<int>>> map;
    std::shared_ptr<Block>& current_block_ref;
};

#endif
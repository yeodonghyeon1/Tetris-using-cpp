#include "blockAction.h"

ActionBlock::ActionBlock(int x, int y, std::shared_ptr<vector<vector<int>>> map, std::shared_ptr<Block>& current_block, std::shared_ptr<block_window> bw)
    : x(x), y(y), map(map), current_block_ref(current_block), bw(bw){

}

bool ActionBlock::block_rotation(int lotation_number){
    int down = std::clamp(bw->insert_window_down, 0, y-1);
    int up = std::clamp(bw->insert_window_up, 0, y-1);
    int back = std::clamp(bw->insert_window_back, 0, x-1);
    int front = std::clamp(bw->insert_window_front, 0, x-1);
    vector<vector<vector<int>>> block = current_block_ref->create_block();
    int block_x=0;
    int block_y=0;
    int rotation_allow = 0;

        for(int i = up; i < down; ++i){
        block_x = 0;
        for(int j = front; j < back; ++j){
                if(block[lotation_number][block_y][block_x] == 1){
                    if((*map)[i][j] == 1 || (*map)[i][j] == 0){
                        rotation_allow++;
                    }
                }
                block_x++;
            }
        block_y++;
    }

    block_x=0;
    block_y=0;
    for(int i = up; i < down; ++i){
        block_x = 0;
        for(int j = front; j < back; ++j){
            if(rotation_allow == 4){
                if((*map)[i][j] == 1 || (*map)[i][j] == 0){
                    (*map)[i][j] = block[lotation_number][block_y][block_x];
                }
            }
            block_x++;

        }
        block_y++;
    }

    if(rotation_allow ==4)
        return true;
    else
        return false;
}


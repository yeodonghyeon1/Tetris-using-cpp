#include "blockAction.h"

ActionBlock::ActionBlock(Tetris* ts, std::shared_ptr<game_state> state, std::shared_ptr<vector<vector<int>>> map, std::shared_ptr<Block>& current_block, std::shared_ptr<block_window> bw)
    : ts(ts), state(state), map(map), current_block_ref(current_block), bw(bw){

}


// Block rotation function. The logic checks and applies rotation possibility through rotation_allow (rotation is only allowed when the number of blocks is 4) from the current_block within the current window.
bool ActionBlock::block_rotation(int lotation_number){
    int down = std::clamp(bw->insert_window_down, 0, state->y-1);
    int up = std::clamp(bw->insert_window_up, 0, state->y-1);
    int back = std::clamp(bw->insert_window_back, 0, state->x-1);
    int front = std::clamp(bw->insert_window_front, 0, state->x-1);
    vector<vector<vector<int>>> block = current_block_ref->create_block();
    int block_x=0;
    int block_y=0;
    int rotation_allow = 0; // Rotation is only possible when 4 or more block units are allowed

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


// Block selection function
std::shared_ptr<Block> ActionBlock::selete_block(int block_number){
    switch(block_number) {
        case 0: {
            auto b = std::make_shared<I_Mino>();
            return b;
        }
        case 1: {
            auto b = std::make_shared<O_Mino>();
            return b;
        }
        case 2: {
            auto b = std::make_shared<T_Mino>();
            return b;
        }
        case 3: {
            auto b = std::make_shared<L_Mino>();
            return b;
        }
        case 4: {
            auto b = std::make_shared<J_Mino>();
            return b;
        }
        case 5: {
            auto b = std::make_shared<S_Mino>();
            return b;
        }
        case 6: {
            auto b = std::make_shared<Z_Mino>();
            return b;
        }
        default:
            return nullptr;
    }
}

// Function to insert block data into map
void ActionBlock::insert_block(std::shared_ptr<std::vector<std::vector<int>>> map){
    vector<vector<vector<int>>> block = current_block_ref->create_block();
    
    bw->insert_window_front = ((state->x/2 -2));
    bw->insert_window_back = ((state->x/2 +2));
    bw->insert_window_up = 1; 
    bw->insert_window_down = 5; 
    int block_x=0;
    int block_y=0;
    // Block creation area

    for(int i = bw->insert_window_up; i < bw->insert_window_down; ++i){
        block_x = 0;
        for(int j = bw->insert_window_front; j < bw->insert_window_back; ++j){
            if(block[3][block_y][block_x] == 1){
                if((*map)[i][j] == 2){
                    ts->game_over_handler();
                }
            }
            (*map)[i][j] = block[3][block_y][block_x];
            block_x++;

        }   
        block_y++;
    }
    state->successfully_bind_block = false;
}
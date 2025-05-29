#include <vector>
#include <memory>
#include "block.h"
#include "keyAction.h"
#include <conio.h>
#include <mutex>
#include <algorithm>
#include <atomic>
#include "tetris.h"
#include "blockAction.h"

KeyAction::KeyAction(Tetris* te, std::shared_ptr<game_state> state, std::shared_ptr<std::vector<std::vector<int>>> map, std::shared_ptr<block_window> bw, std::shared_ptr<Block>& current_block, std::shared_ptr<std::mutex>& mtx)
    : te(te), state(state), map(map), bw(bw), current_block_ref(current_block), mtx(mtx){
    t1 = std::thread(&KeyAction::key_event, this);
}

KeyAction::~KeyAction(){
    if(t1.joinable()) t1.join();
}

void KeyAction::key_event(){
    char c;
    bool can_move = false;
    int lotation_number = 0;
    while(state->running){
        if (_kbhit()) {
            c = _getch();
            if (c == -32) {
                c = _getch();
                {
                    int move_number = 0;
                    std::lock_guard<std::mutex> lock(*mtx);
                    int down = std::clamp(bw->insert_window_down, 0, state->y-1);
                    int up = std::clamp(bw->insert_window_up, 0, state->y-1);
                    int back = std::clamp(bw->insert_window_back, 0, state->x-1);
                    int front = std::clamp(bw->insert_window_front, 0, state->x-1);
                    switch (c) {
                    case LEFT:
                        for(int i = up; i < down; ++i){
                            for(int j = front; j < back; ++j){ 
                                if((*map)[i][j] == 1 && ((*map)[i][j-1] == 0 || (*map)[i][j-1] == 1)){
                                    move_number++;
                                    if(move_number == 4){
                                        can_move = true;
                                        move_number = 0;
                                    }
                                }
                            }
                        }
                        if(can_move){
                            for(int i = up; i < down; ++i){
                                for(int j = front; j < back; ++j){ 
                                    if((*map)[i][j] == 1 && ((*map)[i][j-1] == 0 || (*map)[i][j-1] == 1)){
                                        (*map)[i][j] = 0;
                                        (*map)[i][j-1] = 1;
                                    }
                                }
                            }
                        }
                        if(can_move){
                            bw->insert_window_front--;
                            bw->insert_window_back--;
                            can_move = false;
                        }
                        break;
                    case RIGHT:
                        for(int i = up; i < down; ++i){
                            for(int j = back-1; j > front-1; --j){ 
                                if((*map)[i][j] == 1 && ((*map)[i][j+1] == 0 || (*map)[i][j+1] == 1)){
                                    move_number++;
                                    if(move_number == 4){
                                        can_move = true;
                                        move_number = 0;
                                    }
                                }
                            }
                        }
                        if(can_move){
                            for(int i = up; i < down; ++i){
                                for(int j = back-1; j > front-1; --j){ 
                                    if((*map)[i][j] == 1 && ((*map)[i][j+1] == 0 || (*map)[i][j+1] == 1)){
                                        (*map)[i][j] = 0;
                                        (*map)[i][j+1] = 1;
                                    }
                                }
                            }
                        }
                        if(can_move){
                            bw->insert_window_front++;
                            bw->insert_window_back++;
                            can_move = false;
                        }
                        break;
                    case UP: {
                        lotation_number = te->roation_handler(lotation_number);
                        break;
                    }
                    case DOWN:
                        for(int i = down-1; i >= up-1; --i){
                            for(int j = back-1; j >= front-1; --j){
                                if((*map)[i][j] == 1){
                                    if((*map)[i+1][j] == -1 || (*map)[i+1][j]== 2){
                                        state->bind_block = true;
                                    }
                                }
                            }   
                        }
                        for(int i = down-1; i > up-1; --i){
                            for(int j = front; j < back; ++j){
                                if(!state->bind_block){
                                    if((*map)[i][j] == 1 && (*map)[i+1][j] == 0){
                                        (*map)[i][j] = 0;
                                        (*map)[i+1][j] = 1;
                                    }
                                }
                            }
                        }
                        if(!state->bind_block){
                            bw->insert_window_up++;
                            bw->insert_window_down++;
                        }
                        break;
                    case ' ':
                        // Space bar action to be implemented
                        break;
                    }
                }
            }
        }
    }
}

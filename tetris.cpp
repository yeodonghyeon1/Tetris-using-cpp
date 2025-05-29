#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <conio.h>
#include <algorithm>
#include <atomic>
#include "block.h"
#include "tetris.h"
#include "keyAction.h"
#include "playAction.h"
using namespace std;





Tetris::Tetris(){
}

void Tetris::set_x(int new_x){
    state->x = new_x;
}
void Tetris::set_y(int new_y){
    state->y = new_y;
}

void Tetris::init(){
    state = std::make_shared<game_state>();
    state->x = 10;
    state->y = 20;
    state->running = true;
    state->bind_block = false;
    state->game_state = true;
    state->successfully_bind_block = true;
    bw = std::make_shared<block_window>();
    bw->insert_window_front = ((state->x/2 -2));
    bw->insert_window_back = ((state->x/2 +2));
    bw->insert_window_up = 1; 
    bw->insert_window_down = 5; 
    mtx = std::make_shared<std::mutex>();
    map = gridmap();
}

int Tetris::roation_handler(int lotation_number){
    std::shared_ptr<ActionBlock> ab = std::make_shared<ActionBlock>(this, state, map, current_block, bw);
    bool result = ab->block_rotation(lotation_number);
     if(result){ 
        lotation_number++;
    }
    if(lotation_number == 4){
        lotation_number = 0;
    }
    return lotation_number;
}

void Tetris::run(){
    init();
    t1 = std::thread(&Tetris::down_block_and_bind, this);
    std::shared_ptr<KeyAction> key_thread = std::make_shared<KeyAction>(this, state, map, bw, current_block, mtx);
    int block_number = 0;
    while(true){
        if(state->successfully_bind_block){
            std::shared_ptr<ActionBlock> ab = std::make_shared<ActionBlock>(this, state, map, current_block, bw);

            current_block = ab->selete_block(block_number);
            ab->insert_block(map);
            block_number++;

            if(block_number == 7){
                block_number = 0;
            }       
        }             
        show_map(map);
        system("cls");
        if(!state->game_state) break;
    }
}

Tetris::~Tetris(){
    if(t1.joinable()) t1.join();
}



void Tetris::clear_block(){
    int line_clear_count = 0;
    int line_clear = 0;
    for(int i = state->y -2; i > 0; --i){
        for(int j = state->x -2; j > 0; --j){
            if((*map)[i][j] == 2){
                line_clear_count++;
            }
        } 
        if(line_clear_count == (state->x -2)){
            line_clear++;
            for(int n = state->x -2; n > 0; --n){
                (*map)[i][n] = 0;
            }
        }
        line_clear_count = 0;
    }
    
    while(line_clear != 0){
        for(int i = state->y -2; i > 1; --i){
            for(int j = state->x -2; j > 0; --j){
                if((*map)[i][j] == 0){
                    line_clear_count++;
                }
            }
            if(line_clear_count == (state->x-2)){
                for(int n = state->x-2; n>0; --n){
                    (*map)[i][n] = (*map)[i-1][n];
                    (*map)[i-1][n] = 0;
                }   
            }
            line_clear_count = 0;
        }
        line_clear--;
    }

    if(line_clear == true){
        line_clear = false;
    }            
}

void Tetris::down_block_and_bind(){
    while(state->running){
        mtx->lock();


        // Iterate over the entire board
        // for(int i = y-1; i > 0; --i){
        //     for(int j = x-1; j > 0; --j){
        //         if(map[i][j] == 1){
        //             if((map[i+1][j] == -1 || map[i+1][j]== 2)){
        //                 bind_block = true;
        //             }
        //         }
        //     }   
        // }
        //3  7 (3 4 5 6)
        //1  5 (1 2 3 4)
        // Check only within the window region (18*8*2 = 288 iterations -> optimized to 4*4*2 = 32 iterations)

        int down = std::clamp(bw->insert_window_down, 0, state->y-1);
        int up = std::clamp(bw->insert_window_up, 0, state->y-1);
        int back = std::clamp(bw->insert_window_back, 0, state->x-1);
        int front = std::clamp(bw->insert_window_front, 0, state->x-1);


        for(int i = down-1; i >= up; --i){
            for(int j = back-1; j >= front; --j){
                if((*map)[i][j] == 1){
                    if(((*map)[i+1][j] == -1 || (*map)[i+1][j]== 2)){
                        state->bind_block = true;
                    }
                }
            }   
        }
        for(int i = down-1; i >= up; --i){
            for(int j = back-1; j >= front; --j){
                if((*map)[i][j] == 1){
                    if(state->bind_block){
                        (*map)[i][j] = 2;
                    }
                    else{
                        (*map)[i][j] = 0;
                        (*map)[i+1][j] = 1;     
                    }
                }
            }
        }

        if(state->bind_block){
            state->successfully_bind_block = true;
            state->bind_block = false;
            clear_block();
        }
        else{
            bw->insert_window_up++;
            bw->insert_window_down++;
        }
        mtx->unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }
}

void Tetris::game_over_handler(){
    state->game_state = false;
    state->running = false;
}

std::shared_ptr<std::vector<std::vector<int>>> Tetris::gridmap(){
    auto v2 = std::make_shared<std::vector<std::vector<int>>>(state->y, std::vector<int>(state->x, -1));
    for(int i =0; i< state->y; ++i){
        for(int j =0; j< state->x; ++j){
            if(i == 0) break;
            if(i == (state->y-1)) break;
            if(j == 0) continue;
            if(j == (state->x-1)) continue;
            (*v2)[i][j] = 0;
        }
    }
    return v2;
}

void Tetris::show_map(std::shared_ptr<std::vector<std::vector<int>>> map){
    for(int i = 0; i< state->y; ++i){
        for(int j =0; j< state->x; ++j){
            // Wall
            if((*map)[i][j] == -1){
                std::cout << "бс ";
            }
            // Empty space
            else if((*map)[i][j] == 0){
                std::cout << "  ";
            }
            // Current block
            else if((*map)[i][j] == 1){
                std::cout << "бр ";
            }
            // Fixed block
            else if((*map)[i][j] == 2){
                std::cout << "в├ ";
            }
        }
        std::cout << std::endl;  
    }
}

int main(){
    Tetris te;
    te.run();
    system("pause"); 

}
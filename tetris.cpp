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

// Set map size
void Tetris::set_x(int new_x){
    state->x = new_x;
}
void Tetris::set_y(int new_y){
    state->y = new_y;
}


// Tetris initial setup
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


// Tetris execution
void Tetris::run(){
    init();

    // Declare and initialize block drop and bind thread and keyboard input thread in main run
    std::shared_ptr<PlayAction> down_clear_bind_thread = std::make_shared<PlayAction>(state, map, bw, mtx);
    std::shared_ptr<KeyAction> key_thread = std::make_shared<KeyAction>(this, state, map, bw, current_block, mtx);
    int block_number = 0; //첫 번째 0번 블럭
    // Select and create a new block upon successful bind and insert into map vector, render with show_map() and system("cls"). End game if game_state is false.
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

// Block rotation handler
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


// Game over handler
void Tetris::game_over_handler(){
    std::shared_ptr<PlayAction> pa = std::make_shared<PlayAction>(state, map, bw, mtx);
    pa->game_over();
}


// Initial map setup
std::shared_ptr<std::vector<std::vector<int>>> Tetris::gridmap(){
    // Designed as a 2D vector
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


// Map render
void Tetris::show_map(std::shared_ptr<std::vector<std::vector<int>>> map){
    for(int i = 0; i< state->y; ++i){
        for(int j =0; j< state->x; ++j){
            // Wall
            if((*map)[i][j] == -1){
                std::cout << "■ ";
            }
            // Empty space
            else if((*map)[i][j] == 0){
                std::cout << "  ";
            }
            // Current block
            else if((*map)[i][j] == 1){
                std::cout << "□ ";
            }
            // Fixed block
            else if((*map)[i][j] == 2){
                std::cout << "▣ ";
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
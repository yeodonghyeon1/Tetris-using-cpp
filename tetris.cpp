#include <iostream>
#include <vector>
#include <thread>
#include "block.h"
#include <mutex>
#include <memory>
#include <conio.h>
#include <algorithm>
#include <atomic>
#include "tetris.h"
#include "keyAction.h"
using namespace std;





Tetris::Tetris(){
    x = 10;
    y = 20;
    bw = std::make_shared<block_window>();
    bw->insert_window_front = ((x/2 -2));
    bw->insert_window_back = ((x/2 +2));
    bw->insert_window_up = 1; 
    bw->insert_window_down = 5; 
    mtx = std::make_shared<std::mutex>();
    running = true;
    bind_block = false;
    game_state = true;
    successfully_bind_block = true;
    
}

void Tetris::set_x(int new_x){
    x= new_x;
}
void Tetris::set_y(int new_y){
    y= new_y;
}

void Tetris::run(){
    map = gridmap();
    t1 = std::thread(&Tetris::down_block_and_bind, this);

    std::shared_ptr<KeyAction> key_thread = std::make_shared<KeyAction>(x, y, map, bind_block, running, bw, current_block, mtx);
    int block_number = 0;
    while(true){
        if(successfully_bind_block){
            current_block = selete_block(block_number);
            insert_block(map);
            block_number++;

            if(block_number == 7){
                block_number = 0;
            }       
        }             
        show_map(map);
        system("cls");
        if(!game_state) break;
    }
}

Tetris::~Tetris(){
    if(t1.joinable()) t1.join();

}



void Tetris::clear_block(){

    int line_clear_count = 0;
    int line_clear = 0;
    for(int i = y -2; i > 0; --i){
        for(int j = x -2; j > 0; --j){
            if((*map)[i][j] == 2){
                line_clear_count++;
            }
        } 
        if(line_clear_count == (x -2)){
            line_clear++;
            for(int n = x -2; n > 0; --n){
                (*map)[i][n] = 0;
            }
        }
        line_clear_count = 0;
    }
    
    while(line_clear != 0){
        for(int i = y -2; i > 1; --i){
            for(int j = x -2; j > 0; --j){
                if((*map)[i][j] == 0){
                    line_clear_count++;
                }
            }
            if(line_clear_count == (x-2)){
                for(int n = x-2; n>0; --n){
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
    while(running){
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

        int down = std::clamp(bw->insert_window_down, 0, y-1);
        int up = std::clamp(bw->insert_window_up, 0, y-1);
        int back = std::clamp(bw->insert_window_back, 0, x-1);
        int front = std::clamp(bw->insert_window_front, 0, x-1);


        for(int i = down-1; i >= up; --i){
            for(int j = back-1; j >= front; --j){
                if((*map)[i][j] == 1){
                    if(((*map)[i+1][j] == -1 || (*map)[i+1][j]== 2)){
                        bind_block = true;
                    }
                }
            }   
        }
        for(int i = down-1; i >= up; --i){
            for(int j = back-1; j >= front; --j){
                if((*map)[i][j] == 1){
                    if(bind_block){
                        (*map)[i][j] = 2;
                    }
                    else{
                        (*map)[i][j] = 0;
                        (*map)[i+1][j] = 1;     
                    }
                }
            }
        }

        if(bind_block){
            successfully_bind_block = true;
            bind_block = false;
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

std::shared_ptr<Block> Tetris::selete_block(int block_number){
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

void Tetris::insert_block(std::shared_ptr<std::vector<std::vector<int>>> map){
    vector<vector<vector<int>>> block = current_block->create_block();
    
    bw->insert_window_front = ((x/2 -2));
    bw->insert_window_back = ((x/2 +2));
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
                    game_over();
                }
            }
            (*map)[i][j] = block[3][block_y][block_x];
            block_x++;

        }   
        block_y++;
    }
    successfully_bind_block = false;
}

void Tetris::game_over(){
    game_state = false;
    running = false;
}

std::shared_ptr<std::vector<std::vector<int>>> Tetris::gridmap(){
    auto v2 = std::make_shared<std::vector<std::vector<int>>>(y, std::vector<int>(x, -1));
    for(int i =0; i< y; ++i){
        for(int j =0; j< x; ++j){
            if(i == 0) break;
            if(i == (y-1)) break;
            if(j == 0) continue;
            if(j == (x-1)) continue;
            (*v2)[i][j] = 0;
        }
    }
    return v2;
}

void Tetris::show_map(std::shared_ptr<std::vector<std::vector<int>>> map){
    for(int i = 0; i< y; ++i){
        for(int j =0; j< x; ++j){
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
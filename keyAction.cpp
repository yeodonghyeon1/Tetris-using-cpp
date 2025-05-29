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

KeyAction::KeyAction(int x, int y, std::shared_ptr<std::vector<std::vector<int>>> map, bool& bind_block, std::atomic_bool& running, std::shared_ptr<block_window> bw, std::shared_ptr<Block>& current_block, std::shared_ptr<std::mutex>& mtx) 
    : x(x), y(y), map(map), bind_block(bind_block), running(running), bw(bw), current_block_ref(current_block), mtx(mtx){
    t1 = std::thread(&KeyAction::key_event, this);

}

KeyAction::~KeyAction(){
    if(t1.joinable()) t1.join();
}



void KeyAction::key_event(){
            char c;
            bool can_move = false;
            int lotation_number = 0;
            while(running) {
                if (_kbhit()) {        // Check keyboard input (true / false)
                    c = _getch();      // Remove the leading 224 value because arrow keys send 224 00 sequence
                    if (c == -32) {    // If the input is -32
                        c = _getch();  // Identify which arrow key was pressed
                        {
                            int move_number = 0;

                            std::lock_guard<std::mutex> lock(*mtx);
                            int down = std::clamp(bw->insert_window_down, 0, y-1);
                            int up = std::clamp(bw->insert_window_up, 0, y-1);
                            int back = std::clamp(bw->insert_window_back, 0, x-1);
                            int front = std::clamp(bw->insert_window_front, 0, x-1);
                            
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
                                std::shared_ptr<ActionBlock> rotate_block_ptr = std::make_shared<ActionBlock>(x, y, map, current_block_ref, bw);
                                bool result = rotate_block_ptr->block_rotation(lotation_number);
                                if(result){ 
                                    lotation_number++;
                                }
                                if(lotation_number == 4){
                                    lotation_number = 0;
                                }
                                break;
                            }
                            case DOWN:
                                for(int i = down-1; i >= up-1; --i){
                                    for(int j = back-1; j >= front-1; --j){
                                        if((*map)[i][j] == 1){
                                            if((*map)[i+1][j] == -1 || (*map)[i+1][j]== 2){
                                                bind_block = true;
                                            }
                                        }
                                    }   
                                }
                                for(int i = down-1; i > up-1; --i){
                                    for(int j = front; j < back; ++j){
                                        if(!bind_block){
                                            if((*map)[i][j] == 1 && (*map)[i+1][j] == 0){
                                                (*map)[i][j] = 0;
                                                (*map)[i+1][j] = 1;
                                            }
                                        }
                                    }
                                }
                                if(!bind_block){
                                    bw->insert_window_up++;
                                    bw->insert_window_down++;
                                }
                                break;
                            // Space bar input handling (currently empty)
                            case ' ':
                                // Space bar action to be implemented
                                break;
                    }
                }   
            }
        }
    }    
}

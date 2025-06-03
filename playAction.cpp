#include "playAction.h"
#include <thread>


// Game overall progress action class
PlayAction::PlayAction(std::shared_ptr<game_state> state, std::shared_ptr<vector<vector<int>>> map, std::shared_ptr<block_window> bw,  std::shared_ptr<std::mutex>& mtx)
    : state(state), map(map), bw(bw), mtx(mtx){
    t1 = std::thread(&PlayAction::down_block_and_bind, this);

}

PlayAction::~PlayAction(){
    if(t1.joinable()) t1.join();

}
void PlayAction::game_over(){
    state->game_state = false;
    state->running = false;
}


// Bind block line clear function
void PlayAction::clear_block(){
    int line_clear_count = 0; // Check clear line
    int line_clear = 0; // Number of lines to clear


    // Check clear line
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
    
    // Apply to map using line_clear_cout and line_clear information
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

    // if(line_clear == true){
    //     line_clear = false;
    // }            
}

// Block automatic drop and block bind function
void PlayAction::down_block_and_bind(){
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
        // Board maximum size can be set to the window
        int down = std::clamp(bw->insert_window_down, 0, state->y-1);
        int up = std::clamp(bw->insert_window_up, 0, state->y-1);
        int back = std::clamp(bw->insert_window_back, 0, state->x-1);
        int front = std::clamp(bw->insert_window_front, 0, state->x-1);


        // Bind check
        for(int i = down-1; i >= up; --i){
            for(int j = back-1; j >= front; --j){
                if((*map)[i][j] == 1){
                    if(((*map)[i+1][j] == -1 || (*map)[i+1][j]== 2)){
                        state->bind_block = true;
                    }
                }
            }   
        }

        // Bind apply
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

        // Create new block and check block clear status upon successful bind
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

        // Drop every 0.5 seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }
}

#include "playAction.h"

PlayAction::PlayAction(std::atomic_bool& running, bool& game_state)
    : running(running), game_state(game_state){

}
void PlayAction::game_over(){
    game_state = false;
    running = false;
}


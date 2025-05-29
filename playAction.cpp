
#include "playAction.h"

PlayAction::PlayAction(std::shared_ptr<game_state> state)
    : state(state){

}
void PlayAction::game_over(){
    state->game_state = false;
    state->running = false;
}


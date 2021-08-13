#include "game_state.h"
#include "queue.h"

void game_state_init(struct Game_state* state) {

    state->player_animation = (struct Screen_coord){ 0, 0 };
    state->player_ptr = (struct Cell_pos){ 0, 0 };
    state->pending_movements = queue_create();
}

void game_state_destroy(struct Game_state* state) {
    queue_destroy(state->pending_movements);
}

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "queue.h"

struct Screen_coord {
    int x;
    int y;
};

struct Cell_pos {
    int x;
    int y;
};

struct Game_state {

    struct Screen_coord player_animation;
    struct Cell_pos player_ptr;
    struct Queue* pending_movements;
};

void game_state_init(struct Game_state* state);

void game_state_destroy(struct Game_state* state);

#endif

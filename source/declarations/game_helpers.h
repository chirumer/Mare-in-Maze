#ifndef GAME_HELPERS_H
#define GAME_HELPERS_H


#include <stdbool.h>
#include "maze.h"
#include "game_state.h"

enum Direction {
    LEFT, RIGHT, UP, DOWN
};

void display_loading(SDL_Renderer* renderer);

void move_player(enum Direction direc, struct Maze maze,
                 struct Game_state* game_state);

enum Blockable {
    BLOCKABLE_1=0, BLOCKABLE_2, BLOCKABLE_3, BLOCKABLE_4,
    NO_BLOCKABLES
};

bool is_blocked(enum Blockable);
void set_blocked(enum Blockable);
void unset_blocked(enum Blockable);


#endif

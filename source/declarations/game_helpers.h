#ifndef GAME_HELPERS_H
#define GAME_HELPERS_H


#include "maze.h"
#include "game_state.h"

enum Direction {
    LEFT, RIGHT, UP, DOWN
};

void display_loading(SDL_Renderer* renderer);

void move_player(enum Direction direc, struct Maze maze,
                 struct Game_state* game_state);


#endif

#ifndef GAME_GRAPHICS_H
#define GAME_GRAPHICS_H

#include <SDL2/SDL.h>
#include "maze.h"
#include "game_state.h"


void graphics_init(SDL_Renderer* renderer);

void graphics_destroy(void);

void graphics_render(SDL_Renderer* renderer, struct Maze maze, 
                     struct Game_state game_state, int frame_rate);

void modify_player_animation(struct Game_state* game_state);


#endif

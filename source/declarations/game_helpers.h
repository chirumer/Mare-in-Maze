#ifndef GAME_HELPERS_H
#define GAME_HELPERS_H

#include <SDL2/SDL.h>

void init(SDL_Renderer* renderer);
// initialize game assets

void render_maze_cell(SDL_Renderer* renderer, SDL_Rect dstrect, enum Maze_tile cell);
// renders a maze cell

void render_void_cell(SDL_Renderer* renderer, SDL_Rect dstrect);
// renders a void cell

void debug_display_maze(struct Maze maze);
// DEBUG

#endif

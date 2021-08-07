#ifndef GAME_HELPERS_H
#define GAME_HELPERS_H

#include <SDL2/SDL.h>
#include "queue.h"

struct Coord {
    int x;
    int y;
};

struct Movement {
    struct Coord dest;
    unsigned int reach_in;
};

void init_resources(SDL_Renderer* renderer);
// initialize game assets

void destroy_resources();
// destroy game assets

void render_maze(SDL_Renderer* renderer, struct Maze maze, struct Coord player);
// renders the maze

void render_player_ptr(SDL_Renderer* renderer, struct Coord player, struct Coord ptr);
// renders player_ptr

void move_player_animation(SDL_Renderer* renderer, struct Coord* animation_pos, struct Queue* movements);
// moves player animation

void render_player(SDL_Renderer* renderer);
// renders player

void render_maze_cell(SDL_Renderer* renderer, SDL_Rect dstrect, enum Maze_tile cell);
// renders a maze cell

void render_void_cell(SDL_Renderer* renderer, SDL_Rect dstrect);
// renders a void cell

void debug_display_maze(struct Maze maze);
// DEBUG

#endif

#include <SDL2/SDL.h>
#include "game_helpers.h"
#include "game_state.h"
#include "queue.h"
#include "constants.h"

void display_loading(SDL_Renderer* renderer) {

    // make window white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void move_player(enum Direction direc, struct Maze maze,
                 struct Game_state* state) { 

    struct Cell_pos delta[4];
    delta[LEFT] = (struct Cell_pos){ -1, 0 };
    delta[RIGHT] = (struct Cell_pos){ 1, 0 };
    delta[UP] = (struct Cell_pos){ 0, -1 };
    delta[DOWN] = (struct Cell_pos){ 0, 1 };

    struct Cell_pos new_pos = { .x = state->player_ptr.x + delta[direc].x,
                                .y = state->player_ptr.y + delta[direc].y };

    // boundary condition
    if (new_pos.x < 0 || new_pos.y < 0 || new_pos.x >= MAZE_WIDTH ||
        new_pos.y >= MAZE_HEIGHT) {

        return;
    }

    if (maze.cells[new_pos.y][new_pos.x] != MAZE_WALL) {

        struct Cell_pos* node = malloc(sizeof(struct Cell_pos));
        *node = new_pos;
        queue_push(state->pending_movements, node);
        state->player_ptr = new_pos;
    }
}

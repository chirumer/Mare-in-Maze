#include <stdbool.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "SDL_helpers.h"
#include "maze.h"
#include "parameters.h"
#include "game_helpers.h"

struct Coord {
    int x;
    int y;
};

void render_maze(SDL_Renderer* renderer, struct Maze maze, struct Coord player) {

    SDL_SetRenderDrawColor(renderer, 100,100,100,255);
    SDL_RenderFillRect(renderer, NULL);

    struct Coord offset = { .x = player.x%TILE_SIZE, .y = player.y%TILE_SIZE };
    struct Coord a_tile_no = { .x = player.x/TILE_SIZE, .y = player.y/TILE_SIZE };
    struct Coord a_tile_coord = { .x = SCREEN_WIDTH/2 - TILE_SIZE/2 - offset.x,
                                  .y = SCREEN_HEIGHT/2 - TILE_SIZE/2 - offset.y };

    struct Coord maze_topleft = { .x = a_tile_coord.x - a_tile_no.x * TILE_SIZE,
                                  .y = a_tile_coord.y - a_tile_no.y * TILE_SIZE };
    struct Coord maze_bottomright = { .x = maze_topleft.x + MAZE_WIDTH * TILE_SIZE,
                                      .y = maze_topleft.y + MAZE_HEIGHT * TILE_SIZE };

    int left = a_tile_coord.x - ((a_tile_coord.x - 0)/TILE_SIZE)*TILE_SIZE
                - ((a_tile_coord.x - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int top = a_tile_coord.y - ((a_tile_coord.y - 0)/TILE_SIZE)*TILE_SIZE
               - ((a_tile_coord.y - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int right = a_tile_coord.x + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                 + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int bottom = a_tile_coord.y + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                  + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);

    for (int y = top; y <= bottom; y += 50) {
        for (int x = left; x <= right; x += 50) {

            SDL_Rect tile = { .x = x, .y = y, .w = TILE_SIZE, .h = TILE_SIZE };

            if (x >= maze_topleft.x && y >= maze_topleft.y
                && x <= maze_bottomright.x && y <= maze_bottomright.y) {

                int j = ((y-top) - (maze_topleft.y - top))/TILE_SIZE;
                int i = ((x-left) - (maze_topleft.x - left))/TILE_SIZE;

                render_maze_cell(renderer, tile, maze.cells[j][i]);
            }
            else {
                render_void_cell(renderer, tile);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

bool start_game(SDL_Renderer* renderer) {

    bool is_exit = false;

    init(renderer);

    struct Maze maze;
    create_maze(&maze, MAZE_WIDTH, MAZE_HEIGHT);

    debug_display_maze(maze);

    struct Coord player_animation = { 130, 100 };
    //struct Coord player_ptr = { 0, 0 };

    render_maze(renderer, maze, player_animation);
    //render_player_ptr(player_ptr);
    //render_player();

    free_maze(&maze);

    // DEBUG
    SDL_Delay(20000000);

    return !is_exit;
}

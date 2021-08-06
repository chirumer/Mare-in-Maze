#include <stdbool.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "SDL_helpers.h"
#include "maze.h"
#include "parameters.h"
#include "game_helpers.h"

bool start_game(SDL_Renderer* renderer) {

    bool is_exit = false;

    init(renderer);

    struct Maze maze;
    create_maze(&maze, MAZE_WIDTH, MAZE_HEIGHT);
    debug_display_maze(maze);

    struct Coord player_animation = { 0, 0 };
    //struct Coord player_ptr = { 0, 0 };

    render_maze(renderer, maze, player_animation);
    //render_player_ptr(player_ptr);
    //render_player();

    free_maze(&maze);

    // DEBUG
    SDL_Delay(20000000);

    return !is_exit;
}

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "maze.h"
#include "parameters.h"

void debug_display_maze(struct Maze maze) {

    const char* wall = "██";
    const char* path = "  ";

    for (int i = 0; i < MAZE_WIDTH + 2; ++i) {
        fprintf(stderr, "%s", wall);
    }
    fprintf(stderr, "\n");
    for (int i = 0; i < MAZE_HEIGHT; ++i) {
        fprintf(stderr, "%s", wall);
        for (int j = 0; j < MAZE_WIDTH; ++j) {
            
            if (maze.cells[i][j] == MAZE_WALL) {
                fprintf(stderr, "%s", wall);
            }
            else if (maze.cells[i][j] == MAZE_PATH) {
                fprintf(stderr, "%s", path);
            }
            else if (maze.cells[i][j] == MAZE_START) {
                fprintf(stderr, "S ");
            }
            else if (maze.cells[i][j] == MAZE_END) {
                fprintf(stderr, " E");
            }
        }
        fprintf(stderr, "%s", wall);
        fprintf(stderr, "\n");
    }
    for (int i = 0; i < MAZE_WIDTH + 2; ++i) {
        fprintf(stderr, "%s", wall);
    }
    fprintf(stderr, "\n");
}

bool start_game(SDL_Renderer* renderer) {

    bool is_exit = false;

    struct Maze maze;
    create_maze(&maze, MAZE_WIDTH, MAZE_HEIGHT);

    debug_display_maze(maze);

    free_maze(&maze);

    return !is_exit;
}

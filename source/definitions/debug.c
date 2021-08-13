#include <stdio.h> 

#include "debug.h"
#include "constants.h"

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

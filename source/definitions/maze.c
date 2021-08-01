#include "maze.h"

void create_maze(struct Maze* maze, int width, int height) {

    // set dimensions
    maze->width = width;
    maze->height = height;

    // default all cells to MAZE_WALL
    maze->cells = malloc( sizeof(enum Maze_tile*) * height );
    for (int i = 0; i < height; ++i) {
        maze->cells[i] = malloc( sizeof(enum Maze_tile) * width);
        for (int j = 0; j < width; ++j) {
            maze->cells[i][j] = MAZE_WALL;
        }
    }

    maze->cells[0][0] = MAZE_START;
    maze->cells[height-1][width-1] = MAZE_END;
}

void free_maze(struct Maze* maze) {

    for (int i = 0; i < maze->height; ++i) {
        free(maze->cells[i]);
    }
    free(maze->cells);
}

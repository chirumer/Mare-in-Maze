#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "maze.h"

//------------ Internal declarations -----------

struct Coord {
    int x;
    int y;
};

void shuffle(struct Coord arr[], int size);
    // shuffle integer array (using fischer-yates)

bool is_pathable(struct Maze* maze, struct Coord pos);
    // check if tile can be made a path

void maze_explore(struct Maze* maze, struct Coord pos);
    // generatge paths (using depth-first search)


//--------------- Interface function definitions ----------------

void create_maze(struct Maze* maze, int width, int height) {

    assert(width % 2 && height % 2);

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

    // explore paths into the maze
    maze_explore(maze, (struct Coord){0, 0});

    // set end-points
    maze->cells[0][0] = MAZE_START;
    maze->cells[height-1][width-1] = MAZE_END;
}

void free_maze(struct Maze* maze) {

    for (int i = 0; i < maze->height; ++i) {
        free(maze->cells[i]);
    }
    free(maze->cells);
}

//------------- Internal functions --------------

void maze_explore(struct Maze* maze, struct Coord pos) {

    // mark as path
    maze->cells[pos.y][pos.x] = MAZE_PATH;

    // don't explore past END
    if (pos.x == maze->width-1 && pos.y == maze->height-1) {
        return;
    }

    struct Coord incr[] = { { -1, 0 },
                            { 1, 0 },
                            { 0, -1 },
                            { 0, 1 } };
    shuffle(incr, 4);

    // visit neighbors
    int no_neighbors = 0;
    for (int i = 0; i < 4; ++i) {

        // neighbor coordinate
        struct Coord neighbor;
        neighbor.x = pos.x + incr[i].x;
        neighbor.y = pos.y + incr[i].y;

        if (is_pathable(maze, neighbor)) {
            maze_explore(maze, neighbor);
        }
    }
}

bool is_pathable(struct Maze* maze, struct Coord pos) {

    // bounds check
    if (pos.x < 0 || pos.y < 0 || pos.x >= maze->width
        || pos.y >= maze->height) {
        return false;
    }

    // to avoid dead spaces
    if (pos.x % 2 == 1 && pos.y % 2 == 1) {
        return false;
    }

    // find neighbors
    int no_neighbors = 0;
    struct Coord incr[] = { { -1, 0 },
                            { 1, 0 },
                            { 0, -1 },
                            { 0, 1 } };
    for (int i = 0; i < 4; ++i) {

        struct Coord neighbor;
        neighbor.x = pos.x + incr[i].x;
        neighbor.y = pos.y + incr[i].y;

        // bounds check
        if (neighbor.x < 0 || neighbor.y < 0 ||
            neighbor.x >= maze->width || neighbor.y >= maze->height) {
            continue;
        }

        if (maze->cells[neighbor.y][neighbor.x] == MAZE_PATH) {
            ++no_neighbors;
        }
    }

    // don't collide with other paths
    if (no_neighbors > 1) {
        return false;
    }

    return true;
}

void shuffle(struct Coord arr[], int size) {

    for (int i = size-1; i > 0; --i) {

        int chosen = rand() % (i+1);
            // 0 to i (including)

        // swap
        struct Coord temp = arr[chosen];
        arr[chosen] = arr[i];
        arr[i] = temp;
    }
}

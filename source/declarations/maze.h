#ifndef MAZE_H
#define MAZE_H

enum Maze_tile {
    MAZE_WALL, MAZE_PATH, MAZE_START, MAZE_END
};

struct Maze {
    int width, height;
    enum Maze_tile** cells;
};

void create_maze(struct Maze* maze, int width, int height);
    // create maze of given dimensions

void free_maze(struct Maze* maze);
    // free memory allocated for the maze

#endif

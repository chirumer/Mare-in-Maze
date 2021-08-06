#include <SDL2/SDL.h>
#include "maze.h"
#include "SDL_helpers.h"
#include "parameters.h"
#include "game_helpers.h"

SDL_Texture* start_texture;
SDL_Texture* end_texture;
SDL_Texture* wall_texture;
SDL_Texture* path_texture;
SDL_Texture* void_texture;

void init_resources(SDL_Renderer* renderer) {
    wall_texture = load_texture(WALL_IMAGE_PATH, renderer);
    path_texture = load_texture(PATH_IMAGE_PATH, renderer);
    start_texture = load_texture(START_IMAGE_PATH, renderer);
    end_texture = load_texture(END_IMAGE_PATH, renderer);
    void_texture = load_texture(VOID_IMAGE_PATH, renderer);
}

void destroy_resources() {
    SDL_DestroyTexture(wall_texture);
    SDL_DestroyTexture(path_texture);
    SDL_DestroyTexture(start_texture);
    SDL_DestroyTexture(end_texture);
    SDL_DestroyTexture(void_texture);
}

void render_maze_cell(SDL_Renderer* renderer, SDL_Rect dstrect, enum Maze_tile cell) {

    SDL_Rect srcrect = { .x = 0, .y = 0, .w = TILE_SIZE, .h = TILE_SIZE };

    if (dstrect.x < 0) {
        srcrect.x = 0 + (0 - dstrect.x);
        srcrect.w = TILE_SIZE - srcrect.x;
        dstrect.x = 0;
    }
    else if (dstrect.x + TILE_SIZE > SCREEN_WIDTH) {
        srcrect.w = TILE_SIZE - (SCREEN_WIDTH - (dstrect.x + TILE_SIZE)); 
        dstrect.w = srcrect.w;
    }
    if (dstrect.y < 0) {
        srcrect.y = 0 + (0 - dstrect.y);
        srcrect.h = TILE_SIZE - srcrect.y;
        dstrect.y = 0;
    }
    else if (dstrect.y + TILE_SIZE > SCREEN_HEIGHT) {
        srcrect.h = TILE_SIZE - (SCREEN_WIDTH - (dstrect.y + TILE_SIZE));
        dstrect.h = dstrect.h;
    }

    if (cell == MAZE_PATH) {
        SDL_RenderCopy(renderer, path_texture, &srcrect, &dstrect);
    }
    else if (cell == MAZE_WALL) {
        SDL_RenderCopy(renderer, wall_texture, &srcrect, &dstrect);
    }
    else if (cell == MAZE_START) {
        SDL_RenderCopy(renderer, start_texture, &srcrect, &dstrect);
    }
    else if (cell == MAZE_END) {
        SDL_RenderCopy(renderer, end_texture, &srcrect, &dstrect);
    }

}

void render_void_cell(SDL_Renderer* renderer, SDL_Rect dstrect) {

    SDL_Rect srcrect = { .x = 0, .y = 0, .w = TILE_SIZE, .h = TILE_SIZE };

    if (dstrect.x < 0) {
        srcrect.x = 0 + (0 - dstrect.x);
        srcrect.w = TILE_SIZE - srcrect.x;
        dstrect.x = 0;
    }
    else if (dstrect.x + TILE_SIZE > SCREEN_WIDTH) {
        srcrect.w = TILE_SIZE - (SCREEN_WIDTH - (dstrect.x + TILE_SIZE)); 
        dstrect.w = srcrect.w;
    }
    if (dstrect.y < 0) {
        srcrect.y = 0 + (0 - dstrect.y);
        srcrect.h = TILE_SIZE - srcrect.y;
        dstrect.y = 0;
    }
    else if (dstrect.y + TILE_SIZE > SCREEN_HEIGHT) {
        srcrect.h = TILE_SIZE - (SCREEN_WIDTH - (dstrect.y + TILE_SIZE));
        dstrect.h = dstrect.h;
    }

    SDL_RenderCopy(renderer, void_texture, &srcrect, &dstrect);
}


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

void render_maze(SDL_Renderer* renderer, struct Maze maze, struct Coord player) {

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

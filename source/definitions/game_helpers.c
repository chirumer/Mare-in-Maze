#include <SDL2/SDL.h>
#include "maze.h"
#include "SDL_helpers.h"
#include "parameters.h"

SDL_Texture* start_texture;
SDL_Texture* end_texture;
SDL_Texture* wall_texture;
SDL_Texture* path_texture;
SDL_Texture* void_texture;

void init(SDL_Renderer* renderer) {
    wall_texture = load_texture(WALL_IMAGE_PATH, renderer);
    path_texture = load_texture(PATH_IMAGE_PATH, renderer);
    start_texture = load_texture(START_IMAGE_PATH, renderer);
    end_texture = load_texture(END_IMAGE_PATH, renderer);
    void_texture = load_texture(VOID_IMAGE_PATH, renderer);
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

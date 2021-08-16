#include <SDL2/SDL.h>
#include "game_graphics.h"
#include "SDL_helpers.h"
#include "queue.h"
#include "game_state.h"
#include "constants.h"
#include "math.h"
#include "resource_paths.h"


// -------- internal ---------

enum BASE_TILE {
    GRASS=0, GROUND_TOPLEFT, GROUND_TOP, GROUND_TOPRIGHT, GROUND_LEFT,
    GROUND_CENTER, GROUND_RIGHT, GROUND_BOTTOMLEFT, GROUND_BOTTOM,
    GROUND_BOTTOMRIGHT, GOAL, NO_BASE_TILES
};
SDL_Texture* base_tiles[NO_BASE_TILES];

enum FENCE_TILE {
    FENCE_SINGLE=0, FENCE_HORIZONTAL, FENCE_VERTICAL, NO_FENCES
};
SDL_Texture* fences[NO_FENCES];

enum HORSE1_FRAME {
    HORSE1_DOWN_1=0, HORSE1_DOWN_2, HORSE1_DOWN_3,
    HORSE1_LEFT_1, HORSE1_LEFT_2, HORSE1_LEFT_3,
    HORSE1_RIGHT_1, HORSE1_RIGHT_2, HORSE1_RIGHT_3,
    HORSE1_UP_1, HORSE1_UP_2, HORSE1_UP_3,
    NO_HORSE1_FRAMES
};
SDL_Texture* horse1_frames[NO_HORSE1_FRAMES];

struct Animation_state {

    int current_frame_no;
    int no_frames;
    SDL_Texture** frames;

};

const int DEFAULT_FRAME = 1;

enum Direction {
    LEFT=0, RIGHT, UP, DOWN, NO_DIRECTIONS
};

SDL_Texture* horse1_frame_groups[NO_DIRECTIONS][4];
struct Animation_state animation_state_player; 



void render_cell(SDL_Renderer* renderer, SDL_Rect dstrect, SDL_Texture* texture) {

    SDL_Rect srcrect = { .x = 0, .y = 0, .w = TILE_SIZE, .h = TILE_SIZE };

    if (dstrect.x < 0) {
        srcrect.x = 0 + (0 - dstrect.x);
        srcrect.w = TILE_SIZE - srcrect.x;
        dstrect.w = srcrect.w;
        dstrect.x = 0;
    }
    else if (dstrect.x + TILE_SIZE > SCREEN_WIDTH) {
        srcrect.w = TILE_SIZE - ((dstrect.x + TILE_SIZE) - SCREEN_WIDTH); 
        dstrect.w = srcrect.w;
    }

    if (dstrect.y < 0) {
        srcrect.y = 0 + (0 - dstrect.y);
        srcrect.h = TILE_SIZE - srcrect.y;
        dstrect.h = srcrect.h;
        dstrect.y = 0;
    }
    else if (dstrect.y + TILE_SIZE > SCREEN_HEIGHT) {
        srcrect.h = TILE_SIZE - ((dstrect.y + TILE_SIZE) - SCREEN_HEIGHT);
        dstrect.h = srcrect.h;
    }

    SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
}

void render_base(SDL_Renderer* renderer, struct Screen_coord player) {

    struct Screen_coord offset = { .x = player.x%TILE_SIZE, .y = player.y%TILE_SIZE };
    struct Cell_pos a_tile_no = { .x = player.x/TILE_SIZE, .y = player.y/TILE_SIZE };
    struct Screen_coord a_tile_coord = { .x = SCREEN_WIDTH/2 - TILE_SIZE/2 - offset.x,
                                  .y = SCREEN_HEIGHT/2 - TILE_SIZE/2 - offset.y };

    struct Screen_coord maze_topleft = { .x = a_tile_coord.x - a_tile_no.x * TILE_SIZE,
                                  .y = a_tile_coord.y - a_tile_no.y * TILE_SIZE };
    struct Screen_coord maze_bottomright = { .x = maze_topleft.x + (MAZE_WIDTH-1) * TILE_SIZE,
                                      .y = maze_topleft.y + (MAZE_HEIGHT-1) * TILE_SIZE };

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


            if (x == maze_topleft.x - 50 && y == maze_topleft.y - 50) {
                render_cell(renderer, tile, base_tiles[GROUND_TOPLEFT]);
            }
            else if (x == maze_bottomright.x + 50 && y == maze_topleft.y - 50) {
                render_cell(renderer, tile, base_tiles[GROUND_TOPRIGHT]);
            }
            else if (y == maze_topleft.y - 50 && x >= maze_topleft.x - 50 && x <= maze_bottomright.x + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_TOP]);
            }
            else if (x == maze_bottomright.x + 50 && y == maze_bottomright.y + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_BOTTOMRIGHT]);
            }
            else if (x == maze_topleft.x - 50 && y == maze_bottomright.y + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_BOTTOMLEFT]);
            }
            else if (y == maze_bottomright.y + 50 && x >= maze_topleft.x - 50 && x <= maze_bottomright.x + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_BOTTOM]);
            }
            else if (x == maze_bottomright.x + 50 && y >= maze_topleft.y - 50 && y <= maze_bottomright.y + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_RIGHT]);
            }
            else if (x == maze_topleft.x - 50 && y >= maze_topleft.y - 50 && y <= maze_bottomright.y + 50) {
                render_cell(renderer, tile, base_tiles[GROUND_LEFT]);
            }
            else if (x >= maze_topleft.x && y >= maze_topleft.y
                && x <= maze_bottomright.x && y <= maze_bottomright.y) {
                render_cell(renderer, tile, base_tiles[GROUND_CENTER]);
            }
            else {
                render_cell(renderer, tile, base_tiles[GRASS]);
            }
        }
    }
}

void render_fences(SDL_Renderer* renderer, struct Maze maze, struct Screen_coord player) {

    struct Screen_coord offset = { .x = player.x%TILE_SIZE, .y = player.y%TILE_SIZE };
    struct Cell_pos a_tile_no = { .x = player.x/TILE_SIZE, .y = player.y/TILE_SIZE };
    struct Screen_coord a_tile_coord = { .x = SCREEN_WIDTH/2 - TILE_SIZE/2 - offset.x,
                                  .y = SCREEN_HEIGHT/2 - TILE_SIZE/2 - offset.y };

    struct Screen_coord maze_topleft = { .x = a_tile_coord.x - a_tile_no.x * TILE_SIZE,
                                  .y = a_tile_coord.y - a_tile_no.y * TILE_SIZE };
    struct Screen_coord maze_bottomright = { .x = maze_topleft.x + (MAZE_WIDTH-1) * TILE_SIZE,
                                      .y = maze_topleft.y + (MAZE_HEIGHT-1) * TILE_SIZE };

    int left = a_tile_coord.x - ((a_tile_coord.x - 0)/TILE_SIZE)*TILE_SIZE
                - ((a_tile_coord.x - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int top = a_tile_coord.y - ((a_tile_coord.y - 0)/TILE_SIZE)*TILE_SIZE
               - ((a_tile_coord.y - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int right = a_tile_coord.x + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                 + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int bottom = a_tile_coord.y + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                  + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);

    // because fences are 2 tiles wide
    left -= 50;
    right += 50;
    top -= 50;
    bottom += 50;

    for (int y = top; y <= bottom; y += 50) {
        for (int x = left; x <= right; x += 50) {

            SDL_Rect tile = { .x = x, .y = y, .w = TILE_SIZE, .h = TILE_SIZE };

            if (x == maze_topleft.x - 50 && y == maze_topleft.y - 50) {
                tile.w *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                tile.w /= 2;
                tile.h *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                tile.h /= 2;
            }
            else if (x == maze_bottomright.x + 50 && y == maze_topleft.y - 50) {
                tile.h *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                tile.h /= 2;
            }
            else if (y == maze_topleft.y - 50 && x >= maze_topleft.x - 50 && x <= maze_bottomright.x + 50) {
                tile.w *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                tile.w /= 2;

                int j = ((y-top) - (maze_topleft.y - top))/TILE_SIZE;
                int i = ((x-left) - (maze_topleft.x - left))/TILE_SIZE;
                if (maze.cells[j+1][i] == MAZE_WALL) {
                    tile.h *= 2;
                    SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                    tile.h /= 2;
                }
            }
            else if (x == maze_bottomright.x + 50 && y == maze_bottomright.y + 50) {
            }
            else if (x == maze_topleft.x - 50 && y == maze_bottomright.y + 50) {

                tile.w *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                tile.w /= 2;
            }
            else if (y == maze_bottomright.y + 50 && x >= maze_topleft.x - 50 && x <= maze_bottomright.x + 50) {

                tile.w *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                tile.w /= 2;
            }
            else if (x == maze_bottomright.x + 50 && y >= maze_topleft.y && y <= maze_bottomright.y) {

                tile.h *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                tile.h /= 2;
            }
            else if (x == maze_topleft.x - 50 && y >= maze_topleft.y && y <= maze_bottomright.y) {

                int j = ((y-top) - (maze_topleft.y - top))/TILE_SIZE;
                int i = ((x-left) - (maze_topleft.x - left))/TILE_SIZE;
                if (maze.cells[j][i+1] == MAZE_WALL) {
                    tile.w *= 2;
                    SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                    tile.w /= 2;
                }
                tile.h *= 2;
                SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                tile.h /= 2;
            }


            if (x >= maze_topleft.x && y >= maze_topleft.y
                && x <= maze_bottomright.x && y <= maze_bottomright.y) {

                int j = ((y-top) - (maze_topleft.y - top))/TILE_SIZE;
                int i = ((x-left) - (maze_topleft.x - left))/TILE_SIZE;

                if (maze.cells[j][i] == MAZE_WALL) {

                    if ((i + 1 < MAZE_WIDTH && maze.cells[j][i+1] == MAZE_WALL) || i + 1 == MAZE_WIDTH) {

                        tile.w *= 2;
                        SDL_RenderCopy(renderer, fences[FENCE_HORIZONTAL], NULL, &tile);
                        tile.w /= 2;
                    }
                    if ((j + 1 < MAZE_HEIGHT && maze.cells[j+1][i] == MAZE_WALL) || j + 1 == MAZE_HEIGHT) {

                        tile.h *= 2;
                        SDL_RenderCopy(renderer, fences[FENCE_VERTICAL], NULL, &tile);
                        tile.h /= 2;
                    }

                    if ( !(i + 1 < MAZE_WIDTH && maze.cells[j][i+1] == MAZE_WALL) && 
                         !(j + 1 < MAZE_HEIGHT && maze.cells[j+1][i] == MAZE_WALL) && 
                         !(i+1 == MAZE_WIDTH) && !(j+1 == MAZE_HEIGHT) ) {

                        SDL_RenderCopy(renderer, fences[FENCE_SINGLE], NULL, &tile);
                    }
                }
            }
        }
    }
}

void render_others(SDL_Renderer* renderer, struct Screen_coord player) {

    struct Screen_coord offset = { .x = player.x%TILE_SIZE, .y = player.y%TILE_SIZE };
    struct Cell_pos a_tile_no = { .x = player.x/TILE_SIZE, .y = player.y/TILE_SIZE };
    struct Screen_coord a_tile_coord = { .x = SCREEN_WIDTH/2 - TILE_SIZE/2 - offset.x,
                                  .y = SCREEN_HEIGHT/2 - TILE_SIZE/2 - offset.y };

    struct Screen_coord maze_topleft = { .x = a_tile_coord.x - a_tile_no.x * TILE_SIZE,
                                  .y = a_tile_coord.y - a_tile_no.y * TILE_SIZE };
    struct Screen_coord maze_bottomright = { .x = maze_topleft.x + (MAZE_WIDTH-1) * TILE_SIZE,
                                      .y = maze_topleft.y + (MAZE_HEIGHT-1) * TILE_SIZE };

    int left = a_tile_coord.x - ((a_tile_coord.x - 0)/TILE_SIZE)*TILE_SIZE
                - ((a_tile_coord.x - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int top = a_tile_coord.y - ((a_tile_coord.y - 0)/TILE_SIZE)*TILE_SIZE
               - ((a_tile_coord.y - 0)%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int right = a_tile_coord.x + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                 + ((SCREEN_WIDTH - (a_tile_coord.x + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);
    int bottom = a_tile_coord.y + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))/TILE_SIZE)*TILE_SIZE
                  + ((SCREEN_HEIGHT - (a_tile_coord.y + TILE_SIZE))%TILE_SIZE != 0 ? TILE_SIZE : 0);

    // render goal
    int x = maze_bottomright.x;
    int y = maze_bottomright.y;

    if (x >= left && x <= right &&
        y >= top && y <= bottom) {

        SDL_Rect tile = { .x = x, .y = y, .w = TILE_SIZE, .h = TILE_SIZE };
        render_cell(renderer, tile, base_tiles[GOAL]);
    }
}

void render_player(SDL_Renderer* renderer) {

    SDL_Rect dstrect = { .x = SCREEN_WIDTH/2 - TILE_SIZE/2, 
                         .y = SCREEN_HEIGHT/2 - TILE_SIZE/2, 
                         .w = TILE_SIZE, .h = TILE_SIZE };
    SDL_Texture* player = animation_state_player.frames[animation_state_player.current_frame_no];
    SDL_RenderCopy(renderer, player, NULL, &dstrect);
}

int walk_towards(struct Screen_coord* player, struct Screen_coord dest, int stamina) {

    int distance_travelled;

    if (player->x != dest.x) {
        distance_travelled = stamina <= abs(dest.x - player->x) ? stamina :
                                                                 abs(dest.x - player->x);
        if (dest.x > player->x) {
            player->x += distance_travelled;
        }
        else {
            player->x -= distance_travelled;
        }
    }
    else if (player->y != dest.y) {
        distance_travelled = stamina <= abs(dest.y - player->y) ? stamina :
                                                                 abs(dest.y - player->y);
            if (dest.y > player->y) {
                player->y += distance_travelled;
            }
            else {
                player->y -= distance_travelled;
            }
    }

    return stamina - distance_travelled;
}

enum Direction get_direction(struct Screen_coord player, struct Screen_coord dest) {

    if (dest.x > player.x) {
        return RIGHT;
    }
    else if (dest.x < player.x) {
        return LEFT;
    }
    else if (dest.y > player.y) {
        return DOWN;
    }
    else if (dest.y < player.y) {
        return UP;
    }
    return -1;
}


// -------- export ---------

void graphics_init(SDL_Renderer* renderer) {

    base_tiles[GRASS] = load_texture(PATH_IMAGE_GRASS, renderer);
    base_tiles[GROUND_TOPLEFT] = load_texture(PATH_IMAGE_GROUND_TOPLEFT, renderer);
    base_tiles[GROUND_TOP] = load_texture(PATH_IMAGE_GROUND_TOP, renderer);
    base_tiles[GROUND_TOPRIGHT] = load_texture(PATH_IMAGE_GROUND_TOPRIGHT, renderer);
    base_tiles[GROUND_LEFT] = load_texture(PATH_IMAGE_GROUND_LEFT, renderer);
    base_tiles[GROUND_CENTER] = load_texture(PATH_IMAGE_GROUND_CENTER, renderer);
    base_tiles[GROUND_RIGHT] = load_texture(PATH_IMAGE_GROUND_RIGHT, renderer);
    base_tiles[GROUND_CENTER] = load_texture(PATH_IMAGE_GROUND_CENTER, renderer);
    base_tiles[GROUND_RIGHT] = load_texture(PATH_IMAGE_GROUND_RIGHT, renderer);
    base_tiles[GROUND_BOTTOMLEFT] = load_texture(PATH_IMAGE_GROUND_BOTTOMLEFT, renderer);
    base_tiles[GROUND_BOTTOM] = load_texture(PATH_IMAGE_GROUND_BOTTOM, renderer);
    base_tiles[GROUND_BOTTOMRIGHT] = load_texture(PATH_IMAGE_GROUND_BOTTOMRIGHT, renderer);
    base_tiles[GOAL] = load_texture(PATH_IMAGE_HAY, renderer);

    fences[FENCE_SINGLE] = load_texture(PATH_IMAGE_FENCE_SINGLE, renderer);
    fences[FENCE_HORIZONTAL] = load_texture(PATH_IMAGE_FENCE_HORIZONTAL, renderer);
    fences[FENCE_VERTICAL] = load_texture(PATH_IMAGE_FENCE_VERTICAL, renderer);

    horse1_frames[HORSE1_DOWN_1] = load_texture(PATH_IMAGE_HORSE1_DOWN_1, renderer);
    horse1_frames[HORSE1_DOWN_2] = load_texture(PATH_IMAGE_HORSE1_DOWN_2, renderer);
    horse1_frames[HORSE1_DOWN_3] = load_texture(PATH_IMAGE_HORSE1_DOWN_3, renderer);
    horse1_frames[HORSE1_LEFT_1] = load_texture(PATH_IMAGE_HORSE1_LEFT_1, renderer);
    horse1_frames[HORSE1_LEFT_2] = load_texture(PATH_IMAGE_HORSE1_LEFT_2, renderer);
    horse1_frames[HORSE1_LEFT_3] = load_texture(PATH_IMAGE_HORSE1_LEFT_3, renderer);
    horse1_frames[HORSE1_RIGHT_1] = load_texture(PATH_IMAGE_HORSE1_RIGHT_1, renderer);
    horse1_frames[HORSE1_RIGHT_2] = load_texture(PATH_IMAGE_HORSE1_RIGHT_2, renderer);
    horse1_frames[HORSE1_RIGHT_3] = load_texture(PATH_IMAGE_HORSE1_RIGHT_3, renderer);
    horse1_frames[HORSE1_UP_1] = load_texture(PATH_IMAGE_HORSE1_UP_1, renderer);
    horse1_frames[HORSE1_UP_2] = load_texture(PATH_IMAGE_HORSE1_UP_2, renderer);
    horse1_frames[HORSE1_UP_3] = load_texture(PATH_IMAGE_HORSE1_UP_3, renderer);



    horse1_frame_groups[LEFT][0] = horse1_frames[HORSE1_LEFT_1];
    horse1_frame_groups[LEFT][1] = horse1_frames[HORSE1_LEFT_2];
    horse1_frame_groups[LEFT][2] = horse1_frames[HORSE1_LEFT_3];
    horse1_frame_groups[LEFT][3] = horse1_frames[HORSE1_LEFT_2];

    horse1_frame_groups[RIGHT][0] = horse1_frames[HORSE1_RIGHT_1];
    horse1_frame_groups[RIGHT][1] = horse1_frames[HORSE1_RIGHT_2];
    horse1_frame_groups[RIGHT][2] = horse1_frames[HORSE1_RIGHT_3];
    horse1_frame_groups[RIGHT][3] = horse1_frames[HORSE1_RIGHT_2];

    horse1_frame_groups[UP][0] = horse1_frames[HORSE1_UP_1];
    horse1_frame_groups[UP][1] = horse1_frames[HORSE1_UP_2];
    horse1_frame_groups[UP][2] = horse1_frames[HORSE1_UP_3];
    horse1_frame_groups[UP][3] = horse1_frames[HORSE1_UP_2];

    horse1_frame_groups[DOWN][0] = horse1_frames[HORSE1_DOWN_1];
    horse1_frame_groups[DOWN][1] = horse1_frames[HORSE1_DOWN_2];
    horse1_frame_groups[DOWN][2] = horse1_frames[HORSE1_DOWN_3];
    horse1_frame_groups[DOWN][3] = horse1_frames[HORSE1_DOWN_2];

    animation_state_player = (struct Animation_state){ .current_frame_no = 1, .no_frames = 4,
                               .frames = horse1_frame_groups[RIGHT] };
}

void graphics_destroy(void) { 
    for (int i = 0; i < NO_BASE_TILES; ++i) {
        SDL_DestroyTexture(base_tiles[i]);
    }
    for (int i = 0; i < NO_FENCES; ++i) {
        SDL_DestroyTexture(base_tiles[i]);
    }
    for (int i = 0; i < NO_HORSE1_FRAMES; ++i) {
        SDL_DestroyTexture(horse1_frames[i]);
    }
}

void graphics_render(SDL_Renderer* renderer, struct Maze maze, 
                     struct Game_state game_state, int frame_rate) { 

    // cap frame rate
    static unsigned int last_render = 0;
    if ((SDL_GetTicks() - last_render) <= 1000/frame_rate) {
        return;
    }
    last_render = SDL_GetTicks();

    // rerender only if required
    static struct Screen_coord prev_animation_pos = { -1, -1 };
    static struct Animation_state prev_animation_state;
    if (prev_animation_pos.x == game_state.player_animation.x &&
        prev_animation_pos.y == game_state.player_animation.y &&
        prev_animation_state.current_frame_no == 
        animation_state_player.current_frame_no &&
        prev_animation_state.frames == animation_state_player.frames) {

        return;
    }
    prev_animation_pos = game_state.player_animation;
    prev_animation_state = animation_state_player;

    render_base(renderer, game_state.player_animation);
    render_fences(renderer, maze, game_state.player_animation);
    render_others(renderer, game_state.player_animation);
    render_player(renderer);

    SDL_RenderPresent(renderer);
}

void modify_player_animation(struct Game_state* game_state) { 

    int inverse_speed = 10;
    if (queue_size(game_state->pending_movements)) {
        inverse_speed /= queue_size(game_state->pending_movements);
    }

    static unsigned int last_modify = 0;
    int current_time = SDL_GetTicks();

    if ((current_time - last_modify) <= inverse_speed) {
        return;
    }
    int distance = (current_time - last_modify) / inverse_speed;
    last_modify = current_time;

    if (!queue_size(game_state->pending_movements)) {
        animation_state_player.current_frame_no = DEFAULT_FRAME;
        return;
    }

    enum Direction moving_direction;

    for (int distance_left = distance; distance_left != 0;) {

        struct Screen_coord queue_front = *(struct Screen_coord*)queue_peek(game_state->pending_movements);

        moving_direction = get_direction(game_state->player_animation, queue_front);
        distance_left = walk_towards(&game_state->player_animation,
                                         queue_front,
                                         distance);

        if (distance_left != 0) {
            queue_pop(game_state->pending_movements);

            if(!queue_size(game_state->pending_movements)) {
                break;
            }
        }
    }

    if (queue_size(game_state->pending_movements)) {
        struct Screen_coord queue_front = *(struct Screen_coord*)queue_peek(game_state->pending_movements);

        if (game_state->player_animation.x == queue_front.x &&
            game_state->player_animation.y == queue_front.y) {

            queue_pop(game_state->pending_movements);
        }
    }

    int distance_per_frame = 10;

    static int available_distance = 0;
    available_distance += distance;

    while (available_distance >= distance_per_frame) {

        if (animation_state_player.current_frame_no ==
            animation_state_player.no_frames - 1) {

            animation_state_player.current_frame_no = 0;
        }
        else {
            animation_state_player.current_frame_no++;
        }

        available_distance -= distance_per_frame;
    }

    if (animation_state_player.frames != horse1_frame_groups[moving_direction]) {
        animation_state_player.current_frame_no = DEFAULT_FRAME;
        animation_state_player.frames = horse1_frame_groups[moving_direction];
    }
}



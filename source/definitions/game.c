#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "game.h"
#include "SDL_helpers.h"
#include "maze.h"
#include "parameters.h"
#include "game_helpers.h"
#include "queue.h"

bool move_player(struct Coord delta, struct Coord current_pos, 
                 struct Maze maze) {

    struct Coord pos = { current_pos.x + delta.x,
                   current_pos.y + delta.y };
    
    // boundary condition
    if (pos.x < 0 || pos .y < 0 ||
        pos.x >= MAZE_WIDTH || pos.y >= MAZE_HEIGHT) {

        return false;
    }

    return maze.cells[pos.y][pos.x] != MAZE_WALL;
}

void sfx() {
}

void render(SDL_Renderer* renderer, struct Maze maze, struct Coord player_ptr,
            struct Coord* player_animation, struct Queue* pending_movements) {

    move_player_animation(renderer, player_animation, pending_movements);
    render_maze(renderer, maze, *player_animation);
    render_player_ptr(renderer, *player_animation, player_ptr);
    render_player(renderer);

    SDL_RenderPresent(renderer);
}

bool start_game(SDL_Renderer* renderer) {

    bool is_exit = false;

    // clear window
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // load resources
    init_resources(renderer);

    // load audio
    Mix_Music* music = load_music(GAME_MUSIC_PATH);
    Mix_Chunk* move_sound = load_sound(PLAYER_MOVE_SOUND_PATH);
    Mix_Chunk* click_sound = load_sound(CLICK_SOUND_PATH);
    Mix_AllocateChannels(2);

    // play music
    Mix_PlayMusic(music, -1);

    // generate maze
    struct Maze maze;
    create_maze(&maze, MAZE_WIDTH, MAZE_HEIGHT);
    debug_display_maze(maze);

    // game state parameters
    struct Coord player_ptr = { 0, 0 };
    struct Coord player_animation = { 0, 0 };
    struct Queue* pending_movements = queue_create();

    // main loop
    bool is_done = false;
    while (!is_done) {

        // render
        render(renderer, maze, player_ptr, &player_animation, pending_movements);

        // sfx
        sfx();

        // read and handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch(event.type) {

                case SDL_QUIT:
                        // users wants to quit game
                    is_done = true;
                    is_exit = true;
                    Mix_PlayChannel(0, click_sound, 0);
                        // play sound
                    break;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {

                        // move player_ptr

                        case SDLK_RIGHT:
                        {
                            Mix_PlayChannel(0, click_sound, 0);
                            bool success = move_player((struct Coord){ 1, 0 },
                                                       player_ptr, maze);
                            if (success) {
                                player_ptr.x += 1;
                                struct Movement* movement = malloc(sizeof(struct Movement));
                                movement->dest = (struct Coord){ .x = player_ptr.x * TILE_SIZE,
                                                            .y = player_ptr.y * TILE_SIZE };
                                movement->reach_in = SDL_GetTicks() + 1000;
                                queue_push(pending_movements, movement);
                            }
                            break;
                        }

                        case SDLK_LEFT:
                        {
                            Mix_PlayChannel(0, click_sound, 0);
                            bool success = move_player((struct Coord){ -1, 0 },
                                                       player_ptr, maze);
                            if (success) {
                                player_ptr.x -= 1;
                                struct Movement* movement = malloc(sizeof(struct Movement));
                                movement->dest = (struct Coord){ .x = player_ptr.x * TILE_SIZE,
                                                            .y = player_ptr.y * TILE_SIZE };
                                movement->reach_in = SDL_GetTicks() + 1000;
                                queue_push(pending_movements, movement);
                            }
                            break;
                        }

                        case SDLK_UP:
                        {
                            Mix_PlayChannel(0, click_sound, 0);
                            bool success = move_player((struct Coord){ 0, -1 },
                                                       player_ptr, maze);
                            if (success) {
                                player_ptr.y -= 1;
                                struct Movement* movement = malloc(sizeof(struct Movement));
                                movement->dest = (struct Coord){ .x = player_ptr.x * TILE_SIZE,
                                                            .y = player_ptr.y * TILE_SIZE };
                                movement->reach_in = SDL_GetTicks() + 1000;
                                queue_push(pending_movements, movement);
                            }
                            break;
                        }

                        case SDLK_DOWN:
                        {
                            Mix_PlayChannel(0, click_sound, 0);
                            bool success = move_player((struct Coord){ 0, 1 },
                                                       player_ptr, maze);
                            if (success) {
                                player_ptr.y += 1;
                                struct Movement* movement = malloc(sizeof(struct Movement));
                                movement->dest = (struct Coord){ .x = player_ptr.x * TILE_SIZE,
                                                            .y = player_ptr.y * TILE_SIZE };
                                movement->reach_in = SDL_GetTicks() + 1000;
                                queue_push(pending_movements, movement);
                            }
                            break;
                        }
                    }
                    break;
            }
        }
    }

    // clean resources
    free_maze(&maze);
    destroy_resources();

    // stop audio
    Mix_HaltMusic();
    while (Mix_Playing(0)) {
        SDL_Delay(50);
            // wait for click sound to finish
    }

    // clean audio resources
    Mix_FreeMusic(music);
    Mix_FreeChunk(move_sound);

    return !is_exit;
}

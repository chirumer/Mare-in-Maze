#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

//temp
    #include <time.h>

#include "game.h"
#include "SDL_helpers.h"
#include "maze.h"
#include "parameters.h"
#include "game_helpers.h"

void sfx() {
}

void render() {
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

    // temp
    int temp = time(0);

    // main loop
    bool is_done = false;
    while (!is_done) {

        // render
        render();

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

                    fprintf(stderr, "time: %ld.. ", time(0)-temp);

                    if (!Mix_Playing(1)) {
                        Mix_PlayChannel(1, move_sound, 0);
                    }
                    
                    switch (event.key.keysym.sym) {

                        case SDLK_RIGHT:
                            fprintf(stderr, "right..\n");
                            break;

                        case SDLK_LEFT:
                            fprintf(stderr, "left..\n");
                            break;

                        case SDLK_UP:
                            fprintf(stderr, "up..\n");
                            break;

                        case SDLK_DOWN:
                            fprintf(stderr, "down..\n");
                            break;
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

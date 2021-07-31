#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "SDL_helpers.h"
#include "parameters.h"


void initialize(SDL_Window** window, SDL_Renderer** renderer) {

    // seed rand()
    srand(time(0));

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO 
        | SDL_INIT_AUDIO) < 0) {
        // could not init
        
        fprintf(stderr, "SDL_Init error: %s\n",
                SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // initialize mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
        MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        // could not init

        fprintf(stderr, "Mix_OpenAudio error: %s\n",
                Mix_GetError());
        exit(EXIT_FAILURE);
    }

    // initialize SDL image
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        // could not init

        fprintf(stderr, "IMG_Init error: %s\n",
                IMG_GetError());
        exit(EXIT_FAILURE);
    }

    // create window
    *window = SDL_CreateWindow("Mare in Maze",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        // error

        fprintf(stderr, "SDL_CreateWindow error: %s\n",
                SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // create renderer
    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (*renderer == NULL) {
        // error

        fprintf(stderr, "SDL_CreateRenderer error: %s\n",
                SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer) {

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    Mix_Quit();
    IMG_Quit();

    SDL_Quit();
}

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer) {

    SDL_Surface* image = IMG_Load(path);
    if (image == NULL) {
        // error

        fprintf(stderr, "IMG_Load error: %s\npath:%s\n",
                IMG_GetError(), path);
        exit(EXIT_FAILURE);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
                                            renderer, image);
    SDL_FreeSurface(image);
    
    return texture;
}

Mix_Music* load_music(char* path) {

    Mix_Music* music = Mix_LoadMUS(path);
    if (music == NULL) {
        // error

        fprintf(stderr, "Mix_LoadMUS error:%s\npath:%s\n",
                Mix_GetError(), path);
        exit(EXIT_FAILURE);
    }

    return music;
}

Mix_Chunk* load_sound(char* path) {

    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == NULL) {
        // error

        fprintf(stderr, "Mix_LoadWAV error:%s\npath:%s\n",
                Mix_GetError(), path);
        exit(EXIT_FAILURE);
    }

    return sound;
}

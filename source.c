// ---- headers ----

#include <SDL2/SDL.h>
 // SDL2 
#include <SDL2/SDL_image.h>
 // for loading images other than bitmap
#include <SDL2/SDL_mixer.h>
// for playing sounds and music
#include <stdio.h>
 // printf(), scanf()
#include <stdlib.h>
 // exit(), EXIT_FAILURE
#include <stdbool.h>
 // bool

// ---- constants ----

// screen dimensions
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

const char* assets_path = "./assets";
// path to the assets folder

// ---- function declarations ----

void initialize(SDL_Window** window, SDL_Renderer** window_renderer);
// create window and surface

void cleanup(SDL_Window** window, SDL_Renderer** window_renderer);
// destroy window and exit SDL

bool show_instructions(SDL_Window* window, SDL_Renderer* window_renderer);
// display an instructions page
// returns whether user has closed program

// ---- main ----

int main(int argc, char* args[]) {

    // initialize necessary things
    SDL_Window* window = NULL;
    SDL_Renderer* window_renderer = NULL;
    initialize(&window, &window_renderer);

    // introduce user to game
    if (!show_instructions(window, window_renderer)) {
        cleanup(&window, &window_renderer);
        return EXIT_SUCCESS;
    }

    // release resources
    cleanup(&window, &window_renderer);

    return EXIT_SUCCESS;
}

// ---- function definitions ----

bool show_instructions(SDL_Window* window, SDL_Renderer* window_renderer) {

    bool is_exit = false;

    // load sound and music
    Mix_Music* background_music = Mix_LoadMUS("./assets/instructions.mp3");
    if (background_music == NULL) {
        printf(
            stderr,
            "Mix_LoadWAV error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }
    Mix_Chunk* click_sound = Mix_LoadWAV("./assets/click.mp3");
    if (click_sound == NULL) {
        printf(
            stderr,
            "Mix_LoadWAV error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(background_music, -1);

    //  load and display instructions image
    SDL_Surface* image = IMG_Load("./assets/instructions.png");
    if (image == NULL) {
        printf(
            stderr,
            "IMG_Load error: %s\n",
            IMG_GetError()
        );
        exit(EXIT_FAILURE);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window_renderer, image);
    SDL_FreeSurface(image);
    SDL_RenderCopy(window_renderer, texture, NULL, NULL);
    SDL_RenderPresent(window_renderer);

    // wait until user press key
    bool is_done = false;
    while (!is_done) {

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                fprintf(stderr, "quitting..\n");
                is_done = true;
                is_exit = true;
                break;
            case SDL_KEYUP:
                Mix_PlayChannel(-1, click_sound, 0);
                is_done = true;
                break;
        }
    }
    Mix_HaltMusic();
    SDL_DestroyTexture(texture);

    while (Mix_Playing(-1)) {
        SDL_Delay(50);
    }
    Mix_FreeMusic(background_music);
    Mix_FreeChunk(click_sound);

    return is_exit;
}

void initialize(SDL_Window** window, SDL_Renderer** window_renderer) {

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {

        fprintf(
            stderr,
            "SDL_Init error: %s\n",
            SDL_GetError()
        );
        exit(EXIT_FAILURE);
    } 

    // initialize mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {

        fprintf(
            stderr,
            "Mix_OpenAudio error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }

    // create our window
    *window = SDL_CreateWindow(
        "Mare in Maze",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (*window == NULL) { // something went wrong

        fprintf(
            stderr,
            "SDL_CreateWindow error: %s\n",
            SDL_GetError()
        );
        exit(EXIT_FAILURE);
    }

    *window_renderer = SDL_CreateRenderer(*window, -1, 0);
    if (*window_renderer == NULL) { // something went wrong

        fprintf(
            stderr,
            "SDL_GetWindowSurface error: %s\n",
            SDL_GetError()
        );
        exit(EXIT_FAILURE);
    }

    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        fprintf(
            stderr,
            "IMG_Init error: %s\n",
            IMG_GetError()
        );
        exit(EXIT_FAILURE);
    }
}

void cleanup(SDL_Window** window, SDL_Renderer** window_renderer) {

    SDL_DestroyWindow(*window);
    *window = NULL;
    SDL_DestroyRenderer(*window_renderer);
    *window_renderer = NULL;
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

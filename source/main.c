#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "SDL_helpers.h"
#include "game_instructions.h"
#include "game.h"


int main(int argc, char* args[]) {

    // initialize
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    initialize(&window, &renderer);

    bool is_exit;

    // game instructions
    is_exit = show_instructions(renderer);
    if (is_exit) {
        cleanup(window, renderer);
        return EXIT_SUCCESS;
    }

    // the game
    is_exit = start_game(renderer);
    if (is_exit) {
        cleanup(window, renderer);
        return EXIT_SUCCESS;
    }

    cleanup(window, renderer);
    return EXIT_SUCCESS;
}

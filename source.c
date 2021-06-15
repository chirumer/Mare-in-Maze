// ---- headers ----

#include <SDL2/SDL.h>
// SDL2 
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

void initialize(SDL_Window** window, SDL_Surface** window_surface);
    // create window and surface

void cleanup(SDL_Window** window, SDL_Surface** window_surface);
    // destroy window and exit SDL
    
void show_instructions(SDL_Window* window, SDL_Surface* window_surface);
    // display an instructions page
    
// ---- main ----

int main(int argc, char* args[]) {

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

	fprintf(
	    stderr,
	    "SDL_Init error: %s\n",
	    SDL_GetError()
	);
	exit(EXIT_FAILURE);
    }
	
    // initialize necessary things
    SDL_Window* window = NULL; 
    SDL_Surface* window_surface = NULL;
    initialize(&window, &window_surface);

    // introduce user to game
    show_instructions(window, window_surface);

    // release resources
    cleanup(&window, &window_surface);

    return EXIT_SUCCESS;
}

// ---- function definitions ----

void show_instructions(SDL_Window* window, SDL_Surface* window_surface) {

    SDL_FillRect(
	window_surface,
	NULL,
	SDL_MapRGB(
	    window_surface->format,
	    0xFF,
	    0xFF,
	    0xFF
	)
    );
    SDL_UpdateWindowSurface(window);

    bool is_quit = false;
    while (!is_quit) {

	SDL_Event event;
	while (!SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
		fprintf(stderr, "quitting..");
		is_quit = true;
		break;
	    }
	}
    }
}

void initialize(SDL_Window** window, SDL_Surface** window_surface) {

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

    *window_surface = SDL_GetWindowSurface(*window);
    if (*window_surface == NULL) { // something went wrong

	fprintf(
	    stderr,
	    "SDL_GetWindowSurface error: %s\n",
	    SDL_GetError()
	);
	exit(EXIT_FAILURE);
    }
}

void cleanup(SDL_Window** window, SDL_Surface** window_surface) {

    SDL_DestroyWindow(*window);
    *window = NULL;
    *window_surface = NULL;
    SDL_Quit();
}

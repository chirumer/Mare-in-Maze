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

void initialize(SDL_Window** window, SDL_Renderer** window_renderer);
    // create window and surface

void cleanup(SDL_Window** window, SDL_Renderer** window_renderer);
    // destroy window and exit SDL
    
bool show_instructions(SDL_Window* window, SDL_Renderer* window_renderer);
    // display an instructions page
    // returns whether user has closed program
    
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

    //  load and display instructions image
    SDL_Surface* image = SDL_LoadBMP("./assets/instructions.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window_renderer, image);
    SDL_FreeSurface(image);
    SDL_RenderCopy(window_renderer, texture, NULL, NULL);
    SDL_RenderPresent(window_renderer);

    // wait until user press key
    bool is_done = false;
    while (!is_done) {

	SDL_Event event;
        SDL_WaitEvent(&event);
	while (!SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT) {
		fprintf(stderr, "quitting..\n");
		is_done = true;
		is_exit = true;
		break;
	    }
	    else if (event.type == SDL_KEYUP) {
		is_done = true;
		break;
	    }
	}
    }
    SDL_DestroyTexture(texture);
    return is_exit;
}

void initialize(SDL_Window** window, SDL_Renderer** window_renderer) {

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
}

void cleanup(SDL_Window** window, SDL_Renderer** window_renderer) {

    SDL_DestroyWindow(*window);
    *window = NULL;
    SDL_DestroyRenderer(*window_renderer);
    *window_renderer = NULL;
    SDL_Quit();
}

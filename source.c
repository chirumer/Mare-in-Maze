// ---- headers ----

#include <SDL2/SDL.h>
 // SDL2 
#include <SDL2/SDL_image.h>
 // for loading images other than bitmap
#include <SDL2/SDL_mixer.h>
// for playing sounds and music
#include <stdio.h>
 // fprintf(), scanf()
#include <stdlib.h>
 // exit(), EXIT_FAILURE, rand(), srand()
#include <time.h>
// time()
#include <stdbool.h>
 // bool

// ---- struct & enum ----

enum MAZE_CELL {

    MAZE_WALL=0, MAZE_PATH, MAZE_START, MAZE_END
};

// ---- constants ----

// screen dimensions
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

const int MAZE_WIDTH = 50;
const int MAZE_HEIGHT = 50;

const char* assets_path = "./assets";
// path to the assets folder

// ---- function declarations ----

void initialize(SDL_Window** window, SDL_Renderer** window_renderer);
    // create window and surface

void cleanup(SDL_Window** window, SDL_Renderer** window_renderer);
    // destroy window and exit SDL

bool show_instructions(SDL_Window* window, SDL_Renderer* window_renderer);
    // display an instructions page
    // returns false if user has closed program

bool start_game(SDL_Window* window, SDL_Renderer* window_renderer);
    // starts the actual game
    // returns false if the user has closed program

void shuffle_array(int array[], int size);
    // shuffles integer array

bool can_path(enum MAZE_CELL** const maze, const int width,
        const int height, const int x, const int y);
    // checks if the cell can be made into a path

void explore(enum MAZE_CELL** const maze, const int width,
        const int height, const int x, const int y);
    // populate the maze

enum MAZE_CELL** generate_maze(const int width, const int height);
    // generate maze

void delete_maze(enum MAZE_CELL** maze, const int width, const int height);
    // unallocate maze

SDL_Texture* load_texture(SDL_Texture* window_renderer, char* path);

void draw_maze(SDL_Renderer* window_renderer, enum MAZE_CELL** maze, int maze_width,
        int maze_height, SDL_Rect view);

void draw_player(SDL_Renderer* window_renderer, SDL_Point player_position, SDL_Rect* view);

void move_player(SDL_Point* player_position, enum MAZE_CELL** maze, SDL_Point movement);

void update_view(SDL_Rect* view, SDL_Point player_position, int width, int height);

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

    // start the actual game
    if (!start_game(window, window_renderer)) {
        cleanup(&window, &window_renderer);
        return EXIT_SUCCESS;
    }

    // release resources
    cleanup(&window, &window_renderer);

    return EXIT_SUCCESS;
}

// ---- function definitions ----

SDL_Texture* load_texture(SDL_Texture* window_renderer, char* path) {

    SDL_Surface* image = IMG_Load(path);
    if (image == NULL) {
        fprintf(
            stderr,
            "IMG_Load error: %s\n",
            IMG_GetError()
        );
        exit(EXIT_FAILURE);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(window_renderer, image);
    SDL_FreeSurface(image);
    return texture;
}

void draw_player(SDL_Renderer* window_renderer, SDL_Point player_position, SDL_Rect* view) {

    SDL_Texture* player_texture = load_texture(window_renderer, "./assets/player.png");
    SDL_Rect player_rect = {
        (player_position.x - view->x) * 50,
        (player_position.y - view->y) * 50,
        50,
        50
    };
    SDL_RenderCopy(window_renderer, player_texture, NULL, &player_rect);
}

void draw_maze(SDL_Renderer* window_renderer, enum MAZE_CELL** maze, int maze_width, 
        int maze_height, SDL_Rect view) {

    // make screen red
    SDL_SetRenderDrawColor(window_renderer, 255, 0, 0, 255);
    SDL_RenderClear(window_renderer);

    // load textures
    SDL_Texture* start_texture = load_texture(window_renderer, "./assets/start.png");
    SDL_Texture* goal_texture = load_texture(window_renderer, "./assets/goal.png");
    SDL_Texture* path_texture = load_texture(window_renderer, "./assets/path.png");
    SDL_Texture* wall_texture = load_texture(window_renderer, "./assets/wall.png");

    // draw maze view
    for (int i = view.x; i < view.w + view.x; ++i) {
        for (int j = view.y; j < view.h + view.y; ++j) {
            SDL_Rect cell = { 50*(i-view.x), 50*(j-view.y), 50, 50 };
            if (maze[i][j] == MAZE_PATH) {
                SDL_RenderCopy(window_renderer, path_texture, NULL, &cell);
            }
            else if (maze[i][j] == MAZE_WALL) {
                SDL_RenderCopy(window_renderer, wall_texture, NULL, &cell);
            }
            else if (maze[i][j] == MAZE_START) {
                SDL_RenderCopy(window_renderer, start_texture, NULL, &cell);
            }
            else if (maze[i][j] == MAZE_END) {
                SDL_RenderCopy(window_renderer, goal_texture, NULL, &cell);
            }
        }
    }
}

void move_player(SDL_Point* player_position, enum MAZE_CELL** maze, SDL_Point movement) {

    player_position->x += movement.x;
    player_position->y += movement.y;
}

void update_view(SDL_Rect* view, SDL_Point player_position, int width, int height) {

    // attempts to make player (5,5) from top left corner

    if (player_position.x - 5 < 0) {
        view->x = 0;
    }
    else if (player_position.x + 4 >= width) {
        view->x = width - 9;
    }
    else {
        view->x = player_position.x - 5;
    }

    if (player_position.y - 5 < 0) {
        view->y = 0;
    }
    else if (player_position.y + 4 >= height) {
        view->y = 0;
    }
    else {
        view->y = player_position.y - 5;
    }
}

bool start_game(SDL_Window* window, SDL_Renderer* window_renderer) {

    bool is_exit = false;

    // load sound and music
    Mix_Music* background_music = Mix_LoadMUS("./assets/game_music.mp3");
    if (background_music == NULL) {
        fprintf(
            stderr,
            "Mix_LoadWAV error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }
    Mix_Chunk* click_sound = Mix_LoadWAV("./assets/click.mp3");
    if (click_sound == NULL) {
        fprintf(
            stderr,
            "Mix_LoadWAV error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(background_music, -1);


    // create game objects
    enum MAZE_CELL** maze;
    for (;;) {
        maze = generate_maze(MAZE_WIDTH, MAZE_HEIGHT);
        if (maze[0][0] == MAZE_PATH && maze[MAZE_WIDTH-1][MAZE_HEIGHT-1] == MAZE_PATH) {
            break;
        }
        // need top left corner and button right corner to be paths
        delete_maze(maze, MAZE_WIDTH, MAZE_HEIGHT);
    }
    maze[0][0] = MAZE_START;
    maze[MAZE_WIDTH-1][MAZE_HEIGHT-1] = MAZE_END;

    SDL_Point player_position = { 0, 0 };
    SDL_Rect view = { 0, 0, 10, 10 };

    // draw 
    draw_maze(window_renderer, maze, MAZE_WIDTH, MAZE_HEIGHT, view);
    SDL_RenderPresent(window_renderer);

    // event loop
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
                if (event.key.keysym.sym == SDLK_DOWN) {
                    move_player(&player_position, maze, (SDL_Point){0, +1});
                    update_view(&view, player_position, MAZE_WIDTH, MAZE_HEIGHT);
                    Mix_PlayChannel(-1, click_sound, 0);
                    draw_maze(window_renderer, maze, MAZE_WIDTH, MAZE_HEIGHT, view);
                    draw_player(window_renderer, player_position, &view);
                    SDL_RenderPresent(window_renderer);
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    move_player(&player_position, maze, (SDL_Point){0, -1});
                    update_view(&view, player_position, MAZE_WIDTH, MAZE_HEIGHT);
                    Mix_PlayChannel(-1, click_sound, 0);
                    draw_maze(window_renderer, maze, MAZE_WIDTH, MAZE_HEIGHT, view);
                    draw_player(window_renderer, player_position, &view);
                    SDL_RenderPresent(window_renderer);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    move_player(&player_position, maze, (SDL_Point){+1, 0});
                    update_view(&view, player_position, MAZE_WIDTH, MAZE_HEIGHT);
                    Mix_PlayChannel(-1, click_sound, 0);
                    draw_maze(window_renderer, maze, MAZE_WIDTH, MAZE_HEIGHT, view);
                    draw_player(window_renderer, player_position, &view);
                    SDL_RenderPresent(window_renderer);
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    move_player(&player_position, maze, (SDL_Point){-1, 0});
                    update_view(&view, player_position, MAZE_WIDTH, MAZE_HEIGHT);
                    Mix_PlayChannel(-1, click_sound, 0);
                    draw_maze(window_renderer, maze, MAZE_WIDTH, MAZE_HEIGHT, view);
                    draw_player(window_renderer, player_position, &view);
                    SDL_RenderPresent(window_renderer);
                }
                fprintf(stderr, "%d %d %d %d\n", view.x, view.y, view.w, view.h);
                break;
        }
    }

    // stop and destroy audio
    Mix_HaltMusic();
    while (Mix_Playing(-1)) {
        SDL_Delay(50);
    }
    Mix_FreeMusic(background_music);
    Mix_FreeChunk(click_sound);

    // make screen white
    SDL_SetRenderDrawColor(window_renderer, 255, 255, 255, 255);
    SDL_RenderClear(window_renderer);
    SDL_RenderPresent(window_renderer);

    return !is_exit;
}

void shuffle_array(int array[], int size) {
    // Fisher-Yates shuffle algorithm

    for (int selected = size-1; selected > 0; --selected) {

        int random = rand() % (selected+1);
        
        // swap selected and random
        int temp = array[selected];
        array[selected] = array[random];
        array[random] = temp;
    }
}

bool can_path(enum MAZE_CELL** const maze, const int width, 
        const int height, const int x, const int y) {

    // boundary condition
    if (x < 0 || y < 0 || x >= width || y >= height) {

        return false;
    }

    // parent condition
    if (maze[x][y] == MAZE_PATH) {

        return false;
    }

    // intersecting condition: no_neighbors > 1
    int no_neighbors = 0;

    int chng_x[] = {0, 0, -1, 1};
    int chng_y[] = {-1, 1, 0, 0};

    // check for neighbors
    for (int i = 0; i < 4; ++i) {

        int neighbor_x = x + chng_x[i];
        int neighbor_y = y + chng_y[i];

        // boundary condition
        if (neighbor_x < 0 || neighbor_y < 0
                || neighbor_x >= width || neighbor_y >= height) {

            continue; // can't be neighbor
        }

        // neighbor condition
        if (maze[neighbor_x][neighbor_y] == MAZE_PATH) {
            ++no_neighbors;
        }
    }
    return no_neighbors <= 1; 
    // 0 -> starting
    // 1 -> parent
}

void explore(enum MAZE_CELL** const maze, const int width,
        const int height, const int x, const int y) {
    // populate maze using depth-first

    // end-point condition
    if (!can_path(maze, width, height, x, y)) {

        return;
    }

    // populate cell
    maze[x][y] = MAZE_PATH;

    int chng_x[] = {0, 0, -1, 1};
    int chng_y[] = {-1, 1, 0, 0};

    int indx_order[] = {0, 1, 2, 3};
    shuffle_array(indx_order, 4);
        // order of visitation

    // explore neighbors
    for (int i = 0; i < 4; ++i) {

        int neighbor_x = x + chng_x[indx_order[i]];
        int neighbor_y = y + chng_y[indx_order[i]];

        explore(maze, width, height, neighbor_x, neighbor_y);
    }
}

enum MAZE_CELL** generate_maze(const int width, const int height) {

    // initialize all maze cells to MAZE_WALL
    enum MAZE_CELL** maze = calloc(width, sizeof(enum MAZE_CELL*));
    
    for (int i = 0; i < width; ++i) {

        maze[i] = calloc(height, sizeof(enum MAZE_CELL));
    }

    // populate the maze
    explore(maze, width, height, 0, 0);

    return maze;
}

void delete_maze(enum MAZE_CELL** maze, const int width, const int height) {

    for (int i = 0; i < width; ++i) {

        free(maze[i]);
    }
    free(maze);
}

bool show_instructions(SDL_Window* window, SDL_Renderer* window_renderer) {

    bool is_exit = false;

    // load sound and music
    Mix_Music* background_music = Mix_LoadMUS("./assets/instructions.mp3");
    if (background_music == NULL) {
        fprintf(
            stderr,
            "Mix_LoadWAV error: %s\n",
            Mix_GetError()
        );
        exit(EXIT_FAILURE);
    }
    Mix_Chunk* click_sound = Mix_LoadWAV("./assets/click.mp3");
    if (click_sound == NULL) {
        fprintf(
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
        fprintf(
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

    // make screen white
    SDL_SetRenderDrawColor(window_renderer, 255, 255, 255, 255);
    SDL_RenderClear(window_renderer);
    SDL_RenderPresent(window_renderer);

    return !is_exit;
}

void initialize(SDL_Window** window, SDL_Renderer** window_renderer) {

    // seed rand()
    srand(time(0));

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

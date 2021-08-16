#include <stdbool.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "game_audio.h"
#include "game_graphics.h"
#include "game_helpers.h"
#include "maze.h"
#include "game_state.h"
#include "debug.h"
#include "constants.h"

const int max_frame_rate = 200;

bool start_game(SDL_Renderer* renderer) {

    bool is_exit = false;

    display_loading(renderer);

    graphics_init(renderer);
    audio_init();

    audio_music_start();

    struct Maze maze;
    maze_create(&maze, MAZE_WIDTH, MAZE_HEIGHT);

    struct Game_state game_state;
    game_state_init(&game_state);

    // main loop
    bool is_done = false;
    while (!is_done) {

        // per frame actions
        modify_player_animation(&game_state);
        graphics_render(renderer, maze, game_state, max_frame_rate);
        audio_toggle(game_state);
        if (reached_goal(game_state)) {

            audio_music_stop();

            audio_sound_play(SOUND_WIN);
            audio_sound_await_end(SOUND_WIN);

            break;
        }

        // event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch(event.type) {

                case SDL_QUIT:
                        // user wants to quit game

                    is_done = true;
                    is_exit = true;

                    audio_music_stop();

                    audio_sound_play(SOUND_CLICK);
                    audio_sound_await_end(SOUND_CLICK);

                    break;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {

                        case SDLK_RIGHT:

                            // prevent sound overlap
                            if (!is_blocked(BLOCKABLE_1)) {
                                audio_sound_play(SOUND_CLICK);
                                set_blocked(BLOCKABLE_1);
                            }

                            move_player(RIGHT, maze, &game_state);
                            break;

                        case SDLK_LEFT:

                            // prevent sound overlap
                            if (!is_blocked(BLOCKABLE_2)) {
                                audio_sound_play(SOUND_CLICK);
                                set_blocked(BLOCKABLE_2);
                            }

                            move_player(LEFT, maze, &game_state);
                            break;

                        case SDLK_UP:

                            // prevent sound overlap
                            if (!is_blocked(BLOCKABLE_3)) {
                                audio_sound_play(SOUND_CLICK);
                                set_blocked(BLOCKABLE_3);
                            }

                            move_player(UP, maze, &game_state);
                            break;

                        case SDLK_DOWN:

                            // prevent sound overlap
                            if (!is_blocked(BLOCKABLE_4)) {
                                audio_sound_play(SOUND_CLICK);
                                set_blocked(BLOCKABLE_4);
                            }

                            move_player(DOWN, maze, &game_state);
                            break;

                        default:
                            audio_sound_play(SOUND_ERROR);
                    }
                    break;

                case SDL_KEYUP:

                    switch(event.key.keysym.sym) {

                        case SDLK_RIGHT:
                            unset_blocked(BLOCKABLE_1);
                            break;

                        case SDLK_LEFT:
                            unset_blocked(BLOCKABLE_2);
                            break;

                        case SDLK_UP:
                            unset_blocked(BLOCKABLE_3);
                            break;

                        case SDLK_DOWN:
                            unset_blocked(BLOCKABLE_4);
                            break;
                    }
                    break;
            }
        }
        // give the CPU a break
        SDL_Delay(10);
    }

    // clean resources
    game_state_destroy(&game_state);
    maze_destroy(&maze);
    graphics_destroy();
    audio_destroy();

    return is_exit;
}

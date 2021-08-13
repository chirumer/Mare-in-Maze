#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "game_instructions.h"
#include "SDL_helpers.h"
#include "constants.h"
#include "resource_paths.h"

bool show_instructions(SDL_Renderer* renderer) {

    bool is_exit = false;

    {
        // display instructions

        SDL_Texture* instructions = load_texture(
                                        PATH_IMAGE_INSTRUCTIONS,
                                        renderer);
        SDL_RenderCopy(renderer, instructions, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(instructions);
    }

    // load audio
    Mix_Music* music = load_music(PATH_MUSIC_INSTRUCTIONS);
    Mix_Chunk* click_sound = load_sound(PATH_SOUND_CLICK_1);
    Mix_AllocateChannels(1);

    // play music
    Mix_PlayMusic(music, -1);

    // main loop
    bool is_done = false;
    while (!is_done) {

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch(event.type) {

            case SDL_QUIT:
                    // user wants to quit game
                is_done = true;
                is_exit = true;

                // play sound
                Mix_PlayChannel(0, click_sound, 0);

                // stop music
                Mix_HaltMusic();

                while (Mix_Playing(0)) {
                    SDL_Delay(50);
                        // wait for click sound to finish
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym != SDLK_RETURN) {
                    break;
                        // only interested in return key
                }
                is_done = true;

                // play sound
                Mix_PlayChannel(0, click_sound, 0);

                // stop music
                Mix_HaltMusic();

                while (Mix_Playing(0)) {
                    SDL_Delay(50);
                        // wait for click sound to finish
                }
                break;
        }
    }

    // clean audio resources
    Mix_FreeMusic(music);
    Mix_FreeChunk(click_sound);
    Mix_AllocateChannels(0);

    return is_exit;
}

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "game_instructions.h"
#include "SDL_helpers.h"
#include "parameters.h"

bool show_instructions(SDL_Renderer* renderer) {

    bool is_exit = false;

    // load audio
    Mix_Music* music = load_music(INSTRUCTIONS_MUSIC_PATH);
    Mix_Chunk* click_sound = load_sound(CLICK_SOUND_PATH);
    Mix_AllocateChannels(1);

    // play music
    Mix_PlayMusic(music, -1);

    // display instructions
    SDL_Texture* instructions = load_texture(
                                    INSTRUCTIONS_IMAGE_PATH,
                                    renderer);
    SDL_RenderCopy(renderer, instructions, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(instructions);
    instructions = NULL;

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
                Mix_PlayChannel(0, click_sound, 0);
                    // play sound
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym != SDLK_RETURN) {
                    break;
                        // only interested in return key
                }
                is_done = true;
                Mix_PlayChannel(0, click_sound, 0);
                    // play sound
                break;
        }
    }

    // stop audio
    Mix_HaltMusic();
    while (Mix_Playing(0)) {
        SDL_Delay(50);
            // wait for click sound to finish
    }

    // clean audio resources
    Mix_FreeMusic(music);
    Mix_FreeChunk(click_sound);

    return !is_exit;
}
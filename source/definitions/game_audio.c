#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "game_audio.h"
#include "queue.h"
#include "SDL_helpers.h"
#include "resource_paths.h"

// music
Mix_Music* music;

// sounds
Mix_Chunk* click;
Mix_Chunk* player_movement;
Mix_Chunk* error;

void audio_init(void) {
    
    music = load_music(PATH_MUSIC_GAME);

    click = load_sound(PATH_SOUND_CLICK_1);
    player_movement = load_sound(PATH_SOUND_PLAYER_MOVE);
    error = load_sound(PATH_SOUND_ERROR_1);
    Mix_AllocateChannels(NO_SOUNDS);
}

void audio_destroy(void) {

    Mix_FreeMusic(music);

    Mix_FreeChunk(click);
    Mix_FreeChunk(player_movement);
    Mix_FreeChunk(error);
    Mix_AllocateChannels(0);
}

void audio_music_start(void) {
    Mix_PlayMusic(music, -1);
}

void audio_music_stop(void) {
    Mix_HaltMusic();
}

void audio_sound_play(enum SOUND sound_id) {

    switch (sound_id) {

        case SOUND_CLICK:
            Mix_PlayChannel(SOUND_CLICK, click, 0);
            break;

        case SOUND_PLAYER_MOVEMENT:
            Mix_PlayChannel(SOUND_PLAYER_MOVEMENT,
                            player_movement, 0);
            break;

        case SOUND_ERROR:
            Mix_PlayChannel(SOUND_ERROR, error, 0);
            break;

        default:
            break;
    }
}

void audio_sound_stop(enum SOUND sound_id) {
    Mix_HaltChannel(sound_id);
}

void audio_sound_await_end(enum SOUND sound_id) {

    while(Mix_Playing(sound_id)) {
        SDL_Delay(50);
    }
}

void audio_toggle(struct Game_state game_state) {

    // play player movement while player is moving

    if (queue_size(game_state.pending_movements)
        && !Mix_Playing(SOUND_PLAYER_MOVEMENT)) {

        audio_sound_play(SOUND_PLAYER_MOVEMENT);
    }

    if (!queue_size(game_state.pending_movements)
        && Mix_Playing(SOUND_PLAYER_MOVEMENT)) {

        audio_sound_stop(SOUND_PLAYER_MOVEMENT);
    }
}

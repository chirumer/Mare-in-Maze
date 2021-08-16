#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "game_audio.h"
#include "queue.h"
#include "SDL_helpers.h"
#include "resource_paths.h"

// music
Mix_Music* music;

// sounds
Mix_Chunk* sounds[NO_SOUNDS];

void audio_init(void) {
    
    music = load_music(PATH_MUSIC_GAME);

    sounds[SOUND_CLICK] = load_sound(PATH_SOUND_CLICK_1);
    sounds[SOUND_PLAYER_MOVEMENT] = load_sound(PATH_SOUND_PLAYER_MOVE);
    sounds[SOUND_ERROR] = load_sound(PATH_SOUND_ERROR_1);
    sounds[SOUND_WIN] = load_sound(PATH_SOUND_WIN);
    Mix_AllocateChannels(NO_SOUNDS);
}

void audio_destroy(void) {

    for (int i = 0; i < NO_SOUNDS; ++i) {
        Mix_FreeChunk(sounds[i]);
    }
    Mix_AllocateChannels(0);

    Mix_FreeMusic(music);
}

void audio_music_start(void) {
    Mix_PlayMusic(music, -1);
}

void audio_music_stop(void) {
    Mix_HaltMusic();
}

void audio_sound_play(enum SOUND sound_id) {
    Mix_PlayChannel(sound_id, sounds[sound_id], 0);
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

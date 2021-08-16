#ifndef GAME_AUDIO_H
#define GAME_AUDIO_H

#include "queue.h"
#include "game_state.h"


enum SOUND {
    SOUND_CLICK=0, SOUND_PLAYER_MOVEMENT, SOUND_ERROR,
    SOUND_WIN, NO_SOUNDS
};


void audio_init(void);

void audio_destroy(void);

void audio_music_start(void);

void audio_music_stop(void);

void audio_sound_play(enum SOUND sound_id);

void audio_sound_stop(enum SOUND sound_id);

void audio_sound_await_end(enum SOUND sound_id);

void audio_toggle(struct Game_state game_state);


#endif

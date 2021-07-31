#ifndef SDL_helpers_h
#define SDL_helpers_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


void initialize(SDL_Window** window, SDL_Renderer** renderer);
// Initialize SDL

void cleanup(SDL_Window* window, SDL_Renderer* renderer);
// cleanup SDL

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer);
// load texture from path

Mix_Music* load_music(char* path);
// load music from path

Mix_Chunk* load_sound(char* path);
// load sound from path

#endif

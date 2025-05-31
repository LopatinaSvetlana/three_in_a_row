#pragma once
#include "common_vars.h"
#include "SDL_mixer.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
int condition = 0;

Mix_Chunk **sounds;
Mix_Music* music;
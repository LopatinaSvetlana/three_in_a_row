#pragma once
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"

//extern const int SCREEN_WIDTH=800;
//extern const int SCREEN_HEIGHT=600;


void FillBackground(SDL_Renderer* rend, int colorR, int colorG, int colorB, int colorA);
bool init(SDL_Window*& window, SDL_Renderer*& renderer);
SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer);
Mix_Chunk* loadSound(const char* name);
Mix_Music* loadMusic(const char* name);

bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event, Mix_Chunk* touchSound = nullptr);

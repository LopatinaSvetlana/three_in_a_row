#pragma once
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"

//extern const int SCREEN_WIDTH=800;
//extern const int SCREEN_HEIGHT=600;

bool init(SDL_Window*& window, SDL_Renderer*& renderer);
SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer);
Mix_Chunk* loadSound(const char* name);
Mix_Music* loadMusic(const char* name);
SDL_Texture* renderText(const char* message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer);
bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event);

#pragma once
#include"base.h"
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"
#include "common_vars.h"
#include "SDL_image.h"

using namespace std;
void FillBackground(SDL_Renderer* rend, int colorR, int colorG, int colorB, int colorA) {
	SDL_SetRenderDrawColor(rend, colorR, colorG, colorB, colorA);
	SDL_RenderClear(rend);
}

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "������: " << SDL_GetError() << endl;
		return false;
	}
	window = SDL_CreateWindow(u8"3 � ���", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "������: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		cout << "������: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return true;
}
SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer) {


	SDL_Surface* surface = IMG_Load(name);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	
	if (!surface) {
		cout << "������ �������� �����������: " << IMG_GetError() << endl;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		cout << "������ �������� ��������: " << SDL_GetError() << endl;
	}

	return texture;
}
Mix_Chunk* loadSound(const char* name) {
	Mix_Chunk* sound = Mix_LoadWAV(name);
	if (!sound) {
		cout << "������ �������� �����: " << Mix_GetError() << endl;
	}
	return sound;
}
Mix_Music* loadMusic(const char* name) {
	Mix_Music* music = Mix_LoadMUS(name);
	if (!music) {
		cout << "������ �������� ������: " << Mix_GetError() << endl;
	}
	return music;
}

bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event, Mix_Chunk* touchSound) {
	int x = event.button.x;
	int y = event.button.y;
	bool check = x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	if (check)
		Mix_PlayChannel(0,touchSound,0);
	return check;
}

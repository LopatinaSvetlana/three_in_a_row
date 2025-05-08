#pragma once
#include"base.h"
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"
using namespace std;

extern const int SCREEN_WIDTH = 800;
extern const int SCREEN_HEIGHT = 600;

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "ошибка: " << SDL_GetError() << endl;
		return false;
	}
	window = SDL_CreateWindow(u8"3 в ряд", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "ошибка: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		cout << "ошибка: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	return true;
}
SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(name);
	if (!surface) {
		cout << "Ошибка загрузки изображения: " << IMG_GetError() << endl;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		cout << "Ошибка создания текстуры: " << SDL_GetError() << endl;
	}

	return texture;
}
Mix_Chunk* loadSound(const char* name) {
	Mix_Chunk* sound = Mix_LoadWAV(name);
	if (!sound) {
		cout << "Ошибка загрузки звука: " << Mix_GetError() << endl;
	}
	return sound;
}
Mix_Music* loadMusic(const char* name) {
	Mix_Music* music = Mix_LoadMUS(name);
	if (!music) {
		cout << "Ошибка загрузки музыки: " << Mix_GetError() << endl;
	}
	return music;
}
SDL_Texture* renderText(const char* message, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer) {
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, message, color);
	if (!surface) {
		cout << "Ошибка создания текстовой поверхности: " << TTF_GetError() << endl;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture) {
		cout << "Ошибка создания текстовой текстуры: " << SDL_GetError() << endl;
	}
	return texture;
}
bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
		int x = event.button.x;
		int y = event.button.y;
		return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	}
}

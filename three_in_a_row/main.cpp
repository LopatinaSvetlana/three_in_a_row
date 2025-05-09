#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"
#include"base.h"
#include "common_vars.h"

using namespace std;

//рисовальщик текста
void DrawText(SDL_Renderer* render, TTF_Font *font, const char *text, SDL_Color color, 
	int xPos, int yPos, int Scale) {
	SDL_Surface* photoroom = TTF_RenderText_Solid(font, text, color);

	//трансформация текста: четкие координаты положения и авто\
	//счёт размера пропорционально. Scale - размер одной буквы, ширина
	//это scale умноженный на количество букв
	SDL_Rect textTransform = { xPos, yPos, Scale * strlen(text) * 0.3, Scale};

	SDL_RenderCopy(render,//рендер
		SDL_CreateTextureFromSurface(render, photoroom),//фоткаем текст с поверхности
		NULL, //ничего вырезать не надо
		&textTransform); //передали трансформацию текста
}


int main(int argc, char* argv[]) {

	if (!init(window, renderer))
		return 1;
	bool quit = false;

	Mix_Init(0); //проигрыватель. Загружаем.
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024); //установили формат звука


	TTF_Init(); //
	TTF_Font* font = TTF_OpenFont("font.ttf", 100);

	SDL_Texture* logoType = loadTexture("logotype.bmp", renderer, true, {7,255,7,0});
	SDL_Rect logoTransform = { SCREEN_WIDTH / 2 - 550, SCREEN_HEIGHT - 700, 1200, 350 };

	SDL_Texture* button1 = loadTexture("button1.bmp", renderer);
	SDL_Rect button1_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 350, 250, 90 };

	SDL_Texture* button_back = loadTexture("back.bmp", renderer, true, {7,255,7,0});
	SDL_Rect button_back_cord = {20,20,100,100};


	while (!quit) {
		while (SDL_PollEvent(&event)) {
			
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button = SDL_BUTTON_LEFT) {
					if (isButtonClicked(button1_cord, event))
						condition = 1;
					if (isButtonClicked(button_back_cord, event))
						condition = 0;

				}

		
			}


			switch (condition) {
			case 0:
				FillBackground(renderer, 191, 119, 167, 0);

				
				SDL_RenderCopy(renderer, logoType, NULL, &logoTransform); //рисуем лого

				SDL_RenderCopy(renderer, button1, NULL, &button1_cord); //рисуем кнопку и текст ИГРАТЬ
				DrawText(renderer, font, "PLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 62, SCREEN_HEIGHT - 360, 100);


				SDL_RenderCopy(renderer, logoType, NULL, &logoTransform);

				



				break;


			case 1:
				FillBackground(renderer, 10, 119, 167, 0);
				SDL_RenderCopy(renderer, button_back, NULL, &button_back_cord);
				break;
			}
		}
		SDL_RenderPresent(renderer);
	}
	
	Mix_CloseAudio(); //закрыли проигрыватель

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"
#include"base.h"
#include "common_vars.h"
#include <malloc.h>
using namespace std;

//рисовальщик текста
void DrawText(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_Color color,
	int xPos, int yPos, int Scale) {
	SDL_Surface* photoroom = TTF_RenderText_Solid(font, text, color);

	//трансформация текста: четкие координаты положения и авто\
	//счёт размера пропорционально. Scale - размер одной буквы, ширина
	//это scale умноженный на количество букв
	SDL_Rect textTransform = { xPos, yPos, Scale * strlen(text) * 0.3, Scale };

	SDL_RenderCopy(render,//рендер
		SDL_CreateTextureFromSurface(render, photoroom),//фоткаем текст с поверхности
		NULL, //ничего вырезать не надо
		&textTransform); //передали трансформацию текста
}

void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_cord, int targetCondition = 0) {
	SDL_RenderCopy(renderer, button_back, NULL, &button_back_cord);

	if (isButtonClicked(button_back_cord, event) && event.type == SDL_MOUSEBUTTONDOWN)
		condition = targetCondition;
}
struct elem {
	int i, j;
	SDL_Rect transPos;
	bool isSelected = false;
	int element = 0;
};

int main(int argc, char* argv[]) {
	if (!init(window, renderer))
		return 1;
	bool quit = false;

	Mix_Init(0); //проигрыватель. Загружаем.
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024); //установили формат звука

	IMG_Init(IMG_INIT_PNG);

	TTF_Init(); //
	TTF_Font* font = TTF_OpenFont("font.ttf", 100);

	SDL_Texture* logoType = loadTexture("logotype.png", renderer);
	SDL_Rect logoTransform = { SCREEN_WIDTH / 2 - 550, SCREEN_HEIGHT - 700, 1200, 350 };

	SDL_Texture* button1 = loadTexture("button1.png", renderer);
	SDL_Rect button1_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 350, 250, 90 };

	SDL_Texture* button_back = loadTexture("returnn.png", renderer);
	SDL_Rect button_back_cord = { 20,20,100,100 };

	SDL_Texture* settings_logo = loadTexture("settingslogo.png", renderer);
	SDL_Rect settings_card = { SCREEN_WIDTH - 120,SCREEN_HEIGHT - 120,90,90 };

	SDL_Texture* level1 = loadTexture("level1.bmp", renderer);
	SDL_Texture* level2 = loadTexture("level2.bmp", renderer);
	SDL_Texture* level3 = loadTexture("level3.bmp", renderer);
	SDL_Rect level1_card = { SCREEN_WIDTH / 2 - 200,SCREEN_HEIGHT - 550,400,150 };
	SDL_Rect level2_card = { SCREEN_WIDTH / 2 - 200,SCREEN_HEIGHT - 380,400,150 };
	SDL_Rect level3_card = { SCREEN_WIDTH / 2 - 200,SCREEN_HEIGHT - 210,400,150 };


	SDL_Texture* circle_elem = loadTexture("krug.png", renderer);
	SDL_Texture* square_elem = loadTexture("kvadrat.png", renderer);
	SDL_Texture* rhomb_elem = loadTexture("romb.png", renderer);
	SDL_Texture* star_elem = loadTexture("star.png", renderer);
	SDL_Texture* triangle_elem = loadTexture("treug.png", renderer);

	SDL_Texture* elems[5] = { circle_elem, square_elem, rhomb_elem, star_elem, triangle_elem };


	int scale = 8;
	elem* field = (elem*)malloc(scale * scale * sizeof(elem));
	int gridScale = 75;
	SDL_Rect selectedElem = { 0,0,0,0 };
	SDL_Texture* selectedImg = loadTexture("selected.png", renderer);
	SDL_Texture* toTargetImg = loadTexture("totarget.png", renderer);
	SDL_Texture* gridImg = loadTexture("back.png", renderer);
	bool isSelectedExist = false;

	for (int i = 0; i < scale; ++i) {
		for (int j = 0; j < scale; ++j) {
			int value;
			do {
				value = rand() % 3;
				// Проверка на горизонтальные совпадения board[i][j - 1]
				if (j >= 2 && *&(field + i*scale + (j-1))->element == value && *&(field + i * scale + (j - 2))->element == value)
					continue;
				// Проверка на вертикальные совпадения
				if (i >= 2 && *&(field + (i-1) * scale + j)->element == value && *&(field + (i - 2) * scale + j)->element == value)
					continue;
				break; // Всё ок, выходим из цикла
			} while (true);
			
			*&(field + i*scale + j)->transPos = { i * gridScale + SCREEN_WIDTH / 2 - scale * gridScale / 2,
							j * gridScale + SCREEN_HEIGHT / 2 - scale * gridScale / 2,
							ELEM_SIZE, ELEM_SIZE };
			*&(field + i * scale + j)->element = value;
			*&(field + i * scale + j)->i = i;
			*&(field + i * scale + j)->j = j;
		}
	}
	

	while (!quit) {
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
			case SDL_QUIT:
				quit = true;
			}


			switch (condition) {
			case 0: //главное меню
				if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button = SDL_BUTTON_LEFT) {
						if (isButtonClicked(button1_cord, event))
							condition = 1; //-> выбор уровней
						if (isButtonClicked(settings_card, event))
							condition = 2;
					}
				}

				FillBackground(renderer, 250, 165, 206, 0);
				//250,165,206
				//191, 119, 167

				SDL_RenderCopy(renderer, logoType, NULL, &logoTransform); //рисуем лого

				SDL_RenderCopy(renderer, button1, NULL, &button1_cord); //рисуем кнопку и текст ИГРАТЬ
				DrawText(renderer, font, "PLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 62, SCREEN_HEIGHT - 360, 100);
				SDL_RenderCopy(renderer, logoType, NULL, &logoTransform);
				SDL_RenderCopy(renderer, settings_logo, NULL, &settings_card);
				break;


			case 1: //выбор уровней
				FillBackground(renderer, 250, 165, 206, 0);
				ButtonBackDraw(button_back, button_back_cord);
				SDL_RenderCopy(renderer, level1, NULL, &level1_card);
				SDL_RenderCopy(renderer, level2, NULL, &level2_card);
				SDL_RenderCopy(renderer, level3, NULL, &level3_card);
				if (event.button.button = SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN) {
					if (isButtonClicked(level1_card, event))
						condition = 3;
					if (isButtonClicked(level2_card, event))
						condition = 4;
					if (isButtonClicked(level3_card, event))
						condition = 5;
					break;
			case 2://настройки
				FillBackground(renderer, 250, 165, 206, 0);
				ButtonBackDraw(button_back, button_back_cord);
				break;
			case 3:
				FillBackground(renderer, 181+10, 139+10, 180+10, 0);
				ButtonBackDraw(button_back, button_back_cord, 1);
				
				
				SDL_RenderCopy(renderer, selectedImg, NULL, &selectedElem);
			

				for (int i = 0; i < scale; i++) {
					for (int j = 0; j < scale; j++) {
						elem *curElem = &*(field + i * scale + j);
						
						SDL_RenderCopy(renderer, gridImg, NULL, &curElem->transPos);

						/*/if (*&(field + (i + 1) * scale + j)->isSelected ||
							*&(field + (i) * scale + (j+1))->isSelected ){
							SDL_RenderCopy(renderer, toTargetImg, NULL, &curElem->transPos);
						}
						*/
						
						SDL_RenderCopy(renderer, elems[curElem->element], NULL, &curElem->transPos);
						if (event.type == SDL_MOUSEBUTTONDOWN) {
							if (event.button.button = SDL_BUTTON_LEFT && isButtonClicked(curElem->transPos, event)) {
								
								selectedElem = curElem->transPos; isSelectedExist = true;

								
							}
						}
					}
				}

				
				

				break;
			case 4:
				FillBackground(renderer, 250, 0, 250, 0);
				ButtonBackDraw(button_back, button_back_cord, 1);
				break;
			case 5:
				FillBackground(renderer, 250, 250, 0, 0);
				ButtonBackDraw(button_back, button_back_cord,1);
				break;
				}
			}
			SDL_RenderPresent(renderer);
		}

		
	}
	Mix_CloseAudio(); //закрыли проигрыватель

	SDL_DestroyTexture(logoType);
	SDL_DestroyTexture(button1);
	SDL_DestroyTexture(button_back);
	SDL_DestroyTexture(settings_logo);
	SDL_DestroyTexture(level1);
	SDL_DestroyTexture(level2);
	SDL_DestroyTexture(level3);
	SDL_DestroyTexture(circle_elem);
	SDL_DestroyTexture(square_elem);
	SDL_DestroyTexture(rhomb_elem);
	SDL_DestroyTexture(star_elem);
	SDL_DestroyTexture(triangle_elem);
	free(field);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

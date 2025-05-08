#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"
#include"base.h"
using namespace std;

int main(int argc, char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (!init(window, renderer)) {
		return 1;
	}
	bool quit = false;
	SDL_Event event;
	int condition = 0;
	
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
			if (condition == 0) {
				SDL_SetRenderDrawColor(renderer, 250, 0, 100, 0);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_Rect startGame = { 350,250,100,100 };
				SDL_RenderFillRect(renderer, &startGame);
				if (isButtonClicked(startGame, event)) {
					condition = 1;
				}
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_Rect settings = { 50,50,100,100 };
				SDL_RenderFillRect(renderer, &settings);
				if (isButtonClicked(settings, event)) {
					condition = 2;
				}

			}
			if (condition == 1) {
				SDL_Window* window1 = NULL;
				init(window1, renderer);
				SDL_SetRenderDrawColor(renderer, 250, 0, 200, 0);
				SDL_RenderClear(renderer);
			}
			if (condition == 2) {
				//SDL_SetRenderDrawColor(renderer, 0, 0, 200, 0);
				//SDL_RenderClear(renderer);
			}
		}
		SDL_RenderPresent(renderer);
	}
	


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
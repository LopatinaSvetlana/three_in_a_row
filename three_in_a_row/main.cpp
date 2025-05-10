#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include"base.h"
#include"common_vars.h"
#include<malloc.h>
#include <string>
using namespace std;

struct elem {
    int i, j;
    SDL_Rect transPos;
    bool isSelected = false;
    int element = 0;
};

void DrawText(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_Color color,
    int xPos, int yPos, int Scale) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect textTransform = { xPos, yPos, static_cast<int>(Scale * strlen(text) * 0.3), Scale };
    SDL_RenderCopy(render, texture, NULL, &textTransform);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_cord, int targetCondition = 0) {
    SDL_RenderCopy(renderer, button_back, NULL, &button_back_cord);
    if (isButtonClicked(button_back_cord, event) && event.type == SDL_MOUSEBUTTONDOWN)
        condition = targetCondition;
}

void GenerateInitialField(elem* field, int scale, int amount_figure, int gridScale) {
    for (int i = 0; i < scale; ++i) {
        for (int j = 0; j < scale; ++j) {
            int value;
            do {
                value = rand() % amount_figure;
                if (j >= 2 &&
                    field[i * scale + (j - 1)].element == value &&
                    field[i * scale + (j - 2)].element == value)
                    continue;
                if (i >= 2 &&
                    field[(i - 1) * scale + j].element == value &&
                    field[(i - 2) * scale + j].element == value)
                    continue;
                break;
            } while (true);

            field[i * scale + j] = {
                i, j,
                {
                    i * gridScale + SCREEN_WIDTH / 2 - scale * gridScale / 2,
                    j * gridScale + SCREEN_HEIGHT / 2 - scale * gridScale / 2,
                    ELEM_SIZE, ELEM_SIZE
                },
                false,
                value
            };
        }
    }
}

int RemoveMatches(elem* field, int scale) {
    int removedCount = 0;

    // Горизонтальные совпадения
    for (int i = 0; i < scale; i++) {
        int j = 0;
        while (j < scale) {
            int start = j;
            int currentElem = field[i * scale + j].element;
            while (j < scale && field[i * scale + j].element == currentElem && currentElem != -1)
                j++;

            if (j - start >= 3) {
                for (int k = start; k < j; ++k) {
                    if (field[i * scale + k].element != -1) {
                        field[i * scale + k].element = -1;
                        removedCount++;
                    }
                }
            }

            if (j == start) j++;
        }
    }

    // Вертикальные совпадения
    for (int j = 0; j < scale; j++) {
        int i = 0;
        while (i < scale) {
            int start = i;
            int currentElem = field[i * scale + j].element;
            while (i < scale && field[i * scale + j].element == currentElem && currentElem != -1)
                i++;

            if (i - start >= 3) {
                for (int k = start; k < i; ++k) {
                    if (field[k * scale + j].element != -1) {
                        field[k * scale + j].element = -1;
                        removedCount++;
                    }
                }
            }

            if (i == start) i++;
        }
    }

    return removedCount;
}

void GenerateNewElements(elem* field, int scale, int amount_figure) {
    for (int i = 0; i < scale; ++i) {
        for (int j = 0; j < scale; ++j) {
            elem* cell = &field[i * scale + j];
            if (cell->element != -1) continue;

            int value;
            do {
                value = rand() % amount_figure;

                // Проверка по горизонтали
                if (j >= 2 &&
                    field[i * scale + (j - 1)].element == value &&
                    field[i * scale + (j - 2)].element == value)
                    continue;

                // Проверка по вертикали
                if (i >= 2 &&
                    field[(i - 1) * scale + j].element == value &&
                    field[(i - 2) * scale + j].element == value)
                    continue;

                // Дополнительная проверка на возможность составить три вместе с соседними (вниз и вправо)
                if (j >= 1 && j + 1 < scale &&
                    field[i * scale + (j - 1)].element == value &&
                    field[i * scale + (j + 1)].element == value)
                    continue;

                if (i >= 1 && i + 1 < scale &&
                    field[(i - 1) * scale + j].element == value &&
                    field[(i + 1) * scale + j].element == value)
                    continue;

                break; // значение подходит
            } while (true);

            cell->element = value;
        }
    }
}



bool HasMatch(elem* field, int scale) {
    for (int i = 0; i < scale; ++i) {
        for (int j = 0; j < scale; ++j) {
            int current = field[i * scale + j].element;
            if (current == -1) continue;

            if (j <= scale - 3 &&
                current == field[i * scale + (j + 1)].element &&
                current == field[i * scale + (j + 2)].element)
                return true;

            if (i <= scale - 3 &&
                current == field[(i + 1) * scale + j].element &&
                current == field[(i + 2) * scale + j].element)
                return true;
        }
    }
    return false;
}

void DrawDynamicUI(SDL_Renderer* renderer, TTF_Font* font, int remainingMoves, int Score) {
    DrawText(renderer, font, "REMAINING MOVES: ", { 255,255,255,0 }, 20, SCREEN_HEIGHT / 2 - 200, 50);
    DrawText(renderer, font, to_string(remainingMoves).c_str(), { 255,255,255,0 }, 20, SCREEN_HEIGHT / 2 + 200, 50);
}

void RunLevel(SDL_Renderer* renderer, int amount_figure, int scale, SDL_Texture* gridImg, SDL_Texture** elems,
    SDL_Texture* selectedImg, SDL_Event event, TTF_Font* font, SDL_Texture* button_back, int maxMoves, int maxScore) {
    int gridScale = 70;
    elem* field = (elem*)malloc(scale * scale * sizeof(elem));
    SDL_Rect selectedElem = { 0,0,0,0 };
    bool isSelectedExist = false;
    SDL_Rect button_back_cord = { 20, 20, 100, 100 };
    int selectedIndex = -1;

   

    GenerateInitialField(field, scale, amount_figure, gridScale);
    int remainingMoves = maxMoves;
    int localScore = 0;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                condition = -1;
                running = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && isButtonClicked(button_back_cord, event)) {
                condition = 1;
                running = false;
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < scale; i++) {
                    for (int j = 0; j < scale; j++) {
                        elem* curElem = &field[i * scale + j];
                        if (isButtonClicked(curElem->transPos, event)) {
                            if (!isSelectedExist) {
                                selectedElem = curElem->transPos;
                                selectedIndex = i * scale + j;
                                isSelectedExist = true;
                            }
                            else {
                               
                                int selI = selectedIndex / scale;
                                int selJ = selectedIndex % scale;

                                if ((abs(selI - i) == 1 && selJ == j) || (abs(selJ - j) == 1 && selI == i)) {
                                    int secondIndex = i * scale + j;
                                    swap(field[selectedIndex].element, field[secondIndex].element);

                                    if (HasMatch(field, scale)) { //если ход выдал три в ряд
                                        RemoveMatches(field, scale);
                                        remainingMoves--;
                                        // Рендерим поле с пустыми клетками
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        //обнови счёт!
                                        DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                        if (isSelectedExist)
                                            SDL_RenderCopy(renderer, selectedImg, NULL, &selectedElem);

                                        for (int ii = 0; ii < scale; ii++) {
                                            for (int jj = 0; jj < scale; jj++) {
                                                elem* e = &field[ii * scale + jj];
                                                SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                if (e->element != -1)
                                                    SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                            }
                                        }
                                        SDL_RenderPresent(renderer);

                                        SDL_Delay(500);  // Пауза перед появлением новых

                                        GenerateNewElements(field, scale, amount_figure);
                                        // Цепная реакция — убираем новые совпадения после генерации
                                        while (HasMatch(field, scale)) {
                                            SDL_Delay(300);
                                            RemoveMatches(field, scale);

                                            // Рендерим после удаления
                                            SDL_RenderClear(renderer);
                                            FillBackground(renderer, 191, 149, 190, 0);
                                            ButtonBackDraw(button_back, button_back_cord, 1);
                                            DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                            for (int ii = 0; ii < scale; ii++) {
                                                for (int jj = 0; jj < scale; jj++) {
                                                    elem* e = &field[ii * scale + jj];
                                                    SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                    if (e->element != -1)
                                                        SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                                }
                                            }

                                            SDL_RenderPresent(renderer);
                                            SDL_Delay(500); // Пауза перед генерацией

                                            GenerateNewElements(field, scale, amount_figure);
                                        }


                                    }
                                    else {
                                        remainingMoves--;
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                        for (int ii = 0; ii < scale; ii++) {
                                            for (int jj = 0; jj < scale; jj++) {
                                                elem* e = &field[ii * scale + jj];
                                                SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                if (e->element != -1)
                                                    SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                            }
                                        }
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(500);
                                        swap(field[selectedIndex].element, field[secondIndex].element);
                                    }
                                }

                                isSelectedExist = false;
                                selectedIndex = -1;
                            }
                        }
                    }
                }
            }
        }

        FillBackground(renderer, 191, 149, 190, 0);
        ButtonBackDraw(button_back, button_back_cord, 1);
        DrawDynamicUI(renderer, font, remainingMoves, localScore);
        if (isSelectedExist)
            SDL_RenderCopy(renderer, selectedImg, NULL, &selectedElem);

        for (int i = 0; i < scale; i++) {
            for (int j = 0; j < scale; j++) {
                elem* curElem = &field[i * scale + j];
                SDL_RenderCopy(renderer, gridImg, NULL, &curElem->transPos);
                if (curElem->element >= 0)
                    SDL_RenderCopy(renderer, elems[curElem->element], NULL, &curElem->transPos);
            }
        }

        SDL_RenderPresent(renderer);
    }

    free(field);
}

int main(int argc, char* argv[]) {
    if (!init(window, renderer))
        return 1;

    Mix_Init(0);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font.ttf", 100);

    SDL_Texture* logoType = loadTexture("logotype.png", renderer);
    SDL_Texture* button1 = loadTexture("button1.png", renderer);
    SDL_Texture* button_back = loadTexture("returnn.png", renderer);
    SDL_Texture* settings_logo = loadTexture("settingslogo.png", renderer);
    SDL_Texture* level1 = loadTexture("level1.bmp", renderer);
    SDL_Texture* level2 = loadTexture("level2.bmp", renderer);
    SDL_Texture* level3 = loadTexture("level3.bmp", renderer);

    SDL_Texture* elems[5] = {
        loadTexture("krug.png", renderer),
        loadTexture("kvadrat.png", renderer),
        loadTexture("romb.png", renderer),
        loadTexture("treug.png", renderer),
        loadTexture("star.png", renderer)
    };

    SDL_Texture* selectedImg = loadTexture("selected.png", renderer);
    SDL_Texture* gridImg = loadTexture("back.png", renderer);

    SDL_Rect button1_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 350, 250, 90 };
    SDL_Rect logoTransform = { SCREEN_WIDTH / 2 - 550, SCREEN_HEIGHT - 700, 1200, 350 };
    SDL_Rect settings_card = { SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120, 90, 90 };
    SDL_Rect level1_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 550, 400, 150 };
    SDL_Rect level2_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 400, 150 };
    SDL_Rect level3_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 210, 400, 150 };
    SDL_Rect button_back_cord = { 20, 20, 100, 100 };

    bool quit = false;
    while (quit==false && condition !=-1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            switch (condition) {
            case 0:
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (isButtonClicked(button1_cord, event))
                        condition = 1;
                    if (isButtonClicked(settings_card, event))
                        condition = 2;
                }

                FillBackground(renderer, 250, 165, 206, 0);
                SDL_RenderCopy(renderer, logoType, NULL, &logoTransform);
                SDL_RenderCopy(renderer, button1, NULL, &button1_cord);
                DrawText(renderer, font, "PLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 62, SCREEN_HEIGHT - 360, 100);
                SDL_RenderCopy(renderer, settings_logo, NULL, &settings_card);
                break;

            case 1:
                FillBackground(renderer, 250, 165, 206, 0);
                ButtonBackDraw(button_back, button_back_cord);
                SDL_RenderCopy(renderer, level1, NULL, &level1_card);
                SDL_RenderCopy(renderer, level2, NULL, &level2_card);
                SDL_RenderCopy(renderer, level3, NULL, &level3_card);

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (isButtonClicked(level1_card, event)) condition = 3;
                    if (isButtonClicked(level2_card, event)) condition = 4;
                    if (isButtonClicked(level3_card, event)) condition = 5;
                }
                break;

            case 2:
                FillBackground(renderer, 250, 165, 206, 0);
                ButtonBackDraw(button_back, button_back_cord);
                break;

            case 3:
                RunLevel(renderer, 3, 8, gridImg, elems, selectedImg, event, font, button_back, 20, 100);
                break;
            case 4:
                RunLevel(renderer, 4, 8, gridImg, elems, selectedImg, event, font, button_back, 30, 200);
                break;
            case 5:
                RunLevel(renderer, 5, 10, gridImg, elems, selectedImg, event, font, button_back, 40, 500);
                break;
            }
            SDL_RenderPresent(renderer);
        }
    }

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

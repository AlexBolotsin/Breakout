#include <SDL.h>
#include <SDL_image.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <vector>
#include "Texture.h"
#include "TitleScreen.h"
#include "GameScreen.h"

bool exitVal = false;
int sceneIndex = 0;

int WinMain(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	SDL_Window* window = NULL;
	window = SDL_CreateWindow("", 350, 150, 800, 500, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		fprintf(stderr, "create window failed: %s\n", SDL_GetError());
		return 1;   // 'error' return status is !0. 1 is good enough
	}

	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {   // renderer creation may fail too
		fprintf(stderr, "create renderer failed: %s\n", SDL_GetError());
		return 1;
	}

	std::vector<Screen*> screens{ new TitleScreen("Title") , new GameScreen("Game") };

	for (auto& screen : screens)
		screen->Load(renderer);


	while (!exitVal) {
		auto& screen = screens[sceneIndex];
		screen->HandleEvents();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		screen->Draw(renderer);
		SDL_RenderPresent(renderer);
	}

	for (auto& screen : screens)
		screen->Clear();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
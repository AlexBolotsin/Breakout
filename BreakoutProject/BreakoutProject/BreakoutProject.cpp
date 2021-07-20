#include <SDL.h>
#include <SDL_image.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <vector>
#include <chrono>
#include "Texture.h"
#include "TitleScreen.h"
#include "GameScreen.h"

bool exitVal = false;
int sceneIndex = 1;

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


	auto last = std::chrono::high_resolution_clock::now();
	while (!exitVal) {
		auto current = std::chrono::high_resolution_clock::now();
		float diff = std::chrono::duration_cast<std::chrono::milliseconds>(current - last).count();
		diff = diff > 1'000.f / 60.f * 5.f ? 1'000.f / 60.f : diff;
		last = std::chrono::high_resolution_clock::now();

		auto& screen = screens[sceneIndex];

		screen->HandleEvents(diff);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		screen->Draw(renderer);
		SDL_RenderPresent(renderer);

		auto delay = 1'000.f/60.f - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - current).count();
		SDL_Delay(delay > 0 ? delay : 0);
	}

	for (auto& screen : screens)
		screen->Clear();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
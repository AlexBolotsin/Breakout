#pragma once
#include "Screen.h"
#include "memory"
#include "Texture.h"

#include <functional>

class TitleScreen :
    public Screen
{

	enum class GameState {
		STATE_NONE,
		STATE_IN_MENU,
		STATE_IN_GAME,
	} state = GameState::STATE_IN_MENU;

	std::shared_ptr<Texture> buttonTexture;
	std::shared_ptr<Texture> titleTexture;
	std::shared_ptr<Texture> producedTexture;
	typedef struct {
		SDL_Rect draw_rect;    // dimensions of button
		struct {
			Uint8 r, g, b, a;
		} colour;

		bool pressed;
		std::function<void(void)> f_display;
	} button_t;

	button_t startButton;
	SDL_Renderer* _renderer;

	void button_process_event(button_t& btn, const SDL_Event* ev);
public:
	TitleScreen(std::string title);
	~TitleScreen();

	void Clear() override;
	void Load(SDL_Renderer* renderer) override;
	void HandleEvents(float diff) override;
	void Draw(SDL_Renderer* renderer) override;
};


#include "TitleScreen.h"
#include "Signal.h"

void TitleScreen::button_process_event(button_t& btn, const SDL_Event* ev) {
	// react on mouse click within button rectangle by setting 'pressed'
	if (ev->type == SDL_MOUSEBUTTONDOWN) {
		if (ev->button.button == SDL_BUTTON_LEFT &&
			ev->button.x >= btn.draw_rect.x &&
			ev->button.x <= (btn.draw_rect.x + btn.draw_rect.w) &&
			ev->button.y >= btn.draw_rect.y &&
			ev->button.y <= (btn.draw_rect.y + btn.draw_rect.h)) {
			btn.pressed = true;
			btn.f_display();
		}
	}
}

TitleScreen::TitleScreen(std::string title) : Screen(title)
{
}

TitleScreen::~TitleScreen()
{
	Clear();
}

void TitleScreen::Clear()
{
	if (buttonTexture)
		buttonTexture->free();
	if (titleTexture)
		titleTexture->free();
}

void TitleScreen::Load(SDL_Renderer* renderer)
{
	_renderer = renderer;
	// button state - colour and rectangle
	if (!buttonTexture)
	{
		buttonTexture = std::make_shared<Texture>(renderer);
		buttonTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\Button.png");
	}

	if (!titleTexture)
	{
		titleTexture = std::make_shared<Texture>(renderer);
		titleTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\Title.png");
	}
	startButton = { {400, 250, buttonTexture->getWidth(), buttonTexture->getHeight() }, {255, 255, 255, 255}, false, []() { Signal().Send(Signal::Code::NEXT_SCREEN, 1); } };
}

void TitleScreen::HandleEvents()
{
	SDL_Event evt;    // no need for new/delete, stack is fine

	// event loop and draw loop are separate things, don't mix them
	while (SDL_PollEvent(&evt)) {
		// quit on close, window close, or 'escape' key hit
		if (evt.type == SDL_QUIT ||
			(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
			(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) {
			Signal().Send(Signal::Code::EXIT, 0);
		}

		// pass event to button
		button_process_event(startButton, &evt);
	}
}

void TitleScreen::Draw(SDL_Renderer* renderer)
{
	titleTexture->render(200, 60);
	buttonTexture->render(startButton.draw_rect.x, startButton.draw_rect.y);
}

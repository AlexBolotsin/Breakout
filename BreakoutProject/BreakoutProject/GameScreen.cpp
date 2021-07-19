#include "GameScreen.h"
#include "Signal.h"

bool circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {
	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // top edge
	else if (cy > ry + rh) testY = ry + rh;   // bottom edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = (float)sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return true;
	}
	return false;
}

GameScreen::GameScreen(std::string title) : Screen(title), mousePosition(400)
{

}

void GameScreen::Draw(SDL_Renderer* renderer)
{
	for (auto& block : blockGroup)
	{
		blockTexture->render(block.rect.x, block.rect.y);
	}
	platformTexture->render(mousePosition, 360);
}

void GameScreen::HandleEvents()
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
	}
	
	static int prevX = -1;
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	if (prevX != -1)
	{
		mousePosition += x - prevX;
	}
	prevX = x;
}

void GameScreen::Load(SDL_Renderer* renderer)
{
	if (!platformTexture)
	{
		platformTexture = std::make_shared<Texture>(renderer);
		platformTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\platform.png");
	}

	if (!blockTexture)
	{
		blockTexture = std::make_shared<Texture>(renderer);
		blockTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\blueblock.png");
	}

	const int blockNumber = 20;
	blockGroup.reserve(blockNumber);
	for (int i = 0; i < blockNumber; i++)
	{
		blockGroup.push_back({ (i < 10 ? i * 64 : (i - 10) * 64) + 96, i < 10 ? 16 : 48,
			blockTexture->getWidth(), blockTexture->getHeight() });
	}
}

void GameScreen::Clear()
{
	blockTexture->free();
}

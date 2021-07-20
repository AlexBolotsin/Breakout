#include "GameScreen.h"
#include "Signal.h"
#include <algorithm>

double deg2rad(float degrees) {
	return degrees * (3.14159265359f/ 180.0f);
}

bool circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh, float& modX, float& modY) {
	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)
	{
		testX = rx;      // test left edge
		modX = -1.f;
	}
	else if (cx > rx + rw)
	{
		testX = rx + rw;   // right edge
		modX = -1.f;
	}

	if (cy < ry)
	{
		testY = ry;      // top edge
		modY = -1.f;
	}
	else if (cy > ry + rh)
	{
		testY = ry + rh;   // bottom edge
		modY = -1.f;
	}

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt(distX * distX + distY * distY);

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return true;
	}
	return false;
}

GameScreen::GameScreen(std::string title) : Screen(title)
{

}

void GameScreen::Draw(SDL_Renderer* renderer)
{
	for (auto& block : blockGroup)
	{
		if (!block.crushed)
			blockTexture->render(block.rect.x, block.rect.y);
	}
	platformTexture->render(platformPos.x, platformPos.y);
	Vec2d rotated = Vec2d(-1.f, 0.f).rotate(deg2rad(90.f)) * 100.f;
	ballTexture->render(ballPos.x, ballPos.y);

	SDL_SetRenderDrawColor(renderer, 0, 120, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, ballPos.x + ballVelocity.x * 100.f, ballPos.y + ballVelocity.y * 100.f, ballPos.x, ballPos.y);
	SDL_RenderDrawLine(renderer, platformPos.x, platformPos.y, platformPos.x + rotated.x, platformPos.y + rotated.y);
}

void GameScreen::HandleEvents(float diff)
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
	
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	platformPos.x = x;

	//platformPos.y 

	ballPos += ballVelocity * diff;

	if (ballPos.x > 800.f - 5.f)
	{
		ballPos.x = 800.f - 5.f;
		ballVelocity.x *= -1.f;
	}
	else if (ballPos.x < 0.f)
	{
		ballPos.x = 0.f;
		ballVelocity.x *= -1.f;
	}

	if (ballPos.y > 500.f - 5.f)
	{
		ballPos.y = 500.f - 5.f;
		ballVelocity.y *= -1.f;
	}
	else if (ballPos.y < 0.f)
	{
		ballPos.y = 0.f;
		ballVelocity.y *= -1.f;
	}

	for (auto& block : blockGroup)
	{
		if (block.crushed)
			continue;

		float modX = 1.f, modY = 1.f;
		if (circleRect(ballPos.x, ballPos.y, 5.f, block.rect.x, block.rect.y, block.rect.w, block.rect.h, modX, modY))
		{
			block.crushed = true;
			ballVelocity.x *= modX;
			ballVelocity.y *= modY;
		}
	}

	float modX = 1.f, modY = 1.f;
	if (circleRect(ballPos.x, ballPos.y, 5.f, platformPos.x, platformPos.y, platformTexture->getWidth(), platformTexture->getHeight(), modX, modY))
	{
		float overlap = (ballPos.x - platformPos.x) / platformTexture->getWidth();
		overlap = std::clamp(overlap, 0.35f, 0.65f);
		float radOverlap = deg2rad(overlap * 180.f);
		
		Vec2d rotated = Vec2d(-1.f, 0.f).rotate(radOverlap);
		ballVelocity = fuse(rotated.norm(), ballVelocity.norm(), overlap).norm() * ballVelocity.length();
	}
}

void GameScreen::Load(SDL_Renderer* renderer)
{
	if (!platformTexture)
	{
		platformTexture = std::make_shared<Texture>(renderer);
		platformTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\platform.png");
	}

	if (!ballTexture)
	{
		ballTexture = std::make_shared<Texture>(renderer);
		ballTexture->loadFromFile("D:\\Breakout\\BreakoutProject\\Assets\\ball.png");
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

	ballPos = { 400, 350 };
	platformPos = { 400, 360 };
	ballVelocity = { -0.12f, -0.2f };
}

void GameScreen::Clear()
{
	blockTexture->free();
	platformTexture->free();
	ballTexture->free();
}

GameScreen::Vec2d GameScreen::fuse(Vec2d first, Vec2d second, float factor)
{
	auto mix = [](float a, float b, float factor) {
		auto min = [](float a, float b) { return a < b ? a : b; };
		auto max = [](float a, float b) { return a > b ? a : b; };
		float maxVal = max(a, b);
		float minVal = min(a, b);
		return minVal + (maxVal - minVal) * factor;
	};

	return { mix(first.x, second.x, factor), mix(first.y, second.y, factor) };
}

GameScreen::GameScreen::Vec2d& GameScreen::Vec2d::operator*=(const Vec2d& val)
{
	x += val.x;
	y *= val.y;
	return *this;
}

GameScreen::GameScreen::Vec2d& GameScreen::Vec2d::operator+=(const Vec2d& val)
{
	x += val.x;
	y += val.y;
	return *this;
}

GameScreen::Vec2d GameScreen::Vec2d::operator+(const Vec2d& val)
{
	return { x + val.x, y + val.y };
}

GameScreen::Vec2d GameScreen::Vec2d::operator-(const Vec2d& val)
{
	return { x - val.x, y - val.y };
}

GameScreen::Vec2d GameScreen::Vec2d::operator*(const float& val)
{
	return { x * val, y * val };
}

GameScreen::Vec2d GameScreen::Vec2d::operator*(const Vec2d& val)
{
	return { x * val.x, y * val.y };
}

GameScreen::Vec2d GameScreen::Vec2d::rotate(float angle) const
{
	auto denoise = [](float value) {
		const float Eps = 1.e-6;
		return abs(value) > Eps ? value : 0.f;
	};

	float sn = denoise(sin(angle));
	float cs = denoise(cos(angle));
	float px = x * cs - y * sn;
	float py = x * sn + y * cs;
	return { px, py };
}

float GameScreen::Vec2d::length() const
{
	return sqrt(x * x + y * y);
}

GameScreen::Vec2d GameScreen::Vec2d::norm()
{
	const float len = length();
	return { x / len, y / len };
}

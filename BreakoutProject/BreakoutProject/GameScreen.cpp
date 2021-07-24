#include "GameScreen.h"
#include "Signal.h"
#include "ParticleExample.h"
#include <algorithm>

float deg2rad(float degrees) {
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
	float distance = distX * distX + distY * distY;

	// if the distance is less than the radius, collision!
	if (distance <= radius*radius) {
		return true;
	}
	return false;
}

bool overlaps(SDL_Rect left, SDL_Rect right)
{
	return right.x + right.w < left.x + left.w
		&& right.x > left.x && right.y > left.y
		&& right.y + right.h < left.y + left.h;
}


GameScreen::GameScreen(std::string title) : Screen(title), velocityLimit(0.f)
{
}

void GameScreen::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 170, 200, 220, 255);
	SDL_RenderClear(renderer);

	if (state == GameState::STATE_LOSE)
	{
		loseTexture->render(windowSize.x/2 - loseTexture->getWidth()/2, windowSize.y/2.5f);
		return;
	}
	else if (state == GameState::STATE_WIN)
	{
		winTexture->render(windowSize.x / 2 - winTexture->getWidth() / 2, windowSize.y / 2.5f);
		return;
	}

	for (auto& block : blockGroup)
	{
		if (!block.crushed)
			blockTexture->render(block.rect.x, block.rect.y);
	}
	dustParticle->setRenderer(renderer);                   // set the renderer
	dustParticle->draw();    // you have to draw it in each loop
	burningBallParticle->setRenderer(renderer);
	burningBallParticle->draw();

	platformTexture->render(platformPos.x, platformPos.y, NULL, scale);
	Vec2d rotated = Vec2d(-1.f, 0.f).rotate(deg2rad(90.f)) * 100.f;
	ballTexture->render(ballPos.x, ballPos.y, NULL, scale);

	if (0)
	{
		SDL_SetRenderDrawColor(renderer, 0, 120, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, ballPos.x + ballVelocity.x * 100.f, ballPos.y + ballVelocity.y * 100.f, ballPos.x, ballPos.y);
		SDL_RenderDrawLine(renderer, platformPos.x, platformPos.y, platformPos.x + rotated.x, platformPos.y + rotated.y);
	}

}

void GameScreen::HandleEvents(float diff)
{
	SDL_Event evt;    // no need for new/delete, stack is fine

// event loop and draw loop are separate things, don't mix them
	while (SDL_PollEvent(&evt)) {
		// quit on close, window close, or 'escape' key hit
		if (evt.type == SDL_QUIT ||
			(evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_CLOSE) ||
			(evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE))
		{
			Signal().Send(Signal::Code::EXIT, 0);
		}

		if (state == GameState::STATE_PREGAME && evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT)
		{
			state = GameState::STATE_IN_GAME;
			SDL_CaptureMouse(SDL_TRUE);
			SDL_ShowCursor(SDL_FALSE);
		}

		if ((evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_SPACE)) {
			enableCheat = !enableCheat;
		}
		if ((evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_F2)) {
			auto itr = std::find_if(blockGroup.begin(), blockGroup.end(), [](Block& val) { return !val.crushed; });
			
			ballVelocity = (Vec2d(itr->rect.x, itr->rect.y) - ballPos).norm() * ballVelocity.length();
		}

		if (state == GameState::STATE_LOSE && evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT)
		{
			state = GameState::STATE_PREGAME;
		}
		if (state == GameState::STATE_WIN && evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == SDL_BUTTON_LEFT)
		{
			SDL_CaptureMouse(SDL_FALSE);
			SDL_ShowCursor(SDL_TRUE);
			state = GameState::STATE_PREGAME;
			for (auto& block : blockGroup)
			{
				block.crushed = false;
			}
			Reset();
			Signal().Send(Signal::Code::NEXT_SCREEN, 0);
		}
		
	}

	if (state != GameState::STATE_IN_GAME)
		return;
	
	int x = 0, y = 0;
	SDL_GetMouseState(&x, &y);
	platformPos.x = x - platformTexture->getWidth()*scale/2;
	
	/*if (overlaps({ 0, 0, 800, 500 }, SDL_Rect(ballPos.x, ballPos.y, ballTexture->getWidth(), ballTexture->getHeight())))
	{
		Vec2d gravity = { 0.f, 0.f };
		gravity.y = 0.005f;
		ballVelocity = ballVelocity + gravity * diff;
	}*/
	
	// HERE STARTS UPDATE
	const Vec2d prevBallPos = ballPos;
	ballPos += ballVelocity * diff;

	// TODO needs rework ?
	if (ballPos.x > windowSize.x - 5.f)
	{
		ballPos = prevBallPos;
		ballVelocity.x *= -1.f;
	}
	else if (ballPos.x < 0.f)
	{
		ballPos = prevBallPos;
		ballVelocity.x *= -1.f;
	}

	if (ballPos.y > windowSize.y - 5.f)
	{
		//TODO failed the ball. Reset?
		//ballPos = prevBallPos;
		//ballVelocity.y *= -1.f;
		Reset();
		state = GameState::STATE_LOSE;
	}
	else if (ballPos.y < 0.f)
	{
		ballPos = prevBallPos;
		ballVelocity.y *= -1.f;
	}

	if (enableCheat)
	{
		if (!burningBallParticle->isActive())
		{
			burningBallParticle->resetSystem();
		}

		burningBallParticle->setPosition(ballPos.x + 5, ballPos.y + 5);
		burningBallParticle->setGravity(Vec2(-1 * ballVelocity.x, -1 * ballVelocity.y));
	}
	else {
		burningBallParticle->stopSystem();
	}

	int aliveBlocks = blockGroup.size();
	for (auto& block : blockGroup)
	{
		if (block.crushed)
		{
			--aliveBlocks;
			continue;
		}

		float modX = 1.f, modY = 1.f;
		if (circleRect(ballPos.x, ballPos.y, 5.f, block.rect.x, block.rect.y, block.rect.w, block.rect.h, modX, modY))
		{
			block.crushed = true;
			PlayDustParticles();

			//TODO create control from keyboard
			if (!enableCheat)
			{
				ballPos = prevBallPos;
				ballVelocity.x *= modX;
				ballVelocity.y *= modY;
			}
		}
	}

	if (!aliveBlocks)
	{
		state = GameState::STATE_WIN;
	}

	float modX = 1.f, modY = 1.f;
	if (circleRect(ballPos.x, ballPos.y, 5.f, platformPos.x, platformPos.y, platformTexture->getWidth() * scale, 3.f, modX, modY))
	{
		float overlap = (ballPos.x - platformPos.x) / platformTexture->getWidth();
		overlap = std::clamp(overlap, 0.35f, 0.65f);
		float radOverlap = deg2rad(overlap * 180.f);
		
		Vec2d rotated = Vec2d(-1.f, 0.f).rotate(radOverlap);
		ballPos = prevBallPos;
		ballVelocity *= { modX, modY };
		const float ballVelocityLength = ballVelocity.length();
		ballVelocity = fuse(rotated.norm(), ballVelocity.norm(), overlap).norm() * ballVelocityLength;
		ballVelocity *= ballVelocityLength < velocityLimit ? 1.05f : 1.f;
	}
}

void GameScreen::PlayDustParticles()
{
	if (!dustParticle->isActive())
	{
		dustParticle->resetSystem();
		dustParticle->setPosition(ballPos.x, ballPos.y);
	}
}

void GameScreen::Load(SDL_Renderer* renderer)
{
	{
		int width = 0, height = 0;
		SDL_GetRendererOutputSize(renderer, &width, &height);
		windowSize = { static_cast<float>(width), static_cast<float>(height) };
	}

	if (!platformTexture)
	{
		platformTexture = std::make_shared<Texture>(renderer);
		platformTexture->loadFromFile("platform.png");
	}

	if (!ballTexture)
	{
		ballTexture = std::make_shared<Texture>(renderer);
		ballTexture->loadFromFile("ball.png");
	}

	if (!dustTexture)
	{
		dustTexture = std::make_shared<Texture>(renderer);
		dustTexture->loadFromFile("dust.png");
	}

	if (!winTexture)
	{
		winTexture = std::make_shared<Texture>(renderer);
		winTexture->loadFromFile("win.png");
	}

	if (!loseTexture)
	{
		loseTexture = std::make_shared<Texture>(renderer);
		loseTexture->loadFromFile("lose.png");
	}

	if (!blockTexture)
	{
		blockTexture = std::make_shared<Texture>(renderer);
		blockTexture->loadFromFile("blueblock.png");
	}

	const int blocksPerLine = 19;
	const int blockNumber = blocksPerLine * 6;
	blockGroup.reserve(blockNumber);
	for (int i = 0; i < blockNumber; i++)
	{
		int x = i % blocksPerLine;
		int y = i / blocksPerLine;
		blockGroup.push_back({ x * (blockTexture->getWidth() + 2) + 96, y * (blockTexture->getHeight() + 2) + 16,
			blockTexture->getWidth(), blockTexture->getHeight() });
	}

	dustParticle = std::make_shared<ParticleExample>(); // create a new particle system pointer
	dustParticle->setStyle(ParticleExample::EXPLOSION);    // set the example effects
	dustParticle->setStartSpin(0);
	dustParticle->setStartSpinVar(90);
	dustParticle->setEndSpin(90);
	dustParticle->setStartSpinVar(90);
	dustParticle->setTexture(dustTexture->getTexture());
	dustParticle->stopSystem();

	burningBallParticle = std::make_shared<ParticleExample>(); // create a new particle system pointer
	burningBallParticle->setStyle(ParticleExample::FIRE);    // set the example effects
	//burningBallParticle->setEmitterMode(ParticleSystem::Mode::RADIUS);
	burningBallParticle->setStartSpin(0);
	burningBallParticle->setStartSpinVar(90);
	burningBallParticle->setEndSpin(90);
	burningBallParticle->setStartSpinVar(90);
	burningBallParticle->setTexture(dustTexture->getTexture());
	//burningBallParticle->setRotationIsDir(true);
	//burningBallParticle->setAngle(0.f);
	burningBallParticle->stopSystem();

	Reset();
	velocityLimit = ballVelocity.length() * 2.66f;
	scale = 1.5f;
}

void GameScreen::Clear()
{
	blockTexture->free();
	platformTexture->free();
	ballTexture->free();
	dustTexture->free();
	winTexture->free();
	loseTexture->free();
}

void GameScreen::Reset()
{
	ballPos = { 400, 350 };
	platformPos = { 400.f - platformTexture->getWidth()*scale/2, 360 };
	ballVelocity = { -0.12f, -0.2f };
}

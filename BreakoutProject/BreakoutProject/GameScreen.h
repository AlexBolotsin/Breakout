#pragma once
#include "Screen.h"
#include "Texture.h"
#include "Vec2d.h"
#include <memory>
#include <vector>

class ParticleExample;

class GameScreen :
    public Screen
{
    struct Block {
        SDL_Rect rect;
        bool crushed = false;
	};

	enum class GameState {
		STATE_NONE,
		STATE_PREGAME,
		STATE_IN_GAME,
        STATE_WIN,
        STATE_LOSE
	} state = GameState::STATE_PREGAME;

    std::vector<Block> blockGroup;

    std::shared_ptr<Texture> platformTexture;
	std::shared_ptr<Texture> blockTexture;
	std::shared_ptr<Texture> ballTexture;
	std::shared_ptr<Texture> dustTexture;
	std::shared_ptr<Texture> winTexture;
	std::shared_ptr<Texture> loseTexture;

    Vec2d platformPos;
    Vec2d ballPos;
	Vec2d ballVelocity;
    Vec2d windowSize;
    float velocityLimit;
    
	float scale = 1.f;
    bool enableCheat = false;

    std::shared_ptr<ParticleExample> dustParticle;
    std::shared_ptr<ParticleExample> burningBallParticle;

public:

    GameScreen(std::string title);
    void Draw(SDL_Renderer* renderer) override;
    void HandleEvents(float diff) override;

	void PlayDustParticles();

	void Load(SDL_Renderer* renderer) override;
    void Clear() override;

private:
    void Reset();
};
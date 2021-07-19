#pragma once
#include "Screen.h"
#include "Texture.h"
#include <memory>
#include <vector>

class GameScreen :
    public Screen
{
    struct Block {
        SDL_Rect rect;
    };
    std::vector<Block> blockGroup;

    std::shared_ptr<Texture> platformTexture;
	std::shared_ptr<Texture> blockTexture;
    int mousePosition;
public:

    GameScreen(std::string title);
    void Draw(SDL_Renderer* renderer) override;
    void HandleEvents() override;
    void Load(SDL_Renderer* renderer) override;
    void Clear() override;
};


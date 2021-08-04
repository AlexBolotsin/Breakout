#pragma once
#include <string>
#include <SDL.h>

class Screen
{
	std::string _title;

public:
	Screen(std::string title);

	virtual void HandleEvents(float diff) = 0;
	virtual void Load(SDL_Renderer* renderer) = 0;
	virtual void Draw(SDL_Renderer* renderer) = 0;
	virtual void Clear() = 0;

	virtual ~Screen() { }
};


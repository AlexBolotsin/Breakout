#pragma once

#include <SDL.h>
#include <string>

class Texture
{
public:
	//Initializes variables
	Texture(SDL_Renderer* _renderer);

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, float scale = 1.f);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	SDL_Texture* getTexture() { return mTexture; }
private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
	SDL_Renderer* renderer;
};


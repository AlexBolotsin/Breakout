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
        bool crushed = false;
    };

    std::vector<Block> blockGroup;

    std::shared_ptr<Texture> platformTexture;
	std::shared_ptr<Texture> blockTexture;
	std::shared_ptr<Texture> ballTexture;

    struct Vec2d
    {
        float x = 0;
        float y = 0;

		Vec2d& operator+=(const Vec2d& val);
		Vec2d& operator*=(const Vec2d& val);
        Vec2d operator+(const Vec2d& val);
		Vec2d operator-(const Vec2d& val);
        Vec2d operator*(const Vec2d& val);
		Vec2d operator*(const float& val);
		Vec2d rotate(float angle) const;
		float length() const;

		Vec2d norm();
	};
    Vec2d platformPos;
    Vec2d ballPos;
	Vec2d ballVelocity;

	struct Curve {
		std::vector<Vec2d> points;

		Curve();

		Vec2d getBezierPoint(float t) {
			std::vector<Vec2d> tmpPoints(points);
			int i = tmpPoints.size() - 1;
			while (i > 0) {
				for (int k = 0; k < i; k++)
					tmpPoints[k] = tmpPoints[k] + (tmpPoints[k + 1] - tmpPoints[k]) * t;
				i--;
			}
			Vec2d answer = tmpPoints[0];
			return answer;
		}
	};

public:

    GameScreen(std::string title);
    void Draw(SDL_Renderer* renderer) override;
    void HandleEvents(float diff) override;
    void Load(SDL_Renderer* renderer) override;
    void Clear() override;
private:
	Vec2d fuse(Vec2d first, Vec2d second, float factor);
};


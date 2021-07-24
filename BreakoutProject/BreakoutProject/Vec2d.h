#pragma once

class Vec2d
{
public:
	float x;
	float y;

	Vec2d(float _x = 0.f, float _y = 0.f);

	Vec2d& operator+=(const Vec2d& val);
	Vec2d& operator*=(const Vec2d& val);
	Vec2d& operator*=(const float& val);
	Vec2d operator+(const Vec2d& val);
	Vec2d operator-(const Vec2d& val);
	Vec2d operator*(const Vec2d& val);
	Vec2d operator*(const float& val);
	Vec2d rotate(float angle) const;
	float length() const;
	Vec2d norm();
};

Vec2d fuse(Vec2d first, Vec2d second, float factor);
Vec2d operator+(const Vec2d& left, const Vec2d& right);
#include "Vec2d.h"
#include <math.h>

Vec2d::Vec2d(float _x, float _y) : x(_x), y(_y)
{
}

Vec2d& Vec2d::operator*=(const Vec2d& val)
{
	x *= val.x;
	y *= val.y;
	return *this;
}

Vec2d& Vec2d::operator*=(const float& val)
{
	x *= val;
	y *= val;
	return *this;
}

Vec2d& Vec2d::operator+=(const Vec2d& val)
{
	x += val.x;
	y += val.y;
	return *this;
}

Vec2d Vec2d::operator+(const Vec2d& val)
{
	return { x + val.x, y + val.y };
}

Vec2d Vec2d::operator-(const Vec2d& val)
{
	return { x - val.x, y - val.y };
}

Vec2d Vec2d::operator*(const float& val)
{
	return { x * val, y * val };
}

Vec2d Vec2d::operator*(const Vec2d& val)
{
	return { x * val.x, y * val.y };
}

Vec2d Vec2d::rotate(float angle) const
{
	auto denoise = [](float value) {
		const float Eps = 1.e-6;
		return fabsf(value) > Eps ? value : 0.f;
	};

	float sn = denoise(sin(angle));
	float cs = denoise(cos(angle));
	float px = x * cs - y * sn;
	float py = x * sn + y * cs;
	return { px, py };
}

float Vec2d::length() const
{
	return sqrt(x * x + y * y);
}

Vec2d Vec2d::norm()
{
	const float len = length();
	return { x / len, y / len };
}

Vec2d fuse(Vec2d first, Vec2d second, float factor)
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


Vec2d operator+(const Vec2d& left, const Vec2d& right)
{
	return { left.x + right.x, left.y + right.y };
}

#pragma once

#include "Vec2d.h"
#include <vector>

struct Curve {
	std::vector<Vec2d> points;

	Curve();

	Vec2d getBezierPoint(float t);
};

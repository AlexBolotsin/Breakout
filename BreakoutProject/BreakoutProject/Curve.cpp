#include "Curve.h"

Curve::Curve()
{
}

Vec2d Curve::getBezierPoint(float t) {
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

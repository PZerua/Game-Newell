/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <ostream>

namespace math 
{

struct vec2
{
	float x, y;

	vec2() : x(0), y(0) {}
	vec2(float x, float y) : x(x), y(y) {}

	float dot(const vec2 &vec) { return (x * vec.x + y * vec.y); }
};

} // namespace math
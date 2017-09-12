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

// Overload operators
vec2 operator+(const vec2 &left, const vec2 &right) { return vec2(left.x + right.x, left.y + right.y); }
vec2 operator-(const vec2 &left, const vec2 &right) { return vec2(left.x - right.x, left.y - right.y); }
vec2 operator*(const vec2 &left, const vec2 &right) { return vec2(left.x * right.x, left.y * right.y); }
vec2 operator/(const vec2 &left, const vec2 &right) { return vec2(left.x / right.x, left.y / right.y); }
vec2 operator+=(vec2 &left, const vec2 &right) { left = left + right; return left; }
vec2& operator-=(vec2 &left, const vec2 &right) { left = left - right; return left; }
vec2& operator*=(vec2 &left, const vec2 &right) { left = left * right; return left; }
vec2& operator/=(vec2 &left, const vec2 &right) { left = left / right; return left; }
bool operator==(const vec2 &left, const vec2 &right) { return (left.x == right.x) && (left.y == right.y); }
bool operator!=(const vec2 &left, const vec2 &right) { return (left.x != right.x) || (left.y != right.y); }
std::ostream& operator<<(std::ostream& os, const vec2 &vec) { return os << "[" << vec.x << ", " << vec.y << "]"; }

} // namespace math
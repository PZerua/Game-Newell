/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <ostream>

namespace math 
{

struct vec3
{
	float x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	float dot(const vec3 &vec) { return (x * vec.x + y * vec.y + z * vec.z); }
	vec3 cross(const vec3 &vec) { return vec3(y * vec.z - vec.y * z, -(x * vec.z - vec.x * z), x * vec.y - vec.x * y); }
};

// Overload operators
vec3 operator+(const vec3 &left, const vec3 &right) { return vec3(left.x + right.x, left.y + right.y, left.z + right.z); }
vec3 operator-(const vec3 &left, const vec3 &right) { return vec3(left.x - right.x, left.y - right.y, left.z - right.z); }
vec3 operator*(const vec3 &left, const vec3 &right) { return vec3(left.x * right.x, left.y * right.y, left.z * right.z); }
vec3 operator/(const vec3 &left, const vec3 &right) { return vec3(left.x / right.x, left.y / right.y, left.z / right.z); }
vec3 operator+=(vec3 &left, const vec3 &right) { left = left + right; return left; }
vec3 operator-=(vec3 &left, const vec3 &right) { left = left - right; return left; }
vec3 operator*=(vec3 &left, const vec3 &right) { left = left * right; return left; }
vec3 operator/=(vec3 &left, const vec3 &right) { left = left / right; return left; }
bool operator==(const vec3 &left, const vec3 &right) { return (left.x == right.x) && (left.y == right.y) && (left.z == right.z); }
bool operator!=(const vec3 &left, const vec3 &right) { return (left.x != right.x) || (left.y != right.y) || (left.z != right.z); }
std::ostream& operator<<(std::ostream& os, const vec3 &vec) { return os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]"; }

} // namespace math
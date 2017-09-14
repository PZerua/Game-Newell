/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
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

} // namespace math
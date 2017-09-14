/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <ostream>

namespace math
{

struct vec4
{
	float x, y, z, w;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	float dot(const vec4 &vec) { return (x * vec.x + y * vec.y + z * vec.z + w * vec.w); }
};

} // namespace math
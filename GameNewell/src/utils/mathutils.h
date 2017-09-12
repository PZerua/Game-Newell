/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <cmath>

namespace math
{

	const float PI = 3.14159265358979f;

	float toRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

} // namespace math
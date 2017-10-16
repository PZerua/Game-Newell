/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/math/math.h>

namespace gfx
{

class Renderable2D
{
protected:
	math::mat4 m_model{1.0f};

public:
	inline math::mat4 getModel() const { return m_model; }

protected:
	Renderable2D(math::vec2 position, math::vec2 size)
	{
		m_model.translate(math::vec3(position, 0.0f));
		m_model.scale(math::vec3(size, 1.0f));
	}
};

} // namespace gfx
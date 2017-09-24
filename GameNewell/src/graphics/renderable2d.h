/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <memory>

#include <src/math/math.h>
#include <src/graphics/buffers/vertexarray.h>
#include <src/graphics/buffers/vertexbuffer.h>
#include <src/graphics/buffers/indexbuffer.h>

namespace gfx
{

class Renderable2D
{
private:
	math::vec2 m_position;
	math::vec2 m_size;

public:
	Renderable2D(math::vec2 position, math::vec2 size) : m_position(position), m_size(size) {}

	inline math::vec2 getPosition() const { return m_position; }
	inline math::vec2 getSize() const { return m_size; }

};

} // namespace gfx
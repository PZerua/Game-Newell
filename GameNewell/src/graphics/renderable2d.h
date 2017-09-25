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
private:
	math::vec2 m_position;
	math::vec2 m_size;	
	unsigned m_textureIndex;

public:
	Renderable2D(math::vec2 position, math::vec2 size, unsigned textureIndex) : m_position(position), m_size(size), m_textureIndex(textureIndex) {}

	inline math::vec2 getPosition() const { return m_position; }
	inline math::vec2 getSize() const { return m_size; }
	inline unsigned getTextureIndex() const { return m_textureIndex; }

};

} // namespace gfx
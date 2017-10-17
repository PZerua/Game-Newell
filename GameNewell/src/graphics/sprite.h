/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/graphics/renderable2d.h>

namespace gfx
{

class Sprite : public Renderable2D
{
private:
	unsigned m_textureIndex;

public:
	Sprite(math::vec2 position, math::vec2 size, unsigned textureIndex) : Renderable2D(position, size), m_textureIndex(textureIndex) {}

	inline unsigned getTextureIndex() { return m_textureIndex; }
};

} // namespace gfx
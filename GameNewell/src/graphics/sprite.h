/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <src/graphics/renderable2d.h>
#include <GL/glew.h>

namespace gfx
{

class Sprite : public Renderable2D
{
private:
	GLuint m_textureIndex;
	GLuint m_textureArrayId;

public:
	Sprite(math::vec2 position, math::vec2 size, std::pair<GLuint, GLuint> textureInfo) : Renderable2D(position, size), m_textureArrayId(textureInfo.first), m_textureIndex(textureInfo.second) {}

	inline GLuint getTextureArrayId() const { return m_textureArrayId; }
	inline GLuint getTextureIndex() const { return m_textureIndex; }
};

} // namespace gfx
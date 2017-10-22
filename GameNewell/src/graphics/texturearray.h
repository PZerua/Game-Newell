/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <map>

#include <src/graphics/texturebase.h>

namespace gfx
{

class TextureArray : public TextureBase
{

private:
	GLuint m_textureCount;
	GLuint m_maxLayers;
	std::map<std::string, GLuint> m_textures;

public:
	TextureArray(GLuint width, GLuint height);
	void bind() const;
	void unbind() const;
	GLuint getTexture(const char* filename);

	inline GLuint getTextureCount() const { return m_textureCount; }

private:
	bool addTexture(const char* filename);

};

} // namespace gfx
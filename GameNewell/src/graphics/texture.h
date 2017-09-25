/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <string>

#include <GL/glew.h>

namespace gfx
{

class Texture
{

#define TEX_ARRAY_DIMENSION 128

private:
	GLuint m_textureId;
	GLuint m_width;
	GLuint m_height;
	GLenum m_target;
	unsigned m_textureCount;

public:
	Texture(GLenum target);
	void bind() const ;
	void unbind() const;
	bool loadTexture(const char* filename);
	bool loadTextureArray(const char* filename);

	inline GLuint getId() const { return m_textureId; }
	inline GLuint getTextureCount() const { return m_textureCount; }
	inline GLenum getTarget() const { return m_target; }

private:
	struct TexInfo
	{
		GLint width;
		GLint height;
		GLint bpp;
		GLubyte* data;
	};

	TexInfo* loadTEX(const char* filename);
};

} // namespace gfx
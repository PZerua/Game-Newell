/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "texture.h"

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION

#include <STB/stb_image.h>
#include <iostream>

namespace gfx
{

Texture::Texture(GLenum target) 
	: m_width(0), m_height(0), m_textureCount(0)
{
	m_target = target;

	glGenTextures(1, &m_textureId);
	glBindTexture(m_target, m_textureId);

	if (m_target == GL_TEXTURE_2D_ARRAY)
	{
		glTexStorage3D(m_target,
			1,											// Mipmaps
			GL_RGBA8,									// Internal format
			TEX_ARRAY_DIMENSION, TEX_ARRAY_DIMENSION,   // Width, Height
			100											// Number of layers
		);
	}
}

bool Texture::loadTexture(const char* filename)
{
	std::string str = filename;
	str = str.substr(str.size() - 4, 4);

	if (str != ".png")
		return false;
	
	TexInfo* texinfo = loadTEX(filename);
	if (!texinfo)
		return false;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(m_target, m_textureId);

	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(m_target, 0, texinfo->bpp, texinfo->width, texinfo->height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, texinfo->data);

	m_width = texinfo->width;
	m_height = texinfo->height;

	delete texinfo->data;
	delete texinfo;

	return true;
}

bool Texture::loadTextureArray(const char* filename)
{
	std::string str = filename;
	str = str.substr(str.size() - 4, 4);

	if (str != ".png")
		return false;

	TexInfo* texinfo = loadTEX(filename);
	if (!texinfo)
		return false;

	if (texinfo->width != TEX_ARRAY_DIMENSION || texinfo->height != TEX_ARRAY_DIMENSION)
	{
		std::cout << "The texture submited for texture array doesn't have a valid size" << std::endl;
		return false;
	}

	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexSubImage3D(m_target,
		0,										// Mipmap number
		0, 0, m_textureCount++,					// Xoffset, Yoffset, Zoffset
		texinfo->width, texinfo->height, 1,     // Width, Height, Depth
		GL_RGBA,								// Format
		GL_UNSIGNED_BYTE,						// Type
		texinfo->data);							// Pointer to data

	m_width = texinfo->width;
	m_height = texinfo->height;

	delete texinfo->data;
	delete texinfo;

	return true;
}

void Texture::bind() const
{
	glBindTexture(m_target, m_textureId);
}

void Texture::unbind() const
{
	glBindTexture(m_target, 0);
}

Texture::TexInfo* Texture::loadTEX(const char* filename)
{
	TexInfo* texinfo = new TexInfo;
	texinfo->data = stbi_load(filename, &texinfo->width, &texinfo->height, &texinfo->bpp, 0);

	if (texinfo->width <= 0 || texinfo->height <= 0 || texinfo->data == NULL)
	{
		std::cout << "Error loading texture: " << filename << std::endl;
		delete texinfo;
		return NULL;
	}

	return texinfo;
}

} // namespace gfx
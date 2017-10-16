/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include "texturearray.h"

#include <iostream>

namespace gfx
{

TextureArray::TextureArray()
	: TextureBase(), m_textureCount(0)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&m_maxLayers);

	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,											// Mipmaps
		GL_RGBA8,									// Internal format
		TEX_ARRAY_DIMENSION, TEX_ARRAY_DIMENSION,   // Width, Height
		m_maxLayers									// Number of layers
	);
}

GLuint TextureArray::getTexture(const char *filename)
{
	// Get id from the map is the texture is already uploaded
	if (m_textures.count(filename))
		return m_textures[filename];
	else
	{
		// Upload the texture, add it to the map and get the current id
		if(addTexture(filename))
			return m_textureCount - 1;
		else return -1;
	}
}

bool TextureArray::addTexture(const char* filename)
{
	if (m_textureCount >= m_maxLayers)
	{
		std::cout << "You can't submit more textures to this texture array" << std::endl;
		return false;
	}

	std::string str = filename;
	str = str.substr(str.size() - 4, 4);

	if (str != ".png")
	{
		std::cout << "The image is not in .png format" << std::endl;
		return false;
	}

	TexInfo* texinfo = loadTEX(filename);
	if (!texinfo)
	{
		std::cout << "Error reading texture: " << filename << std::endl;
		return false;
	}

	if (texinfo->width != TEX_ARRAY_DIMENSION || texinfo->height != TEX_ARRAY_DIMENSION)
	{
		std::cout << "The texture submited for texture array doesn't have a valid size" << std::endl;
		return false;
	}

	// Add to the map <textureName, index>
	m_textures[filename] = m_textureCount;

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
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

void TextureArray::bind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
}

void TextureArray::unbind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

} // namespace gfx
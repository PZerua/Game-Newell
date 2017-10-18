/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "textureManager.h"

#include <src/utils/fileutils.h>
#include <iostream>

namespace gfx
{

GLuint TextureManager::getTexture(const char *filename)
{
	unsigned width, height;
	math::vec2 size;

	if (utils::getTextureSize(filename, width, height))
	{
		size.x = (float)width;
		size.y = (float)height;

		if (!m_textureArrays.count(size))
			m_textureArrays[size] = std::make_unique<TextureArray>(width, height);

		return m_textureArrays[size]->getTexture(filename);
	}
	else return -1;
}

} // namespace gfx
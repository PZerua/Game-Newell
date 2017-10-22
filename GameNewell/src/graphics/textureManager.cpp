/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include "textureManager.h"

#include <src/utils/fileutils.h>
#include <iostream>

namespace gfx
{

std::tuple<GLuint, GLuint, bool> TextureManager::getTexture(const char *filename)
{
    unsigned width, height;
    math::vec2 size;
    bool newTextureArray = false;

    if (utils::getTextureSize(filename, width, height))
    {
        size.x = (float)width;
        size.y = (float)height;

        if (!m_textureArrays.count(size))
        {
            newTextureArray = true;
            m_textureArrays[size] = std::make_unique<TextureArray>(width, height);
        }

        return {m_textureArrays[size]->getId(), m_textureArrays[size]->getTexture(filename), newTextureArray};
    }
    else return {0, -1, false};
}

} // namespace gfx

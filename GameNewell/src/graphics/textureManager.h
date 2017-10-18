/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <map>
#include <memory>

#include <src/graphics/texturearray.h>
#include <src/math/vec2.h>

namespace gfx
{

class TextureManager
{
public:
	GLuint getTexture(const char* filename);

private:
	// Each texture array is defined by its size
	std::map<math::vec2, std::unique_ptr<TextureArray>> m_textureArrays;
};

} // namespace gfx
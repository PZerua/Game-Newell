/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
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
	std::tuple<GLuint, GLuint, bool> getTexture(const char* filename);

private:
	// Each texture array is defined by its size
	std::map<math::vec2, std::unique_ptr<TextureArray>> m_textureArrays;
};

} // namespace gfx
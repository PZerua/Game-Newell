/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <src/graphics/renderable2d.h>

namespace gfx
{

class Sprite : public Renderable2D
{
private:


public:
    Sprite(math::vec2 position, math::vec2 size, TextureArrayInfo textureInfo, GLuint shaderId) : Renderable2D(position, size, textureInfo, shaderId) {}
};

} // namespace gfx

/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <string>
#include <map>

#include <src/graphics/renderable2d.h>
#include <src/graphics/texturearray.h>
#include <GL/glew.h>

namespace gfx
{

class Text
{
private:
    // This contains all the different fonts loaded and their texture id, they are shared for all texts
    static std::map<std::string, GLuint> m_fonts;

    // The text we want to render
    std::string m_text;

public:
    Text(const char *text, math::vec2 position, math::vec2 size, const std::string &fontName = "default");
};

} // namespace gfx

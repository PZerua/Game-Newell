/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#include <src/graphics/text.h>

#include <fstream>
#include <sstream>

namespace gfx
{

std::map<std::string, GLuint> Text::m_fonts;

Text::Text(const char *text, math::vec2 position, math::vec2 size, const std::string &fontName) :
    m_text(text)
{
    // Add the font to the map if it is not loaded yet
    if (!m_fonts.count(fontName))
    {

    }
}

} // namespace gfx

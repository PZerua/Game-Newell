/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <string>
#include <map>
#include <memory>

#include <src/graphics/renderable2d.h>
#include <src/graphics/texture.h>

namespace gfx
{
	
// All character info contained in a BMFont txt file
struct Character
{
	unsigned short id;
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
	short xOffset;
	short yOffset;
	short xAdvance;
};

struct Font
{
	std::map<unsigned short, Character> characters;
	unsigned short lineHeight;
	unsigned short base;
	std::string name;
	unsigned m_textureIndex;

	Font(const std::string &name);
	void parse();
};

class Text
{
private:
	// This contains all the different fonts loaded, they are shared for all texts
	static std::map<std::string, std::unique_ptr<Font>> m_fonts;

	std::string m_text;

public:
	Text(const char *text, math::vec2 position, math::vec2 size, const std::string &fontName = "default");
};

} // namespace gfx
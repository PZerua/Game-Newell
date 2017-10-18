/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#include <src/graphics/text.h>

#include <fstream>
#include <sstream>

#include <src/utils/stringutils.h>

namespace gfx
{

std::map<std::string, std::unique_ptr<Font>> Text::m_fonts;

Font::Font(const std::string &name) 
	: name(name) 
{ 
	parse(); 
}

void Font::parse()
{
	std::ifstream file("data/fonts/" + name + ".fnt");

	if (file)
	{
		std::string line, tag, pair;

		while (getline(file, line))
		{
			std::istringstream iss(line);
			iss >> tag;

			if (tag == "common")
			{
				while (!iss.eof())
				{
					iss >> pair;
					auto keyValue = utils::getKeyValue(pair, "=");

					if (keyValue.first == "lineHeight")
						lineHeight = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "base")
						base = (unsigned short)std::stoi(keyValue.second);
				}
			}
			else if (tag == "char")
			{
				Character character;

				while (!iss.eof())
				{
					iss >> pair;
					auto keyValue = utils::getKeyValue(pair, "=");

					if (keyValue.first == "id")
						character.id = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "x")
						character.x = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "y")
						character.y = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "width")
						character.width = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "height")
						character.height = (unsigned short)std::stoi(keyValue.second);
					if (keyValue.first == "xoffset")
						character.xOffset = (short)std::stoi(keyValue.second);
					if (keyValue.first == "yoffset")
						character.yOffset = (short)std::stoi(keyValue.second);
					if (keyValue.first == "xadvance")
						character.xAdvance = (short)std::stoi(keyValue.second);
				}

				characters[character.id] = character;
			}
		}
	}
}

Text::Text(const char *text, math::vec2 position, math::vec2 size, const std::string &fontName) :
	m_text(text)
{
	// Add the font to the map if it is not loaded yet
	if (!m_fonts.count(fontName))
	{
		m_fonts[fontName] = std::make_unique<Font>(fontName);
	}
}

} // namespace gfx
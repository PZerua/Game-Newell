#include "TextureManager.h"

TextureManager *TextureManager::instance = NULL;

TextureManager::TextureManager()
{
	if (instance == NULL)
		instance = this;
}

Texture *TextureManager::getTexture(const char *path)
{
	auto pos = textureMan.find(std::string(path));

	Texture *texture = new Texture();
	if (pos == textureMan.end())
	{
		if (texture->load(path))
			textureMan[std::string(path)] = texture;
		else
		{
			std::cout << "Texture " << path << " not found" << std::endl;
			delete texture;
			return NULL;
		}
	}

	return textureMan[std::string(path)];
}
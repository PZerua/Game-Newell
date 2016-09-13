#include "texture.h"
#include "Framework.h"
#include "includes.h"
#include <cmath>
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture()
{
	width = 0;
	height = 0;
}

bool Texture::load(const char* filename)
{
	std::string str = filename;
	std::string ext = str.substr(str.size() - 4, 4);

	this->filename = filename;

	if (ext == ".png")
	{
		TEXInfo* texinfo = loadTEX(filename);
		if (texinfo == NULL)
			return false;

		this->filename = filename;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, texinfo->bpp, texinfo->width, texinfo->height, 0, (texinfo->bpp == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, texinfo->data);
	
		width = texinfo->width;
		height = texinfo->height;

		delete texinfo->data;
		delete texinfo;
		return true;
	}
	return false;
}

void Texture::bind()
{
	glEnable(GL_TEXTURE_2D); //enable the textures
	glBindTexture(GL_TEXTURE_2D, texture_id);	//enable the id of the texture we are going to use
}

void Texture::unbind()
{
	glDisable(GL_TEXTURE_2D); //disable the textures
	glBindTexture(GL_TEXTURE_2D, 0);	//disable the id of the texture we are going to use
}

void Texture::UnbindAll()
{
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Texture::TEXInfo* Texture::loadTEX(const char* filename)
{
	TEXInfo* texinfo = new TEXInfo;
	texinfo->data = stbi_load(filename, &texinfo->width, &texinfo->height, &texinfo->bpp, 0);

	if (texinfo->width <= 0 || texinfo->height <= 0 || texinfo->data == NULL)
	{
		delete texinfo;
		return NULL;
	}

	return texinfo;
}

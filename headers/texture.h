#pragma once

#include <map>
#include <string>
#include "includes.h"

// TEXTURE CLASS
class Texture
{
	typedef struct sTEXInfo //a general struct to store all the information about a TGA file
	{
		GLint width;
		GLint height;
		GLint bpp; //bits per pixel
		GLubyte* data; //bytes with the pixel information
	} TEXInfo;

public:
	GLuint texture_id; // GL id to identify the texture in opengl, every texture must have its own id
	float width;
	float height;
	std::string filename;

	Texture();
	void bind();
	void unbind();
	static void UnbindAll();

	bool load(const char* filename);
	void generateMipmaps();

protected:
	TEXInfo* loadTEX(const char* filename);
};
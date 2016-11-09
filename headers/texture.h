#pragma once

#include <map>
#include <string>
#include "includes.h"

class CTexture
{

public:
	GLuint texture_id;
	std::string filename;
	float width;
	float height;

	CTexture() : width(0), height(0) {}
	void bind();
	void unbind();
	static void UnbindAll();
	bool load(const char* filename);

private:
	struct STexInfo
	{
		GLint width;
		GLint height;
		GLint bpp; // bits per pixel
		GLubyte* data; // bytes with the pixel information
	};

	STexInfo* loadTEX(const char* filename);
};
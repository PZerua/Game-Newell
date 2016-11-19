/**
* (c) 2016 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include "includes.h"
#include <map>
#include <string>

class CTexture
{
public:
	GLuint m_texture_id;
	std::string m_filename;
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
		GLint bpp;
		GLubyte* data;
	};

	STexInfo* loadTEX(const char* filename);
};
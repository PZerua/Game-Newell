/**
* (c) 2017 Pablo Luis Garc�a. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/debugutils.h>
#include <iostream>

namespace utils
{

	void printGlErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "OpenGL error: " << std::hex << err << std::dec << std::endl;
		}
	}

} // namespace utils
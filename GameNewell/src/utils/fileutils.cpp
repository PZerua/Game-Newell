/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/fileutils.h>
#include <iostream>

namespace utils
{

std::string readFile(const std::string &filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		std::cout << "Error opening: " + filePath << std::endl;
		return "";
	}

	std::stringstream fileStringstream;
	fileStringstream << fileStream.rdbuf();

	fileStream.close();

	return fileStringstream.str();
}

} // namespace utils
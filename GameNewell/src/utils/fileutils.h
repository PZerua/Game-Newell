/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace utils
{

std::string readFile(const char *filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
		return "";

	std::stringstream fileStringstream;
	fileStringstream << fileStream.rdbuf();

	fileStream.close();

	return fileStringstream.str();
}

} // namespace utils
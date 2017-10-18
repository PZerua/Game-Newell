/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/fileutils.h>

#include <iostream>
#include <STB/stb_image.h>

namespace utils
{

std::string readFileString(const char *filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		std::cout << "Error opening: " << filePath << std::endl;
		return "";
	}

	std::stringstream fileStringstream;
	fileStringstream << fileStream.rdbuf();

	fileStream.close();

	return fileStringstream.str();
}

std::vector<unsigned char> readFile(const char *filePath)
{
	std::ifstream fileStream(filePath, std::ios::ate);

	unsigned size = (int)fileStream.tellg();

	if (!fileStream.is_open())
	{
		std::cout << "Error opening: " << filePath << std::endl;
		return {};
	}

	// Points to the beginning of the file
	fileStream.seekg(0, std::ios::beg);

	std::vector<unsigned char> data(size);

	fileStream.read((char *)&data[0], size);

	fileStream.close();

	return data;
}

bool getTextureSize(const char *fileName, unsigned &width, unsigned &height)
{
	std::ifstream fileStream(fileName, std::ios::binary);

	if (!fileStream.is_open())
	{
		std::cout << "Error opening: " << fileName << std::endl;
		return false;
	}
	
	fileStream.seekg(16);
	fileStream.read(reinterpret_cast<char*>(&width), 4);
	fileStream.read(reinterpret_cast<char*>(&height), 4);

	// They are in be
	width = _byteswap_ulong(width);
	height = _byteswap_ulong(height);

	return true;
}

} // namespace utils
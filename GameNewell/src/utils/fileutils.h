/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace utils
{

std::string readFileString(const char *filePath);
std::vector<unsigned char> readFile(const char *filePath);

} // namespace utils
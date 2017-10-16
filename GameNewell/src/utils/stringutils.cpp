/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under GPL v2 license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/stringutils.h>

namespace utils
{

std::pair<std::string, std::string> getKeyValue(const std::string &base, const std::string &divider)
{
	unsigned i = base.find(divider);
	return {base.substr(0, i), base.substr(i + 1)};
}

} // namespace utils
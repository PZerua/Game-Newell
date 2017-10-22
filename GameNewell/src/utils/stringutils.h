/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <string>
#include <utility>

namespace utils
{

std::pair<std::string, std::string> getKeyValue(const std::string &base, const std::string &divider);

} // namespace utils

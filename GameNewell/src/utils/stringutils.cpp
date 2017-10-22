/**
* (c) 2017 Pablo Luis García. All rights reserved.
* Released under MIT license. Read LICENSE for more details.
*/

#pragma once

#include <src/utils/stringutils.h>

namespace utils
{

std::pair<std::string, std::string> getKeyValue(const std::string &base, const std::string &divider)
{
    auto i = base.find(divider);
    return {base.substr(0, i), base.substr(i + 1)};
}

} // namespace utils

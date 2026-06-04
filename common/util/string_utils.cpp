#include "string_utils.h"

#include <algorithm>


namespace string_utils {

std::string to_lowercase(std::string s) {
    std::ranges::transform(s, s.begin(), [](const unsigned char c) { return std::tolower(c); });
    return s;
}

}  // namespace string_utils

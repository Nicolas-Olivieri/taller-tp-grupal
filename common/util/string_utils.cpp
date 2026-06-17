#include "string_utils.h"

#include <algorithm>


namespace string_utils {

std::string normalize(const std::string& s) {
    std::string result = s;
    trim_spaces(result);
    remove_accents(result);
    to_lowercase(result);
    return result;
}

void trim_spaces(std::string& s) {
    // borrar espacios al principio
    s.erase(s.begin(), std::ranges::find_if(s, [](const unsigned char c) { return not std::isspace(c); }));

    // colapsar espacios duplicados en el medio
    const auto r = std::ranges::unique(s, [](const unsigned char a, const unsigned char b) {
        return std::isspace(a) and std::isspace(b);
    });
    s.erase(r.begin(), s.end());

    // borrar posible espacio final único
    if (not s.empty() and std::isspace(s.back()))
        s.pop_back();
}

void remove_accents(std::string& s) {
    for (const auto& [accented, normalized]: accent_map) replace_all(s, accented, normalized);
}

void replace_all(std::string& s, const std::string& from, const std::string& to) {
    if (from.empty())
        return;

    size_t start = 0;
    while ((start = s.find(from, start)) != std::string::npos) {
        s.replace(start, from.length(), to);
        start += to.length();
    }
}

void to_lowercase(std::string& s) {
    std::ranges::transform(s, s.begin(), [](const unsigned char c) { return std::tolower(c); });
}

}  // namespace string_utils

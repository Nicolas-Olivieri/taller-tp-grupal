#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <unordered_map>


namespace string_utils {

static const std::unordered_map<std::string, std::string> accent_map = {
        {"á", "a"}, {"é", "e"}, {"í", "i"}, {"ó", "o"}, {"ú", "u"}, {"ü", "u"}, {"ñ", "n"},
        {"Á", "a"}, {"É", "e"}, {"Í", "i"}, {"Ó", "o"}, {"Ú", "u"}, {"Ü", "u"}, {"Ñ", "n"}};

std::string normalize(const std::string& s);

void trim_spaces(std::string& s);

void remove_accents(std::string& s);

void replace_all(std::string& s, const std::string& from, const std::string& to);

void to_lowercase(std::string& s);

}  // namespace string_utils


#endif  // STRING_UTILS_H

#include <iostream>
#include <map>
#include <toml.hpp>
#include "common/foo.h"
#include "common/toml_test.cpp"

int main() {
    try {
        auto root = toml::parse("../items.toml");

        auto equipables = toml::find<std::map<std::string, Equipable>>(root, "equipable");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return foo(6);
}

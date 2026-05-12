#include <string>

#include <toml.hpp>

struct Equipable {
    std::string name;
    std::string type;
    bool magic;
    uint8_t min_stat;
    uint8_t max_stat;
};

template <>
struct toml::from<Equipable> {
    static Equipable from_toml(const value& v) {
        return Equipable{
                toml::find<std::string>(v, "name"),
                toml::find<std::string>(v, "type"),
                toml::find<bool>(v, "magic"),
                toml::find<uint8_t>(v, "min_stat"),
                toml::find<uint8_t>(v, "max_stat"),
        };
    }
};

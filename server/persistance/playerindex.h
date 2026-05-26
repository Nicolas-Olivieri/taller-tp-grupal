#ifndef PLAYERINDEX_H
#define PLAYERINDEX_H

#include <cstdint>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct PlayerNotFound: public std::runtime_error {
    PlayerNotFound(): std::runtime_error("There is no player with this username.") {}
};

struct PlayerAlreadyExists: public std::runtime_error {
    PlayerAlreadyExists(): std::runtime_error("There already is a player with this username.") {}
};

// TODO: modificar la clase para considerar contraseñas si llegamos
class PlayerIndex {
private:
    std::mutex mutex;
    std::fstream file;
    std::unordered_map<std::string, uint32_t> index;

public:
    explicit PlayerIndex(const std::string& index_path);

    void add(const std::string& username, uint32_t offset);

    uint32_t get(const std::string& username);

    bool exists(const std::string& username) const;

    ~PlayerIndex();
};

#endif  // PLAYERINDEX_H

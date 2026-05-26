#include "playerindex.h"

#include <vector>

#include <arpa/inet.h>

PlayerIndex::PlayerIndex(const std::string& index_path):
        file(index_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::app) {

    open_file_correctly(index_path);
    load_index_map();

    file.clear();
}

void PlayerIndex::open_file_correctly(const std::string& index_path) {
    if (!file.is_open()) {
        file.clear();

        // fuerza la creación del archivo
        file.open(index_path, std::ios::out | std::ios::binary);
        file.close();

        file.open(index_path, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    }
}

void PlayerIndex::load_index_map() {
    uint16_t username_size;
    while (file.read(reinterpret_cast<char*>(&username_size), sizeof(username_size))) {
        username_size = ntohs(username_size);

        std::string username(username_size, 0);
        file.read(&username[0], username_size);

        uint32_t offset;
        file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
        offset = ntohl(offset);

        index[username] = offset;
    }
}

void PlayerIndex::add(const std::string& username, uint32_t offset) {
    uint16_t net_username_size = htons(static_cast<uint16_t>(username.size()));
    uint32_t net_offset = htonl(offset);

    append_player(username, net_username_size, net_offset, offset);
}

void PlayerIndex::append_player(const std::string& username, uint16_t net_username_size, uint32_t net_offset,
                                uint32_t offset) {
    std::lock_guard<std::mutex> lock(mutex);

    file.write(reinterpret_cast<const char*>(&net_username_size), sizeof(net_username_size));
    file.write(reinterpret_cast<const char*>(username.data()), username.size());
    file.write(reinterpret_cast<const char*>(&net_offset), sizeof(net_offset));

    file.flush();

    index[username] = offset;
}

uint32_t PlayerIndex::get(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex);

    if (index.find(username) == index.end())
        throw PlayerNotFound();

    return index.at(username);
}

bool PlayerIndex::exists(const std::string& username) {
    std::lock_guard<std::mutex> lock(mutex);

    return index.find(username) != index.end();
}

PlayerIndex::~PlayerIndex() { file.close(); }

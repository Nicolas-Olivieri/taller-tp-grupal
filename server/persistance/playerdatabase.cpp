#include "playerdatabase.h"

#include <arpa/inet.h>

PlayerDataBase::PlayerDataBase(const std::string& database_path):
        file(database_path, std::ios::in | std::ios::out | std::ios::binary) {

    open_file_correctly(database_path);

    file.clear();
}

// TODO: lógica muy parecida en playerindex. revisar
void PlayerDataBase::open_file_correctly(const std::string& database_path) {
    if (!file.is_open()) {
        file.clear();

        // fuerza la creación del archivo
        file.open(database_path, std::ios::out | std::ios::binary);
        file.close();

        file.open(database_path, std::ios::in | std::ios::out | std::ios::binary);
    }
}

uint32_t PlayerDataBase::add(PlayerData data) {
    set_data_for_network(data);

    std::lock_guard<std::mutex> lock(mutex);

    file.seekp(0, std::ios::end);
    uint32_t offset = static_cast<uint32_t>(file.tellp());
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));

    file.flush();

    return offset;
}

void PlayerDataBase::set_data_for_network(PlayerData& data) {
    data.current_xp_amount = htonl(data.current_xp_amount);
    data.current_hp = htons(data.current_hp);
    data.current_mana = htons(data.current_mana);
    data.current_gold = htons(data.current_gold);
    data.bank_gold = htons(data.bank_gold);
    data.position_x = htons(data.position_x);
    data.position_y = htons(data.position_y);
}

void PlayerDataBase::update(PlayerData data, uint32_t offset) {
    set_data_for_network(data);

    std::lock_guard<std::mutex> lock(mutex);

    file.seekp(offset, std::ios::beg);
    file.write(reinterpret_cast<const char*>(&data), offsetof(PlayerData, archetype));

    file.flush();
}

PlayerData PlayerDataBase::get(uint32_t offset) {
    PlayerData data;

    read_player_data(data, offset);
    set_data_for_host(data);

    return data;
}

void PlayerDataBase::read_player_data(PlayerData& data, uint32_t offset) {
    std::lock_guard<std::mutex> lock(mutex);

    file.seekg(offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
}

void PlayerDataBase::set_data_for_host(PlayerData& data) {
    data.current_xp_amount = ntohl(data.current_xp_amount);
    data.current_hp = ntohs(data.current_hp);
    data.current_mana = ntohs(data.current_mana);
    data.current_gold = ntohs(data.current_gold);
    data.bank_gold = ntohs(data.bank_gold);
    data.position_x = ntohs(data.position_x);
    data.position_y = ntohs(data.position_y);
}

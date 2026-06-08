#ifndef CLAN_H
#define CLAN_H

#include <cstdint>
#include <deque>
#include <string>
#include <unordered_set>
#include <vector>

class Clan {
    std::string clan_name;
    std::string founder;
    std::unordered_set<std::string> members;

    std::deque<std::string> joining_requests;
    std::unordered_set<std::string> banned_players;

    static const uint8_t MAX_MEMBERS = 16;

public:
    // TODO definir parámetros
    Clan(/* clan_data */);

    /// Principales operaciones

    Clan(const std::string& founder_name);


private:

    void join(const std::string& player_name);

    void accept(const std::string& player_name, const std::string& player_to_accept);

    void reject(const std::string& player_name, const std::string& player_to_reject);

    void kick(const std::string& player_name, const std::string& player_to_kick);

    void ban(const std::string& player_name, const std::string& player_to_ban);

    void leave(const std::string& player_name);

    std::vector<std::string> list(const std::string& player_name);

    /// Auxiliares
    bool is_founder(const std::string& player_name);
};


#endif  // CLAN_H

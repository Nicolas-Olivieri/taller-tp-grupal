#ifndef SERVER_MAP_LOADER_H
#define SERVER_MAP_LOADER_H
#include <format>
#include <stdexcept>
#include <string>

#include "common/dto/snapshot/map/client_map_data.h"
#include "common/dto/snapshot/map/server_map_data.h"

struct MapFileNotFound: public std::invalid_argument {
    explicit MapFileNotFound(std::string path) :
        std::invalid_argument(std::format("No map.bin file found at folder {}", path)) {}
};

class ServerMapLoader {
private:
    std::string map_path;

    template<class intType>
    intType parse_int(std::ifstream &file);

public:
    ServerMapLoader();

    ServerMapDataDTO get_server_data();

    ClientMapDataDTO get_client_data();

    std::vector<AssetInfoDTO> get_assets(std::ifstream &map);
};


#endif //SERVER_MAP_LOADER_H

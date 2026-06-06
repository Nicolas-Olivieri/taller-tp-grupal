#ifndef SERVER_MAP_LOADER_H
#define SERVER_MAP_LOADER_H
#include <format>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/dto/snapshot/map/client_map_data.h"
#include "common/dto/snapshot/map/server_map_data.h"

struct MapFileNotFound: public std::invalid_argument {
    explicit MapFileNotFound(std::string path):
            std::invalid_argument(
                    std::format("No se encontró ningun archivo map.bin en la dirección {}", path)) {}
};

struct InvalidFile: public std::invalid_argument {
    explicit InvalidFile(std::string path):
            std::invalid_argument(
                    std::format("El archivo map.bin encontrado en {} no tiene el formato correcto", path)) {}
};

class ServerMapLoader {
private:
    std::string map_path;
    std::ifstream map;

    template <class intType>
    intType parse_int();

    std::vector<AssetInfoDTO> get_assets();

public:
    ServerMapLoader();

    ServerMapDataDTO get_server_data();

    ClientMapDataDTO get_client_data();
};


#endif  // SERVER_MAP_LOADER_H

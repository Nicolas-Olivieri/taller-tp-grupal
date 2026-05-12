#include "server.h"
#include <iostream>
#include <map>
#include <toml.hpp>
#include "common/toml_test.cpp"


Server::Server(Socket&& skt) : protocol(skt.accept()) {}

void Server::run() {
    DataDTO response = protocol.recv_msg();
    std::cout << response.msg << std::endl;

    DataDTO msg = {Command::HANDSHAKE, "Hola mundo! Atte. Server"};
    protocol.send_msg(msg);


    // Si se quiere probar TOML (ver desde debug)
    // poc_toml();
}


void Server::poc_toml() {
    try {
        auto root = toml::parse("items.toml");

        auto equipables = toml::find<std::map<std::string, Equipable>>(root, "equipable");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

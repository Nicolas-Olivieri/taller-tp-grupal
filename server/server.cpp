#include "server.h"

#include <iostream>
#include <memory>

#include <toml.hpp>

#include "command/command.h"
#include "command/move_command.h"
#include "common/queue.h"
#include "common/toml_test.h"
#include "game/game_world.h"
#include "game/position.h"


Server::Server() {}

void Server::run() { poc_command(); }


void Server::poc_command() {
    std::cout << "\n--- Iniciando la PoC del Patrón Command ---\n\n";

    GameWorld world;
    Queue<std::shared_ptr<Command>> command_queue;

    world.add_player("Mage", Position(0, 0));

    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::UP));
    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::UP));
    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::RIGHT));

    std::shared_ptr<Command> cmd;
    while (command_queue.try_pop(cmd)) {
        cmd->execute(world);
    }

    std::cout << "\n--- Fin de la PoC ---\n";
}

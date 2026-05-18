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

    GameWorld world(10, 10);  // inicializar el mundo como una grilla de 10x10
    Queue<std::shared_ptr<Command>> command_queue;

    world.add_player("Mage", Position(0, 0));
    world.add_player("Rogue", Position(0, 1));

    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::DOWN));
    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::RIGHT));
    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::DOWN));
    command_queue.push(std::make_shared<MoveCommand>("Mage", Direction::DOWN));

    command_queue.push(
            std::make_shared<MoveCommand>("Rogue", Direction::RIGHT));
    command_queue.push(std::make_shared<MoveCommand>("Rogue", Direction::UP));
    command_queue.push(
            std::make_shared<MoveCommand>("Rogue", Direction::RIGHT));

    std::shared_ptr<Command> cmd;
    while (command_queue.try_pop(cmd)) {
        cmd->execute(world);
    }

    world.add_player("Cleric");

    std::cout << "\n--- Fin de la PoC ---\n";
}

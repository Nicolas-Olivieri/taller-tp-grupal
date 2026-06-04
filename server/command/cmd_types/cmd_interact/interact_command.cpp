#include "interact_command.h"

#include <cassert>
#include <format>
#include <map>
#include <vector>

InteractCommand::InteractCommand(const std::string& player_name, const int x, const int y):
        player_name(player_name), position(x, y), result() {}

void InteractCommand::execute(GameWorld& world) {
    result = world.interact(player_name, position);
    // TODO aca podriamos conseguir el clan name y eso

    if (result.type == InteractionType::ATTACK) {
        for (const auto& [name, player]: world.get_players()) {
            if (name == player_name)
                continue;
            if (player.get_position() == position) {
                result.attack.player_attacked = name;
            }
        }
    }
}


void InteractCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.type) {
        case InteractionType::ATTACK:
            handle_attack(builder);
            break;
        case InteractionType::BIND:
            handle_bind(builder);
            break;
        case InteractionType::MUST_NOT_NOTIFY:
            break;
        default:
            throw std::runtime_error("InteractCommand encontró un tipo de interacción inválido");
    }
}

void InteractCommand::handle_attack(SnapshotBuilder& builder) {
    assert(result.type == InteractionType::ATTACK);

    AttackStatus status = result.attack.status;

    static std::map<AttackStatus, std::string> status_to_message(
            {{AttackStatus::OUT_OF_RANGE, "El objetivo esta fuera de alcance"},
             {AttackStatus::DEAD_TARGET, "El objetivo ya esta muerto"},
             {AttackStatus::CANNOT_ATTACK, "No es posible atacar en este momento"}});

    if (status_to_message.contains(status)) {
        const std::string message = status_to_message.at(status);
        builder.add_action(ActionDTO(ChatListDTO(message, player_name)));
        return;
    }

    switch (status) {
        case AttackStatus::HIT:
            handle_hit(builder);
            break;
        case AttackStatus::TARGET_DODGED:
            handle_dodge(builder);
            break;
        default:
            throw std::runtime_error("InteractCommand recibió un tipo de ataque inválido");
    }
}

void InteractCommand::handle_hit(SnapshotBuilder& builder) {
    std::vector<std::string> lines;
    const std::string& player_attacked = result.attack.player_attacked;

    if (!player_attacked.empty()) {
        std::vector<std::string> lines_to_attacked;

        lines.push_back(
                std::format("Le quitaste {} de vida a {}", result.attack.damage_dealt, player_attacked));
        lines_to_attacked.push_back(
                std::format("{} te quito {} de vida", player_name, result.attack.damage_dealt));

        if (result.attack.was_killed) {
            lines.push_back(std::format("Mataste a {}", player_attacked));
            lines_to_attacked.push_back(std::format("{} te mato", player_name, result.attack.damage_dealt));

            builder.add_action(ActionDTO(DeathDTO(player_attacked)));
        }

        builder.add_action(ActionDTO(ChatListDTO(lines_to_attacked, player_attacked)));
    } else {

        lines.push_back(std::format("Le quitaste {} vida a la entidad", result.attack.damage_dealt));

        if (result.attack.was_killed) {
            lines.push_back("Mataste a la entidad");
        }
    }

    builder.add_action(ActionDTO(ChatListDTO(lines, player_name)));
}

void InteractCommand::handle_dodge(SnapshotBuilder& builder) {
    std::vector<std::string> lines;
    const std::string& player_attacked = result.attack.player_attacked;

    if (!player_attacked.empty()) {

        lines.push_back(std::format("{} esquivo tu ataque", player_attacked));

        builder.add_action(ActionDTO(
                ChatListDTO(std::format("Esquivaste el ataque de {}!!", player_name), player_attacked)));
    } else {
        lines.push_back("La entidad te Esquivo");
    }

    builder.add_action(ActionDTO(ChatListDTO(lines, player_name)));
}

void InteractCommand::handle_bind(SnapshotBuilder& builder) {
    std::string msg;

    switch (result.bind) {
        case BindResult::PRIEST:
            msg += "Estas hablando con el Sacerdote";
            break;
        case BindResult::MERCHANT:
            throw std::runtime_error("Interact command recibió una interacción de merchant y no se "
                                     "implementó su handle en handle_bind()");
            break;
        case BindResult::BANKER:
            throw std::runtime_error("Interact command recibió una interacción de banker y no se implementó "
                                     "su handle en handle_bind()");
            break;
        default:
            throw std::runtime_error(
                    "Interact command recibió una interacción de un tipo de aliado desconocido");
    }

    builder.add_action(ActionDTO(ChatListDTO(msg, player_name)));
}

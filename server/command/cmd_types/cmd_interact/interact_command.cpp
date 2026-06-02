#include "interact_command.h"

#include <cassert>
#include <format>
#include <map>

InteractCommand::InteractCommand(const std::string& player_name, const int x, const int y):
        player_name(player_name), position(x, y), result(0) {}


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
    //    && result.attack.status == AttackStatus::HIT
}


void InteractCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.type) {
        case InteractionType::ATTACK:
            handle_attack(builder);
            break;
        case InteractionType::BIND:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Aliado", player_name,
                                                        "Te escucho. ¿En que te ayudo?")));
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
        builder.add_action(
                ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Mundo", player_name, message)));
        return;
    }

    switch (status) {
        case AttackStatus::HIT:
            handle_hit(builder);
            break;
        case AttackStatus::TARGET_DODGED:
            break;
        default:
            throw std::runtime_error("InteractCommand recibió un tipo de ataque inválido");
    }

    //    AttackStatus::TARGET_DODGED, "El objetivo esquivo el ataque"
    assert(status == AttackStatus::HIT);

    // TODO

    return;
}

void InteractCommand::handle_hit(SnapshotBuilder& builder) {
    const std::string& player_attacked = result.attack.player_attacked;
    if (!player_attacked.empty()) {
        builder.add_action(ActionDTO(ChatMessageDTO(
                MessageVisibility::PRIVATE, "Mundo", player_name,
                std::format("Le hiciste {} de daño a {}", result.attack.damage_dealt, player_attacked))));
        builder.add_action(ActionDTO(ChatMessageDTO(
                MessageVisibility::PRIVATE, "Mundo", player_attacked,
                std::format("{} te hizo {} de daño", player_name, result.attack.damage_dealt))));

        if (result.attack.was_killed) {
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Mundo", player_name,
                                                        std::format("Mataste a {}", player_attacked))));
            builder.add_action(ActionDTO(
                    ChatMessageDTO(MessageVisibility::PRIVATE, "Mundo", player_attacked,
                                   std::format("{} te mato", player_name, result.attack.damage_dealt))));
        }

        return;
    }
}

void InteractCommand::handle_dodge(SnapshotBuilder& builder) {
    const std::string& player_attacked = result.attack.player_attacked;
    builder.add_action(
            ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, player_name, player_attacked, "TE PEGO")));

    if (!player_attacked.empty())
        builder.add_action(ActionDTO(
                ChatMessageDTO(MessageVisibility::PRIVATE, player_attacked, player_name, "ME PEGASTE")));
}

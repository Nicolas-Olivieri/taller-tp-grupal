#include "ally_action_payload.h"


AllyActionPayload::AllyActionPayload(const AllyAction& action): action(action), item_id(0), gold_amount(0) {}


AllyActionPayload::AllyActionPayload(const AllyAction& action, const uint8_t item_id):
        action(action), item_id(item_id), gold_amount(0) {}


AllyActionPayload::AllyActionPayload(const AllyAction& action, const uint16_t gold_amount):
        action(action), item_id(0), gold_amount(gold_amount) {}

#include "common/protocol/protocol.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "common/dto/events/ally_related/deposit/deposit_gold_event.h"
#include "common/dto/events/ally_related/shop/buy_event.h"
#include "common/dto/events/chat/chatevent.h"
#include "common/dto/events/clan/clan_found_event.h"
#include "common/dto/events/clan/clan_request_response_event.h"
#include "common/dto/events/movement/moveevent.h"
#include "common/socket.h"

#include "test_helper.h"

class ProtocolTest: public ::testing::Test {
protected:
    std::unique_ptr<Socket> server_skt;
    std::unique_ptr<Socket> client_skt;
    std::unique_ptr<Protocol> client_protocol;
    std::unique_ptr<Protocol> server_protocol;
    TestHelper helper;

    void SetUp() override {
        int sv[2];

        if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
            FAIL() << "Fallo al crear socketpair en el sistema operativo";
        }

        server_skt = std::make_unique<Socket>(Socket(sv[0]));
        client_skt = std::make_unique<Socket>(Socket(sv[1]));

        client_protocol = std::make_unique<Protocol>(*client_skt);
        server_protocol = std::make_unique<Protocol>(*server_skt);
    }

    void TearDown() override {
        server_protocol.reset();
        client_protocol.reset();
        client_skt.reset();
        server_skt.reset();
    }
};


/*
 * Pruebas unitarias para ClientMapDataDTO
 */

TEST_F(ProtocolTest, SendAndReceiveMap) {
    constexpr int assets_size = 5;
    constexpr int width = 10;
    constexpr int height = 10;
    const auto tiles = helper.mock_asset_vector(assets_size);
    const auto safe_zones = helper.mock_asset_vector(assets_size);
    const auto colliders = helper.mock_asset_vector(assets_size);
    const auto npcs = helper.mock_asset_vector(assets_size);
    const ClientMapDataDTO to_send(width, height, tiles, safe_zones, colliders, npcs);

    client_protocol->send(to_send);

    const ClientMapDataDTO received = server_protocol->recv_map();

    EXPECT_EQ(received.world_width, to_send.world_width);
    EXPECT_EQ(received.world_height, to_send.world_height);
    EXPECT_TRUE(helper.equals(received.tiles, to_send.tiles));
    EXPECT_TRUE(helper.equals(received.safe_zones, to_send.safe_zones));
    EXPECT_TRUE(helper.equals(received.colliders, to_send.colliders));
    EXPECT_TRUE(helper.equals(received.npcs, to_send.npcs));
}


/*
 * Pruebas unitarias para CredentialsDTO
 */

TEST_F(ProtocolTest, Credentials_SendAndReceiveSuccessfully) {
    const CredentialsDTO to_send("test_user");

    client_protocol->send(to_send);
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_EQ(received.username, to_send.username);
}

TEST_F(ProtocolTest, Credentials_HandlesEmptyUsername) {
    const CredentialsDTO to_send("");

    client_protocol->send(to_send);
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_EQ(received.username, to_send.username);
    EXPECT_EQ(received.message_size(), sizeof(uint16_t));
}

TEST_F(ProtocolTest, Credentials_ThrowsOnWrongHeaderByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::EXISTENCE));

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_credentials(), std::runtime_error);
}

TEST_F(ProtocolTest, Credentials_DecodesRawBinaryFrameCorrectly) {
    const std::string expected = "test_user";
    const uint16_t net_value = htons(expected.size());

    const size_t total_size = sizeof(uint8_t) + sizeof(uint16_t) + expected.size();
    std::vector<uint8_t> buffer(total_size);

    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::CREDENTIALS);

    std::memcpy(&buffer[offset], &net_value, sizeof(net_value));
    offset += sizeof(net_value);

    std::memcpy(&buffer[offset], expected.data(), expected.size());

    client_skt->sendall(buffer.data(), buffer.size());
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_EQ(received.username, expected);
}


/*
 * Pruebas unitarias para RequestedCommandDTO
 */

TEST_F(ProtocolTest, Command_ReceiveMoveCorrectly) {
    const MoveEventDTO to_send(Direction::DOWN);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.direction, to_send.direction);
}

TEST_F(ProtocolTest, Command_ReceiveChatCorrectly) {
    const ChatEventDTO to_send("test_user", "hello");

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.receiver, to_send.receiver);
    EXPECT_EQ(received.message, to_send.content);
}

TEST_F(ProtocolTest, Command_ReceiveParameterlessCommand) {
    const EventDTO to_send(CommandType::HEAL);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
}

TEST_F(ProtocolTest, Command_ReceiveOneByteNumberCommand) {
    const BuyEventDTO to_send(1);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.one_byte_number, to_send.item_id);
}

TEST_F(ProtocolTest, Command_ReceiveTwoByteNumberCommand) {
    const DepositGoldEventDTO to_send(1000);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.gold_amount, to_send.gold_amount);
}

TEST_F(ProtocolTest, Command_ReceiveClanNameCommand) {
    const ClanFoundEventDTO to_send("test_clan");

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.clan_name, to_send.clan_name);
}

TEST_F(ProtocolTest, Command_ReceivePlayerNameAndBoolCommand) {
    const RequestResponseEventDTO to_send("test_clan", true);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, to_send.command);
    EXPECT_EQ(received.player_name, to_send.player_name);
    EXPECT_EQ(received.command_selector, to_send.is_accepted);
}

TEST_F(ProtocolTest, Command_ThrowsOnUnknownCommandByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::COMMAND));
    buffer.push_back(255);

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_command(), std::invalid_argument);
}

TEST_F(ProtocolTest, Command_DecodesInteractRawBinaryCorrectly) {
    const uint16_t target_x = htons(150);
    const uint16_t target_y = htons(200);

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(CommandType) + sizeof(target_x) + sizeof(target_y));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::COMMAND);
    buffer[offset++] = static_cast<uint8_t>(CommandType::INTERACT);

    std::memcpy(&buffer[offset], &target_x, sizeof(target_x));
    offset += sizeof(target_x);

    std::memcpy(&buffer[offset], &target_y, sizeof(target_y));

    client_skt->sendall(buffer.data(), buffer.size());
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_EQ(received.command, CommandType::INTERACT);
    EXPECT_EQ(received.x, ntohs(target_x));
    EXPECT_EQ(received.y, ntohs(target_y));
}

/*
 * Pruebas unitarias para ExistenceDTO
 */

TEST_F(ProtocolTest, Existence_SendAndReceiveSuccessfully) {
    const ExistenceDTO to_send(1, 0);

    client_protocol->send(to_send);
    const ExistenceDTO received = server_protocol->recv_existence();

    EXPECT_EQ(received.user_exists, to_send.user_exists);
    EXPECT_EQ(received.user_connected, to_send.user_connected);
}

TEST_F(ProtocolTest, Existence_DecodesRawBinaryCorrectly) {
    constexpr uint8_t user_exists = 1;
    constexpr uint8_t user_connected = 1;

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(user_exists) + sizeof(user_connected));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::EXISTENCE);
    buffer[offset++] = user_exists;
    buffer[offset] = user_connected;

    client_skt->sendall(buffer.data(), buffer.size());
    const ExistenceDTO received = server_protocol->recv_existence();

    EXPECT_EQ(received.user_exists, user_exists);
    EXPECT_EQ(received.user_connected, user_connected);
}

TEST_F(ProtocolTest, Existence_ThrowsOnTruncatedFrameAndDisconnect) {
    constexpr uint8_t user_exists = 1;

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(user_exists));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::EXISTENCE);
    buffer[offset] = user_exists;

    client_skt->sendall(buffer.data(), buffer.size());
    client_skt->close();  // simula una desconexión abrupta del cliente

    EXPECT_THROW(server_protocol->recv_existence(), ClosedSocket);
}
/*
 * Prueba unitarias para RequestedCommandDTO
 */

TEST_F(ProtocolTest, CreatePlayer_ReceiveValidOptionsCorrectly) {
    const CreatePlayerDTO to_send = helper.mock_create_player();
    client_protocol->send(to_send);

    const CreatePlayerDTO received = server_protocol->recv_appearance();
    const AppearanceDTO received_appearance = received.appearance;

    EXPECT_EQ(to_send.appearance.body, received.appearance.body);
    EXPECT_EQ(to_send.appearance.head, received.appearance.head);
    EXPECT_EQ(to_send.archetype, received.archetype);
    EXPECT_EQ(to_send.race, received.race);
}

TEST_F(ProtocolTest, CreatePlayer_ThrowsOnWrongHeaderByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::CREDENTIALS));
    buffer.push_back(sizeof(CreatePlayerDTO) - sizeof(Message));

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_appearance(), std::runtime_error);
}

TEST_F(ProtocolTest, CreatePlayer_DecodesCreatePlayerRawBinaryCorrectly) {
    const CreatePlayerDTO target = helper.mock_create_player();

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(target.appearance.body) +
                                sizeof(target.appearance.head) + sizeof(target.archetype) +
                                sizeof(target.race));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::CREATE_PLAYER);

    buffer[offset++] = target.appearance.body;
    buffer[offset++] = target.appearance.head;
    buffer[offset++] = target.archetype;
    buffer[offset++] = target.race;

    client_skt->sendall(buffer.data(), buffer.size());

    const CreatePlayerDTO received = server_protocol->recv_appearance();

    EXPECT_EQ(received.appearance.body, target.appearance.body);
    EXPECT_EQ(received.appearance.head, target.appearance.head);
    EXPECT_EQ(received.archetype, target.archetype);
    EXPECT_EQ(received.race, target.race);
}

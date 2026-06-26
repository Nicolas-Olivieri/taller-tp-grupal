#include "common/protocol/protocol.h"

#include <cstring>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

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
    const CredentialsDTO to_send = helper.mock_credentials("dummy");

    client_protocol->send(to_send);
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Credentials_HandlesEmptyUsername) {
    const CredentialsDTO to_send = helper.mock_credentials("");

    client_protocol->send(to_send);
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_EQ(received.message_size(), sizeof(uint16_t));
    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Credentials_ThrowsOnWrongHeaderByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::EXISTENCE));

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_credentials(), std::runtime_error);
}

TEST_F(ProtocolTest, Credentials_DecodesRawBinaryFrameCorrectly) {
    const CredentialsDTO to_send = helper.mock_credentials("dummy");

    const uint16_t net_value = htons(to_send.username.size());

    const size_t total_size = sizeof(uint8_t) + sizeof(uint16_t) + to_send.username.size();
    std::vector<uint8_t> buffer(total_size);
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::CREDENTIALS);

    std::memcpy(&buffer[offset], &net_value, sizeof(net_value));
    offset += sizeof(net_value);

    std::memcpy(&buffer[offset], to_send.username.data(), to_send.username.size());

    client_skt->sendall(buffer.data(), buffer.size());
    const CredentialsDTO received = server_protocol->recv_credentials();

    EXPECT_TRUE(helper.equals(received, to_send));
}


/*
 * Pruebas unitarias para RequestedCommandDTO
 */

TEST_F(ProtocolTest, Command_ReceiveMoveCorrectly) {
    const MoveEventDTO to_send = helper.mock_move_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceiveChatCorrectly) {
    const ChatEventDTO to_send = helper.mock_chat_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceiveParameterlessCommand) {
    const EventDTO to_send = helper.mock_event(CommandType::HEAL);

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceiveOneByteNumberCommand) {
    const BuyEventDTO to_send = helper.mock_buy_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceiveTwoByteNumberCommand) {
    const DepositGoldEventDTO to_send = helper.mock_deposit_gold_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceiveClanNameCommand) {
    const ClanFoundEventDTO to_send = helper.mock_clan_found_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ReceivePlayerNameAndBoolCommand) {
    const RequestResponseEventDTO to_send = helper.mock_request_response_event();

    client_protocol->send(to_send);
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Command_ThrowsOnUnknownCommandByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::COMMAND));
    buffer.push_back(255);

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_command(), std::invalid_argument);
}

TEST_F(ProtocolTest, Command_DecodesInteractRawBinaryCorrectly) {
    const InteractEventDTO to_send = helper.mock_interact_event();
    const uint16_t target_x = htons(to_send.target_x);
    const uint16_t target_y = htons(to_send.target_y);

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(CommandType) + sizeof(target_x) + sizeof(target_y));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::COMMAND);
    buffer[offset++] = static_cast<uint8_t>(CommandType::INTERACT);

    std::memcpy(&buffer[offset], &target_x, sizeof(target_x));
    offset += sizeof(target_x);

    std::memcpy(&buffer[offset], &target_y, sizeof(target_y));

    client_skt->sendall(buffer.data(), buffer.size());
    const RequestedCommandDTO received = server_protocol->recv_command();

    EXPECT_TRUE(helper.equals(received, to_send));
}

/*
 * Pruebas unitarias para ExistenceDTO
 */

TEST_F(ProtocolTest, Existence_SendAndReceiveSuccessfully) {
    const ExistenceDTO to_send = helper.mock_existence();

    client_protocol->send(to_send);
    const ExistenceDTO received = server_protocol->recv_existence();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, Existence_DecodesRawBinaryCorrectly) {
    const ExistenceDTO to_send = helper.mock_existence();

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(to_send.user_exists) +
                                sizeof(to_send.user_connected));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::EXISTENCE);
    buffer[offset++] = to_send.user_exists;
    buffer[offset] = to_send.user_connected;

    client_skt->sendall(buffer.data(), buffer.size());
    const ExistenceDTO received = server_protocol->recv_existence();

    EXPECT_TRUE(helper.equals(received, to_send));
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
 * Pruebas unitarias para CreatePlayerDTO
 */

TEST_F(ProtocolTest, CreatePlayer_ReceiveValidOptionsCorrectly) {
    const CreatePlayerDTO to_send = helper.mock_create_player();

    client_protocol->send(to_send);
    const CreatePlayerDTO received = server_protocol->recv_appearance();

    EXPECT_TRUE(helper.equals(received, to_send));
}

TEST_F(ProtocolTest, CreatePlayer_ThrowsOnWrongHeaderByte) {
    std::vector<uint8_t> buffer;
    buffer.push_back(static_cast<uint8_t>(Message::CREDENTIALS));
    buffer.push_back(sizeof(CreatePlayerDTO) - sizeof(Message));

    client_skt->sendall(buffer.data(), buffer.size());

    EXPECT_THROW(server_protocol->recv_appearance(), std::runtime_error);
}

TEST_F(ProtocolTest, CreatePlayer_DecodesCreatePlayerRawBinaryCorrectly) {
    const CreatePlayerDTO to_send = helper.mock_create_player();

    std::vector<uint8_t> buffer(sizeof(Message) + sizeof(to_send.appearance.body) +
                                sizeof(to_send.appearance.head) + sizeof(to_send.archetype) +
                                sizeof(to_send.race));
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(Message::CREATE_PLAYER);

    buffer[offset++] = to_send.appearance.body;
    buffer[offset++] = to_send.appearance.head;
    buffer[offset++] = to_send.archetype;
    buffer[offset] = to_send.race;

    client_skt->sendall(buffer.data(), buffer.size());
    const CreatePlayerDTO received = server_protocol->recv_appearance();

    EXPECT_TRUE(helper.equals(received, to_send));
}

/*
 * Pruebas unitarias para SnapshotDTO
 */
TEST_F(ProtocolTest, Snapshot_SendAndReceiveSuccessfully) {
    const auto players_information = helper.mock_players_information(10);
    const auto creatures_information = helper.mock_creatures_information(5);
    const auto loot_information = helper.mock_loots_information(7);
    const auto actions = helper.mock_actions();

    const SnapshotDTO to_send(players_information, creatures_information, loot_information, actions);

    client_protocol->send(to_send);

    const SnapshotDTO received = server_protocol->recv_snapshot();

    EXPECT_TRUE(helper.equals(received.players_information, to_send.players_information));
    EXPECT_TRUE(helper.equals(received.creatures_information, to_send.creatures_information));
    EXPECT_TRUE(helper.equals(received.loot_information, to_send.loot_information));
    EXPECT_TRUE(helper.equals(received.actions, to_send.actions));
}

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

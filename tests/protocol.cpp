#include "common/protocol/protocol.h"

#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "test_helper.h"
#include "common/socket.h"

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



#include "common/protocol/protocol.h"

#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <sys/socket.h>

#include "common/socket.h"

class ProtocolTest: public ::testing::Test {
protected:
    std::unique_ptr<Socket> server_skt;
    std::unique_ptr<Socket> client_skt;
    std::unique_ptr<Protocol> client_protocol;
    std::unique_ptr<Protocol> server_protocol;

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

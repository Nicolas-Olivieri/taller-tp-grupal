#include "client_protocol.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

ClientProtocol::ClientProtocol(Socket&& skt): skt(std::move(skt)) {}

void ClientProtocol::send_msg(const DataDTO& data) {
    const uint8_t cmd = data.command;
    const uint8_t msg_size = data.msg.length() * sizeof(char);

    const size_t buf_size = sizeof(cmd) + sizeof(msg_size) + msg_size;
    std::vector<uint8_t> buffer(buf_size);

    buffer[0] = cmd;
    buffer[1] = msg_size;
    std::copy(data.msg.begin(), data.msg.end(), buffer.begin() + 2);

    skt.sendall(buffer.data(), buf_size);
}

DataDTO ClientProtocol::recv_msg() {
    uint8_t cmd;
    uint8_t msg_size;
    skt.recvall(&cmd, sizeof(cmd));
    skt.recvall(&msg_size, sizeof(msg_size));

    std::string msg(msg_size, 0);
    skt.recvall(&msg[0], msg_size);

    DataDTO data = {static_cast<CommandCode>(cmd), msg};
    return data;
}

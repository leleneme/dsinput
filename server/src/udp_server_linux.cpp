#include "udp_server.hpp"
#include <cassert>
#include <cstdint>

udp_server_error udp_server_init(std::string address, int port, udp_server *server) {
    assert(server != nullptr);
    assert(port <= UINT16_MAX && port > 0);

    server->address = address;
    server->addr_info = nullptr;

    struct addrinfo hints = { };
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    char port_as_string[6]; // UINT16_MAX is 5 characters long (+1 for null terminator)
    snprintf(port_as_string, sizeof(port_as_string), "%d", port);

    int r = getaddrinfo(address.c_str(), port_as_string, &hints, &server->addr_info);
    if (r != 0 || server->addr_info == nullptr) {
        return udp_server_error::getaddrinfo;
    }

    server->sock = socket(server->addr_info->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if (server->sock < 0) {
        return udp_server_error::socket_open;
    }

    r = bind(server->sock, server->addr_info->ai_addr, server->addr_info->ai_addrlen);
    if (r != 0) {
        return udp_server_error::socket_bind;
    }

    return udp_server_error::none;
}

void udp_server_destroy(udp_server *server) {
    freeaddrinfo(server->addr_info);
    close(server->sock);
}
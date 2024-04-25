#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <cstdlib>
#include <string>
#include <cassert>

enum class udp_server_error {
    none,
    getaddrinfo,
    socket_open,
    socket_bind,
};

struct udp_server {
    static constexpr size_t max_msg = sizeof(uint16_t);
    int sock;
    int port;
    std::string address;
    struct addrinfo *addr_info;

    template<typename T>
    bool receive_data(T *data) {
        assert(data != nullptr);
        return recv(sock, data, sizeof(T), 0) != -1;
    }
};

udp_server_error udp_server_init(std::string address, int port, udp_server *server);
void udp_server_destroy(udp_server *server);
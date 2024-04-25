#include "vcontroller.hpp"
#include "udp_server.hpp"

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cerrno>

static void show_vcontroller_error(vcontroller_error error);
static void show_udp_server_error(udp_server_error error);

const char* ip = "0.0.0.0";
const int port = 1032;

int main() {
    vcontroller vctrl {};
    auto verror = vcontroller_init(&vctrl);
    if (verror != vcontroller_error::none) {
        show_vcontroller_error(verror);
        vcontroller_destroy(&vctrl);
        return 1;
    }

    printf("[Info] Created virtual controller as '%s' (%X) (%X)\n", vctrl.usetup.name, vctrl.usetup.id.vendor, vctrl.usetup.id.product);
    printf("[Info] Starting UDP listener at %s:%d\n", ip, port);

    udp_server server = {};
    auto serror = udp_server_init(ip, port, &server);
    if (serror != udp_server_error::none) {
        show_udp_server_error(serror);
        vcontroller_destroy(&vctrl);
        return 1;
    }

    uint32_t error_count = 0;
    while (1) {
        uint16_t data = 0x0;
        server.receive_data<uint16_t>(&data);

        // Assume data is send in big-endianness
        uint16_t input = (data >> 8) | (data << 8);
        verror = vcontroller_update(&vctrl, input);

        if (verror != vcontroller_error::none) [[unlikely]] {
            show_vcontroller_error(verror);
            error_count++;
        } else {
            error_count = 0;
        }

        if (error_count >= 50) [[unlikely]] {
            fprintf(stderr, "To many consecutive errors ocurred! Exiting!\n");
            break;
        }
    }

    vcontroller_destroy(&vctrl);
    udp_server_destroy(&server);

    return 0;
}

static const char* get_vcontroller_error_message(vcontroller_error error) {
    switch (error) {
    case vcontroller_error::uinput_dev_create:
        return "Failed to create virtual controller";
    case vcontroller_error::uinput_dev_setup:
        return "Failed to setup virtual controller";
    case vcontroller_error::uinput_open:
        return "Failed to open virtual controller file descriptor";
    case vcontroller_error::uinput_write:
        return "Failed to write commands to virtual controller file descriptor";
    case vcontroller_error::none:
    default:
        return "Unknow error";
    }
}

static const char* get_udp_server_error_message(udp_server_error error) {
    switch (error) {
    case udp_server_error::getaddrinfo:
        return "Failed to get address information";
    case udp_server_error::socket_open:
        return "Failed to create and open socket";
    case udp_server_error::socket_bind:
        return "Failed to bind socket";
    case udp_server_error::none:
    default:
        return "Unknow error";
    }
}

static void show_vcontroller_error(vcontroller_error error) {
    fprintf(stderr, "[Error] %s: %s\n", get_vcontroller_error_message(error), strerror(errno));
}

static void show_udp_server_error(udp_server_error error) {
    fprintf(stderr, "[Error] %s: %s\n", get_udp_server_error_message(error), strerror(errno));
}
#include "vcontroller.hpp"
#include "udp_server.hpp"

#include <cstring>
#include <arpa/inet.h>

void show_vcontroller_error(vcontroller_error error);
void show_udp_server_error(udp_server_error error);

void print_usage(const char *prog_name) {
    printf("%s [address] [port]\n", prog_name);
    exit(0);
}

int main(int argc, char** argv) {
    const char* ip = "0.0.0.0";
    int port = 1032;

    if (argc > 1) {
        if (strcmp(argv[1], "help") == 0) {
            print_usage(argv[0]);
        } else {
            ip = argv[1];
        }
    }

    if (argc > 2) {
        try {
            port = std::stoi(argv[2]);
        } catch (const std::exception& ex) {
            printf("[Warn] Bad port argument, using default instead\n");
        }
    }

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
        uint16_t input = ntohs(data);
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

void show_vcontroller_error(vcontroller_error error) {
    fprintf(stderr, "[Error] %s: %s\n", vcontroller_error_string(error), strerror(errno));
}

void show_udp_server_error(udp_server_error error) {
    fprintf(stderr, "[Error] %s: %s\n", udp_server_error_string(error), strerror(errno));
}

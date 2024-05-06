#include <nds.h>
#include <fat.h>
#include <dsiwifi9.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

#include "app.hpp"
#include "fs.hpp"

namespace dsinput {

static void u32_to_u8_arr(uint32_t ip, uint8_t dest[4]) {
    memcpy(dest, &ip, 4);
}

app_error app_init(app& app) {
    consoleDemoInit();

    DSiWifi_InitDefault(true);

    if (!fatInitDefault())
        return app_error::fat_init;

    auto conf_file_opt = file_read_to_string(APPCONFPATH);
    if (!conf_file_opt)
        return app_error::config_not_found;

    auto conf_file = conf_file_opt.value();
    auto conf_error = app_config_parse(app.config, conf_file);
    if (conf_error != app_config_error::none) {
        return static_cast<app_error>(conf_error);
    }

    return app_error::none;
}

void app::on_connect() {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock < 0) {
        iprintf("socket() failed: %s\n", strerror(errno));
        while (1) { }
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config.host_port);
    server_addr.sin_addr.s_addr = inet_addr(config.host_address.c_str());

    // connected = true;
}

void app::loop() {
    while (1) {
        swiWaitForVBlank();
        scanKeys();
        this->tick();
        consoleClear();
        u8 addr_bytes[8];
        u32_to_u8_arr(client_ip, addr_bytes);

        iprintf("\x1b[s\x1b[0;0HClient IP: %u.%u.%u.%u               ", addr_bytes[0], addr_bytes[1], addr_bytes[2], addr_bytes[3]);
        iprintf("\x1b[s\x1b[1;0HServer Ip: %s:%d\n", config.host_address.c_str(), config.host_port);
        iprintf("\x1b[s\x1b[2;0HConnected: %s\n", connected ? "yes" : "no");
    }
}

void app::tick() {
    uint32_t new_ip = DSiWifi_GetIP();
    if (new_ip != this->client_ip) {
        client_ip = new_ip;
        if (new_ip == 0 || new_ip == 0xFFFFFFFF) {
            connected = false;
        } else {
            connected = true;
        }
    }

    uint16_t keys = keysCurrent();
    if (keys != last_keys && connected) {
        send_data(keys);
    }
}

void app::send_data(uint16_t keys) {
    last_keys = keys;
    uint16_t data_be = htons(keys);
    sendto(sock, &data_be, sizeof(uint16_t), 0,
        (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
}

void app::set_wifi_connect_cb(WifiConnectHandler handler) {
    DSiWifi_SetConnectHandler(handler);
}

}

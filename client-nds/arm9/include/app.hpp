#pragma once

#include "app_config.hpp"

#include <dsiwifi9.h>

#include <sys/socket.h>
#include <cstdint>
#include <string>

namespace dsinput {

constexpr const char* APPCONFPATH = "/data/settings/dsinput.conf";

enum class app_error {
    none,
    invalid_port,
    invalid_address,
    fat_init,
    config_not_found,
};

// Ensure casting from app_config_error to app_error is possible
static_assert((int)app_error::invalid_address == (int)app_config_error::invalid_address);
static_assert((int)app_error::invalid_port == (int)app_config_error::invalid_port);
static_assert((int)app_error::none == (int)app_error::none);

struct app {
    app_config config;
    uint32_t client_ip;
    bool connected;
    int sock;
    struct sockaddr_in server_addr;

    uint16_t last_keys = 0;

    std::string log;

    void on_connect();
    void loop();
    void tick();
    void send_data(uint16_t keys);

    void set_wifi_connect_cb(WifiConnectHandler handler);
};

app_error app_init(app& app);

}

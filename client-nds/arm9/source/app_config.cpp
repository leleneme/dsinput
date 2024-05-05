#include "app_config.hpp"
#include <sstream>

namespace dsinput {

#include <sys/socket.h>

static bool is_ip_valid(const char* ip) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
    return result != 0;
}

app_config_error app_config_parse(app_config& cfg, std::string& contents) {
    std::string line;
    std::istringstream iss(contents);

    bool addr_set, port_set, bckl_set, touc_set;
    addr_set = port_set = bckl_set = touc_set = false;

    while (std::getline(iss, line)) {
        size_t line_size = line.length();
        size_t eq_pos = line.find("=");
        if (eq_pos >= line_size - 1 || eq_pos == std::string::npos) {
            continue;
        }

        auto const key = line.substr(0, eq_pos);
        auto const val = line.substr(eq_pos + 1);

        if (!addr_set && key == "host_address") {
            if (!is_ip_valid(val.c_str())) {
                return app_config_error::invalid_address;
            }

            cfg.host_address = std::move(val);
            addr_set = true;
        } else if (!port_set && key == "host_port") {
            auto val_int = atoi(val.c_str());
            if (val_int <= 0 || val_int >= INT16_MAX) {
                return app_config_error::invalid_port;
            }

            cfg.host_port = val_int;
            port_set = true;
        } else if (!bckl_set && key == "enable_backlight") {
            bool val_bool = (val == "true");
            cfg.enable_backlight = val_bool;
            bckl_set = true;
        } else if (!touc_set && key == "enable_touchscreen") {
            bool val_bool = (val == "true");
            cfg.enable_touchscreen = val_bool;
            touc_set = true;
        }
    }

    return app_config_error::none;
}

std::string app_config_write(app_config& cfg) {
    return "";
}

}

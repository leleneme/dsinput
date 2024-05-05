#pragma once

#include <string>

namespace dsinput {

enum class app_config_error {
    none,
    invalid_port,
    invalid_address,
};

struct app_config {
    std::string host_address;
    int host_port;
    bool enable_backlight;
    bool enable_touchscreen; // Unused for now... :)
};

app_config_error app_config_parse(app_config& cfg, std::string& contents);
std::string app_config_write(app_config& cfg);

}

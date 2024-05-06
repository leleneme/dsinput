#undef NDEBUG
#include <nds.h>
#include "app.hpp"

dsinput::app app;

void wifi_on_reconnect() {
    app.on_connect();
}

const char* app_error_to_string(dsinput::app_error err) {
    using namespace dsinput;
    switch (err) {
    case app_error::invalid_port:
        return "Invalid server port";
    case app_error::invalid_address:
        return "Invalid server IP address";
    case app_error::fat_init:
        return "Failed to initialize libfat";
    case app_error::config_not_found:
        return "Configuration file not found";
    case app_error::none:
    default:
        return "Unknow error";
    }
}

int main() {
    consoleDemoInit();

    auto error = dsinput::app_init(app);
    if (error != dsinput::app_error::none) {
        iprintf("A error occured: %s\n", app_error_to_string(error));
        while (1) { }
    }

    app.set_wifi_connect_cb(wifi_on_reconnect);
    app.loop();

    return 0;
}

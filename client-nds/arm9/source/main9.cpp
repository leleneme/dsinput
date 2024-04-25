#include <nds.h>
#include <dsiwifi9.h>

#include <cstdio>
#include <cstdint>
#include <cstddef>

#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef ENABLE_WIFI_LOG
    #define ENABLE_WIFI_LOG 0
#endif

struct app_state {
    bool is_connected;
    int sock;
    struct sockaddr_in server_addr;
};

app_state state = {};

bool send_data(uint16_t data) {
    uint16_t data_be = htons(data);
    ssize_t result = sendto(state.sock, &data_be, sizeof(uint16_t), 0,
        (struct sockaddr*)&state.server_addr, sizeof(struct sockaddr_in));
    return result >= 0;
}

#if ENABLE_WIFI_LOG
void appwifi_log(const char* s) {
    iprintf("%s", s);
}
#endif

void appwifi_connect() {
    state.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (state.sock < 0) {
        iprintf("socket() failed\n");
        while (1) { }
    }

    state.server_addr.sin_family = AF_INET;
    state.server_addr.sin_port = htons(1032);
    state.server_addr.sin_addr.s_addr = inet_addr("192.168.0.224");

    state.is_connected = true;
}

void appwifi_reconnect() {
    state.is_connected = true;
}

int main() {
    consoleDemoInit();

#if ENABLE_WIFI_LOG
    DSiWifi_SetLogHandler(appwifi_log);
#endif
    DSiWifi_SetConnectHandler(appwifi_connect);
    DSiWifi_SetReconnectHandler(appwifi_reconnect);
    DSiWifi_InitDefault(true);

    uint16_t last_keys = 0;
    while (1) {
        swiWaitForVBlank();
        scanKeys();

        u32 addr = DSiWifi_GetIP();
        u8 addr_bytes[4];
        memcpy(addr_bytes, &addr, 4);

        consoleClear();
        iprintf("\x1b[s\x1b[0;0HIP: %u.%u.%u.%u               ", addr_bytes[0], addr_bytes[1], addr_bytes[2], addr_bytes[3]);
        iprintf("\x1b[s\x1b[1;0HConnected: %s", (state.is_connected ? "yes" : "no"));

        uint16_t keys = keysCurrent();
        bool addr_valid = (addr != 0 && addr != 0xFFFFFFFF);

        if (keys != last_keys && addr_valid && state.is_connected) {
            send_data(keys);
            last_keys = keys;
        }
    }

    return 0;
}

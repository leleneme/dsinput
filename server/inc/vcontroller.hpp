#pragma once

#include <cstdint>
#include <linux/uinput.h>

enum class vcontroller_error {
    none,
    uinput_open,
    uinput_dev_setup,
    uinput_dev_create,
    uinput_write,
};

struct vcontroller {
    int fd;
    struct uinput_setup usetup;
};

vcontroller_error vcontroller_init(vcontroller *ctl);
vcontroller_error vcontroller_update(vcontroller *ctl, uint16_t state);
void vcontroller_destroy(vcontroller *ctl);

#include "button.hpp"
#include "vcontroller.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

vcontroller_error vcontroller_init(vcontroller* ctl) {
    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0) {
        return vcontroller_error::uinput_open;
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);

    ioctl(fd, UI_SET_KEYBIT, BTN_EAST);
    ioctl(fd, UI_SET_KEYBIT, BTN_SOUTH);
    ioctl(fd, UI_SET_KEYBIT, BTN_NORTH);
    ioctl(fd, UI_SET_KEYBIT, BTN_WEST);
    ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_UP);
    ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
    ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_DPAD_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_START);
    ioctl(fd, UI_SET_KEYBIT, BTN_SELECT);
    ioctl(fd, UI_SET_KEYBIT, BTN_TL);
    ioctl(fd, UI_SET_KEYBIT, BTN_TR);

    struct uinput_setup usetup = {
        .id = {
            .bustype = BUS_USB,
            .vendor = 0x045e,
            .product = 0x028e,
            .version = 0x110,
        },
        .name = "Microsoft X-Box 360 pad",
        .ff_effects_max = 0
    };

    if (ioctl(fd, UI_DEV_SETUP, &usetup)) {
        return vcontroller_error::uinput_dev_setup;
    }

    if (ioctl(fd, UI_DEV_CREATE)) {
        return vcontroller_error::uinput_dev_create;
    }

    *ctl = vcontroller {
        .fd = fd,
        .usetup = usetup
    };

    return vcontroller_error::none;
}

static inline int get_uinput_btn(button btn) {
    switch (btn) {
    case button::A:
        return BTN_EAST;
    case button::B:
        return BTN_SOUTH;
    case button::X:
        return BTN_NORTH;
    case button::Y:
        return BTN_WEST;
    case button::dpad_up:
        return BTN_DPAD_UP;
    case button::dpad_down:
        return BTN_DPAD_DOWN;
    case button::dpad_right:
        return BTN_DPAD_RIGHT;
    case button::dpad_left:
        return BTN_DPAD_LEFT;
    case button::start:
        return BTN_START;
    case button::select:
        return BTN_SELECT;
    case button::trigger_left:
        return BTN_TL;
    case button::trigger_right:
        return BTN_TR;
    default:
        return 0;
    }
}

vcontroller_error vcontroller_update(vcontroller* ctl, uint16_t state) {
    struct input_event ev[BUTTON_COUNT + 1];

    for (int i = 0; i < BUTTON_COUNT; i++) {
        bool is_set = (state & (1 << i)) != 0;

        ev[i].type = EV_KEY;
        ev[i].code = get_uinput_btn(static_cast<button>(i));
        ev[i].value = is_set ? 1 : 0;
    }

    ev[BUTTON_COUNT] = {
        .time = { .tv_sec = 0, .tv_usec = 0 },
        .type = EV_SYN,
        .code = SYN_REPORT,
        .value = 0,
    };

    int write_result = write(ctl->fd, &ev, sizeof(ev));
    if (write_result < 0) {
        return vcontroller_error::uinput_write;
    }

    return vcontroller_error::none;
}

void vcontroller_destroy(vcontroller* ctl) {
    ioctl(ctl->fd, UI_DEV_DESTROY);
    close(ctl->fd);
}

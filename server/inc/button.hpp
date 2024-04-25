#pragma once

#include <cstdint>

// this is the same order as KEYPAD_BITS in libnds/input.h
enum class button : uint16_t {
    A,
    B,
    select,
    start,
    dpad_right,
    dpad_left,
    dpad_up,
    dpad_down,
    trigger_right,
    trigger_left,
    X,
    Y,
};

constexpr int BUTTON_COUNT = 12;
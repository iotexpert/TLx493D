#pragma once
#include "TLx493D.h"

typedef struct {
    int8_t x;
    int8_t y;
} TLxJoyStickXY_t;

// Convert a joystick frame to an XY position
void TLxJoystickCovertXY(TLx493D_data_frame_t *frame, TLxJoyStickXY_t *result);

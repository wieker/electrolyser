
#pragma once



enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
};
typedef enum {
    X = 0,
    Y,
    Z
} sensor_axis_e;

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif /* M_PI */

#define RADX10 (M_PI / 1800.0f)                  // 0.001745329252f
#define RAD    (M_PI / 180.0f)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))

uint32_t micros(void);
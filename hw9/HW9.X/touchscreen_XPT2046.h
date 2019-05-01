#ifndef TOUCHSREEN_H_
#define TOUCHSREEN_H_

#define SS  LATBbits.LATB9 //
struct touch_read{
    unsigned short x;
    unsigned short y;
    int z;
};

void touchscreen_init();
struct touch_read read_touchpad();
struct touch_read read_touchinpixel();
#endif
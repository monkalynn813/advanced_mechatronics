#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include<stdio.h>
#include "ili9341.h"
#include "touchscreen_XPT2046.h"

#define X_ADD 0b11010001 //address of x position
#define Y_ADD 0b10010001 //address of y position
#define Z1_ADD 0b10110001 //address of z1 pressure
#define Z2_ADD 0b11000001 //address of z2 pressure


void touchscreen_init(){
    TRISBbits.TRISB9=0;  //chip selection at B9 output
    SS =1;
}

unsigned short read_x(){
    SS =0;
    spi_io(X_ADD);
    unsigned short c1=spi_io(0x00);
    unsigned short c2=spi_io(0x00);
    
    SS=1; 
    unsigned short r=(c2>>3) | (c1<<5);
    return r;
}

unsigned short read_y(){
    SS =0;
    spi_io(Y_ADD);
    unsigned short c1=spi_io(0x00);
    unsigned short c2=spi_io(0x00);
    
    SS=1; 
    unsigned short r=(c2>>3) | (c1<<5);
    return r;
}
int read_z(){
    SS=0;
    spi_io(Z1_ADD);
    unsigned short c1= spi_io(0x00);
    unsigned short c2= spi_io(0x00);
    SS=1;
    int z1=(c2>>3)|(c1<<5);
    SS=0;
    spi_io(Z2_ADD);
    unsigned short c3= spi_io(0x00);
    unsigned short c4= spi_io (0x00);
    SS=1;
    int z2=(c4>>3)|(c3<<5);
    SS=0;
    int z= z1-z2+4095;
    return z;
}

struct touch_read read_touchpad(){
    
    struct touch_read read;
    read.x=read_x();
    read.y=read_y();
    read.z=read_z();
    return read;
}

struct touch_read read_touchinpixel(){
    struct touch_read read;
    read.x=read_x()*240/4095;
    read.y=(4095-read_y())*360/4095;
    return read;
}


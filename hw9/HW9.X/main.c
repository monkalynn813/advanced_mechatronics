#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include<stdio.h>
#include "ili9341.h"
#include "lcd_lib.h"
#include "touchscreen_XPT2046.h"
// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module



int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4=0;
    
    SPI1_init();
    LCD_init();
    touchscreen_init();
    
    
    __builtin_enable_interrupts();
    
        
    
    char press_lower=0;
    char press_upper=0;
    char press_temp_l=0;
    char press_temp_h=0;
    int i =0;
    char msg[100];
    struct touch_read press_read;
    struct touch_read press_read_pix;
    
    LCD_clearScreen(ILI9341_BLACK);
    sprintf(msg,"  I-");
    LCD_drawButton(130,250,25,18,ILI9341_ORANGE,ILI9341_ORANGE,ILI9341_BLACK,msg,press_lower);
    sprintf(msg,"  I+");
    LCD_drawButton(130,150,25,18,ILI9341_ORANGE,ILI9341_ORANGE,ILI9341_BLACK,msg,press_upper);
    
    
    while(1) {
        _CP0_SET_COUNT(0);
        LATAbits.LATA4=!LATAbits.LATA4;
        //store press indicator from last loop in temp value
        press_temp_l=press_lower;
        press_temp_h=press_upper;
        
        
        press_read=read_touchpad();
        press_read_pix=read_touchinpixel();
        
        sprintf(msg,"x raw =    %d     ",press_read.x);
        LCD_print(msg,28,32,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"y raw=     %d     ", press_read.y);
        LCD_print(msg,28,40,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"z=     %d    ", press_read.z);
        LCD_print(msg,28,48,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"FPS: %5.2f", 1.0/(_CP0_GET_COUNT()/24000000.0));
        LCD_print(msg,28,100,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"x =    %d     ",press_read_pix.x);
        LCD_print(msg,28,60,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"y=     %d     ", press_read_pix.y);
        LCD_print(msg,28,68,ILI9341_ORANGE,ILI9341_BLACK);
        sprintf(msg,"I=  %d     ", i);
        LCD_print(msg,130,200,ILI9341_ORANGE,ILI9341_BLACK);
        
        
        if(press_read_pix.x>105 & press_read_pix.x<155 & press_read_pix.y>232 & press_read_pix.y <268 & press_read.z>1500){
                press_lower=1;} else{press_lower=0;}
        
        if(press_read_pix.x>105 & press_read_pix.x<155 & press_read_pix.y>132 & press_read_pix.y <168 & press_read.z>1500){
                press_upper=1;} else{press_upper=0;}
        
        LCD_drawBoundary(130,250,25,18,ILI9341_BLUE,ILI9341_WHITE, press_lower);
        LCD_drawBoundary(130,150,25,18,ILI9341_BLUE,ILI9341_WHITE, press_upper);
        
        //debounce function
        
        if(press_temp_l==1 & press_lower==0 ){i--;}
        if(press_temp_h==1 & press_upper==0 ){i++;}

        
        while(_CP0_GET_COUNT() < 2400000){;}

        }
        
    }



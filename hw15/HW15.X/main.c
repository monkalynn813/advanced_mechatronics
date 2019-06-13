#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "motor_control.h"
#include "ili9341.h"
#include "lcd_lib.h"

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

//prototypes
void generate_random_array(unsigned char*array,int array_size);


void __ISR(_TIMER_3_VECTOR,IPL5SOFT)SetMotor(void){
    
    static volatile int counter=0, dir=1;
    
    set_pwm(counter*dir);
    
    counter++;
    if (counter>100){
        counter=0;
        dir=(-1)*dir;
    }
    
    
    IFS0bits.T3IF=0;
}


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

    motor_control_init();
    SPI1_init();
    LCD_init();

    __builtin_enable_interrupts();
    
    _CP0_SET_COUNT(0);
    LCD_clearScreen(ILI9341_BLACK);

    unsigned char r_array[240];
    
    int r_array_lim[2]={0,200};
    char msg[100];
    sprintf(msg,"R_pot");
    
    while(1) {
        generate_random_array(r_array,240);
        LCD_plot(0,360,r_array_lim,ILI9341_ORANGE,ILI9341_BLACK,msg,r_array);
        LCD_plot(0,240,r_array_lim,ILI9341_ORANGE,ILI9341_BLACK,msg,r_array);
        LCD_plot(0,120,r_array_lim,ILI9341_ORANGE,ILI9341_BLACK,msg,r_array);
//        LCD_clearScreen(ILI9341_BLACK);
    }
}

void generate_random_array(unsigned char*array,int array_size){
    static int i;
    for (i=0;i<array_size;i++){
        array[i]=(rand()%200);
    }
}
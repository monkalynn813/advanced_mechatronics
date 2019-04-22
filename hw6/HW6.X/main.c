#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include<stdio.h>
#include "ili9341.h"
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

//Prototypes

void LCD_printletter(char letter,unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc);
void LCD_print(char*msg, unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc);
void LCD_bar(int progress, unsigned short x, unsigned short y, unsigned short pc, unsigned short bc);

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
    
    __builtin_enable_interrupts();
    
    
    
    LCD_clearScreen(ILI9341_BLACK);
    int i=0;
    while(1) {
        _CP0_SET_COUNT(0);
                
        char msg[100];
        sprintf(msg,"Hello World!  %d",i);
        LCD_print(msg,28,32,ILI9341_ORANGE,ILI9341_BLACK);
        LCD_bar(i,28,50,ILI9341_BLUE ,ILI9341_WHITE);
        sprintf(msg,"FPS: %5.2f", 1.0/(_CP0_GET_COUNT()/24000000.0));
        LCD_print(msg,28,100,ILI9341_ORANGE,ILI9341_BLACK);
        
        i++;
        while(_CP0_GET_COUNT() < 2400000){;}
        if (i==101){
            break;
        }
        
    }
}

void LCD_print(char*msg, unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc){
    int t=0;
    while(msg[t]){
        LCD_printletter(msg[t],x+(t*5),y,fc,bgc);
        t=t+1;
    }
    
}
void LCD_printletter(char letter,unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc){
    int i,j;
    for (i=0;i<5;i++){
        char col=ASCII[letter-0x20][i];
        for (j=0;j<8;j++){
            char bit = (col>>j);
            if ((bit & 0b01)==1){
                LCD_drawPixel(x+i,y+j,fc);
            }else {
                LCD_drawPixel(x+i,y+j,bgc);
            }
        }
    }
}
void LCD_bar(int progress, unsigned short x, unsigned short y, unsigned short pc, unsigned short bc){
    int i, j;
    int bar_height=8;
    for (i=progress;i<100;i++){
        for (j=0;j<bar_height;j++){
            LCD_drawPixel(x+i,y+j,bc); //draw the background bar
        }
    }
    for(i=0;i<progress;i++){
        for (j=0;j<bar_height;j++){
            LCD_drawPixel(x+i,y+j,pc); //draw the process bar
        }
    }
     
}
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <stdio.h>
#include "ili9341.h"
#include "lcd_lib.h"
#include "motor_control.h"
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
#pragma config PMDL1WAY = ON // allow multiple reconfigurations
#pragma config IOL1WAY = ON // allow multiple reconfigurations
#pragma config FUSBIDIO = OFF // USB pins controlled by USB module
#pragma config FVBUSONIO = OFF // USB BUSON controlled by USB module


#define USER PORTBbits.RB4
#define TESTDUR 500

int traj=4;
char msg[100];



void startup() {
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // pin definitions
    ANSELA = 0;
    ANSELB = 0;
    TRISAbits.TRISA7 = 0; // DIR2
    DIR2 = 1;
    TRISAbits.TRISA10 = 0; // DIR1
    DIR1 = 1;
    TRISBbits.TRISB4 = 1; // USER
    
    // OC1 is B15, goes with DIR1
    // OC4 is A4, goes with DIR2
    motor_control_init();
    
    // LCD uses SPI1: A0 is SDO, A1 is SDI, B5 is CST, B14 is SCK1, A9 is DC, B7 is CS
    SPI1_init();
    LCD_init();
    
    // Camera uses C0-7, C8 as OC2, A8 as INT3/PCLK, B13 as INT2/HREF, C9 as INT1/VSYNC, and I2C1
    
    // B3 is available as SCL2, B2 is available as SDA2
    
    
    
    
}

void __ISR(_TIMER_3_VECTOR,IPL5SOFT)SetMotor(void){
    
    static volatile int counter=0,dir=1,p=100,set=1;
   
    //TEST PWM
//    if (set==1){
//    if(counter%5==0){
//        p--;
//    }
//    
//    
//    set_pwm(1,p*dir);
//    set_pwm(2,p*dir);
//    }else{
//        set_pwm(1,100);
//        set_pwm(2,100);
//    }
//    sprintf(msg,"%d    ",p   );
//        counter++;
//    if (counter>500){
//        p=100;
//        counter=0;
//        set=0;
//        
//        dir=(-1)*dir;
//    }
    
    switch (traj)
    {
        case 0:{ //move forward
            
            set_pwm(1,90,1);
            set_pwm(2,90,1);
            counter++;
            if (counter>TESTDUR){
                counter=0;
                traj=1;
            }
            sprintf(msg,"move forward    ");  
            break;
        }
        case 1:{ //move backward
            
            set_pwm(1,90,0);
            set_pwm(2,90,0);
            counter++;
            if(counter>TESTDUR){
                counter=0;
                traj=2;
            }sprintf(msg,"move backward   ");  
            break;
            
        }
        case 2:{ //rotate to left
            set_pwm(1,60,1);
            set_pwm(2,60,0);    
            counter++;
            if(counter>TESTDUR){
                counter=0;
                traj=3;              
            }sprintf(msg,"rotate to left   ");  
            break;
        }
        case 3:{ //rotate to right
            set_pwm(1,60,0);
            set_pwm(2,60,1);
            counter++;
            if(counter>TESTDUR){
                counter=0;
                traj=4;              
            }sprintf(msg,"rotate to right   ");  
            
            break;
        }
        case 4:{
            set_pwm(1,2,1);
            set_pwm(2,2,1);
            sprintf(msg,"IDLE             ");  
            break;
            
        }
        
        default:
            sprintf(msg,"NONE             ");  
            break;
    }
    IFS0bits.T3IF=0;
}

int main() {

    __builtin_disable_interrupts();

    startup();

    __builtin_enable_interrupts();
    
//    sprintf(msg,"LCD is working");  
    LCD_clearScreen(ILI9341_BLACK);

    while(1) {

//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<48000000/2/2){
//            while(USER == 0){}
//        }
//        DIR1 = 0;
//        DIR2 = 1;
//        
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<48000000/2/2){
//            while(USER == 0){}
//        }
//        DIR1 = 1;
//        DIR2 = 0;
        if (USER==0){traj=0;}
        //TEST LCD
        LCD_print(msg,28,32,ILI9341_ORANGE,ILI9341_BLACK);
        
        

        
    }
}
#include <xc.h>
#include "motor_control.h"
#include<sys/attribs.h>  // __ISR macro
#include<math.h>

void timer2_init(){
    //timer 2 for PWM (20kHz)
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR2 = 2399; // PR = PBCLK / N / desiredF - 1 

    TMR2 = 0; // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC1RS = 0; // duty cycle

    OC1R = 0; // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1; // turn on Timer2
    
    
    OC1CONbits.ON = 1; // turn on OC1
}

void timer3_init(){
    //timer3 for interrupt (100Hz)
    T3CONbits.TCKPS=1; //N=1
    PR3= 239999;
    TMR3 =0;
    T3CONbits.ON=1;
    IPC3bits.T3IP=5;
    IFS0bits.T3IF=0;
    IEC0bits.T3IE=1;
    
}

void motor_control_init(){
    RPA0Rbits.RPA0R=0b0101;    //set pin 2 (RA0) to OC1 function
    TRISAbits.TRISA4 =0; //set pin 12 (RA4) to I/O pin

    timer2_init(); //pwm timer
    timer3_init(); //interrupt timer
}

void set_dir(int k){
    /*k : desired percentage of pwm*/
    if (k<0){
        DP =0;
    }else if (k>1){
        DP=1;
    }
}
void set_pwm(int k){
    /*k: desired percentage of pwm*/
    OC1RS=(PR2+1)*abs(k)/100;
    set_dir(k);
}
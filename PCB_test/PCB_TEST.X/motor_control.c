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

    OC4CONbits.OCM=0b110;
    OC4RS=0;
    OC4R=0;
    
    T2CONbits.ON = 1; // turn on Timer2
        
    OC1CONbits.ON = 1; // turn on OC1
    OC4CONbits.ON = 1;
}

void timer3_init(){
    //timer3 for interrupt (100Hz) (cpu: 48Mhz)
    T3CONbits.TCKPS=0b100; //N=16
    PR3= 29999;
    TMR3 =0;
    T3CONbits.ON=1;
    IPC3bits.T3IP=5;
    IFS0bits.T3IF=0;
    IEC0bits.T3IE=1;
    
}

void motor_control_init(){
    RPB15Rbits.RPB15R=0b0101;    //set pin  (RB15) to OC1 function
    RPA4Rbits.RPA4R=0b0101; //set pin (RBA4) to OC2 function
    TRISAbits.TRISA10 = 0; // set pin (RA10) to I/O pin --> DIR1
    TRISAbits.TRISA7 =0; //set pin (RA7) to I/O pin --> DIR2
    DIR1=1;
    DIR2=1;
//    
    timer2_init(); //pwms timer
    timer3_init(); //interrupt timer
}

//void set_dir(int motor,int k, int dir){
//    /* motor: index of motor 
//     * k : desired percentage of pwm*/
//    if (motor==1){
//        if (k<0){
//            DIR1 =0;
//        }
//        else{
//            DIR1 =1;
//        }
//    }
//    else if(motor==2){
//        if (k<0){
//            DIR2 =0;
//        }
//        else{
//            DIR2 =1;
//        }
//    }
//}
void set_pwm(int motor,int k, int dir){
    /*k: desired percentage of pwm*/
    int p=0;
    if (dir==0){p=k;}
    if (dir==1){p=100-k;}
    
    if(motor==1){
        OC1RS=(PR2+1)*p/100;
        DIR1 = dir;
//        set_dir(motor,k,dir);
    }else if(motor==2){
        OC4RS=(PR2+1)*p/100;
//        set_dir(motor,k,dir);
        DIR2 = dir;
    }
}
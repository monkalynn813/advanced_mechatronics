#ifndef TIMER_H_
#define TIMER_H_

#define DP LATAbits.LATA4 /*direction pin :RA4*/ 

void motor_control_init();
void set_pwm(int k);

#endif
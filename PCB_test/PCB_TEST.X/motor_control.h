#ifndef TIMER_H_
#define TIMER_H_
#define DIR1 LATAbits.LATA10
#define DIR2 LATAbits.LATA7



void motor_control_init();
void set_pwm(int motor,int k, int dir);

#endif
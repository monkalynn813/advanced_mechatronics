#ifndef FILTER_H_
#define FILTER_H_


float moving_average(short v, int order,int counter,short* buffer);
float fir_filter(short v,int order,int counter,float * coef,short* buffer);
float iir_filter(short v, float *coef,float iir);
#endif
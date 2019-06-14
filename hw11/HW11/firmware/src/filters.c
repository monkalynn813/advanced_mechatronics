#include <stdio.h>
#include <xc.h>
#include "filters.h"
#include "filters.h"
float moving_average(short v, int order,int counter,short* buffer){
    /*function returns average value in moving window
     * v: value of single sample point 
     * order: buffer size, average operation will be proceed when buffer size = order
     * counter: counter if put this function in a loop
     * buffer: temp buffer array, array size = order
    */
    float r;
    int sum=0,k=0;

    if (counter<order){
        buffer[counter]=v;
        r=0;
    }else{
        for(k=0;k<order;k++){
            sum=sum + buffer[k];
        }
        
        r= (float)sum/order;
        
        for(k=0;k<order-1;k++){
            buffer[k]=buffer[k+1]; //shift data when new data received
        }
        buffer[order-1]=v;
               
    }

    return r;
}

float fir_filter(short v,int order,int counter,float * coef,short* buffer){
    /* function returns sum of value with assigned weight
     * v: value of single sample point
     * order: buffer size
     * counter: counter if put this function in a loop
     * coef: array of weights, array size = order
     * buffer: temp buffer array, array size = order
     */
    float r=0;
    int k=0;
    
    if (counter<order){
        buffer[counter]=v;

    }else{
        for(k=0;k<order;k++){
            r= r+ coef[k]*buffer[k];
        }
        
        
        for(k=0;k<order-1;k++){
            buffer[k]=buffer[k+1]; //shift data when new data received
        }
        buffer[order-1]=v;
               
    }

    return r;
    
}
float iir_filter(char v, float *coef,float iir){
    /* function return sum of previous filtered value and current value with assigned weight
     * value of single sample point
     * coef: array of weights size=2 A+B=1;
     * iir: value of initial filtered value
     */
    iir= iir*coef[0]+v*coef[1];
    return iir;

}
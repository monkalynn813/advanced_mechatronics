#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include<stdio.h>
#include "ili9341.h"
#include "lcd_lib.h"


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
void LCD_x_y_cross(short x, short y,unsigned short pc, unsigned short bc){
    /*Function draws progress cross bar at center of LCD
     * x: amount to fill in x axis
     * y: amount to fill in y axis
     * pc: fill color
     * bc: cross background color
     */
    float scale=0.02;
    x=(-1)*x*scale+120;
    y=(-1)*y*scale+160;
//    char msg[100];
//    sprintf(msg,"%d",y);
//    LCD_print(msg,120,32,ILI9341_ORANGE,ILI9341_BLACK);
    
    int i,j,k,l,m,n;
    int thickness=4;
    
    if (x>120){
        for(i=0;i<120;i++){
            for (j=158;j<158+thickness;j++){
                LCD_drawPixel(i,j,bc);
            }
        }
        for (k=x;k<240;k++){
            for (l=158;l<158+thickness;l++){
                LCD_drawPixel(k,l,bc);
            }
        }
        for (m=120;m<x;m++){
            for(n=158;n<158+thickness;n++){
                LCD_drawPixel(m,n,pc);
            }
        }
    }else {
        for (i=240;i>120;i--){
            for (j=158;j<158+thickness;j++){
                LCD_drawPixel(i,j,bc);
            }
        }
        for (k=0;k<x;k++){
            for (l=158;l<158+thickness;l++){
                LCD_drawPixel(k,l,bc);
            }
        }
        for (m=x; m<119; m++){
            for (n=158;n<158+thickness;n++){
                LCD_drawPixel(m,n,pc);
            }
        }
    }
//    
    if (y>160){
        for(i=0;i<160;i++){
            for (j=118;j<118+thickness;j++){
                LCD_drawPixel(j,i,bc);
            }
        }
        for (k=y;k<320;k++){
            for (l=118;l<118+thickness;l++){
                LCD_drawPixel(l,k,bc);
            }
        }
        for (m=160;m<y;m++){
            for(n=118;n<118+thickness;n++){
                LCD_drawPixel(n,m,pc);
            }
        }
    }else {
        for (i=320;i>160;i--){
            for (j=118;j<118+thickness;j++){
                LCD_drawPixel(j,i,bc);
            }
        }
        for (k=y;k>0;k--){
            for (l=118;l<118+thickness;l++){
                LCD_drawPixel(l,k,bc);
            }
        }
        for (m=160; m>y; m--){
            for (n=118;n<118+thickness;n++){
                LCD_drawPixel(n,m,pc);
            }
        }
    }
}

void LCD_plot(short x, short y, int*ylim, unsigned short pc, unsigned short bc, char*msg, unsigned char*data){
    /*function plots an array of 240 data at certain location on LCD
     * x, y: position of left-bottom corner of the plot, (set x= 0 to let full 240 data to show)
     * ylim: two numbers array indicates y-axis data range, eg.[0,200](each plot takes 110 pixels + 10pixels for title in height to show)
     * pc: plot line color
     * bc: LCD background color
     * msg: title of the plot
     * data: array of size 240
     */
    
    int i,plotvalue,plotposition_y,j;
    float scalar=110.0/(ylim[1]-ylim[0]);
    
    LCD_print(msg,110,y,pc,bc);
    
    for(i=0;i<240;i++){ //clear previous plots
        for (j=y-8;j>y-120;j--){
            LCD_drawPixel(i,j,bc);
        }
    }
    for(i=0;i<240;i++){
//        data[i]=data[i]>>5;
        plotvalue=(data[i]-ylim[0])*scalar;
        plotposition_y= y-plotvalue-10;
        LCD_drawPixel(i,plotposition_y,pc);

        
        char test[100];
        sprintf(test,"%2.3f",scalar);
        LCD_print(test,10,10,pc,bc);
       
    }            
    
}
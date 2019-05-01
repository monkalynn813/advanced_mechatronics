#ifndef LCD_LIB_H_
#define LCD_LIB_H_


void LCD_printletter(char letter,unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc);
void LCD_print(char*msg, unsigned short x, unsigned short y, unsigned short fc, unsigned short bgc);
void LCD_bar(int progress, unsigned short x, unsigned short y, unsigned short pc, unsigned short bc);
void LCD_x_y_cross(short x, short y,unsigned short pc, unsigned short bc);
void LCD_drawButton(unsigned short x, unsigned short y, int wide, int height, unsigned short bc, unsigned short pc, unsigned short fc,char *msg,char press);
void LCD_drawBoundary(unsigned short x, unsigned short y, int wide, int height, unsigned short bc, unsigned short pc, char press);
#endif
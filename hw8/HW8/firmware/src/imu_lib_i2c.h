#ifndef IMU_LIB_I2C_H_
#define IMU_LIB_I2C_H_

#define OPCODE 0b1101011
#define OUT_TEMP_L 0x20

void initIMU();
void setIMU(char pin, char bits);
char IMU_test();
signed short* get_multi_IMU(unsigned char reg, int length,short* output);
void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char *data, int length);
#endif
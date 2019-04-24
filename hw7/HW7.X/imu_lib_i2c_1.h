#ifndef IMU_LIB_I2C_H_
#define IMU_LIB_I2C_H_

#define OPCODE 0b1101011
#define OUT_TEMP_L 0x20

void initIMU();
void setIMU();
signed short* getIMU(unsigned char reg, int length);

#endif
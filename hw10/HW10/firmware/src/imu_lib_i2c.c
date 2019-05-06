#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include "i2c_master_noint.h"
#include "imu_lib_i2c.h"



void initIMU(){
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;
    i2c_master_setup();
    setIMU(0x10,0b10000010); // CTRL1_XL register with sample rate 1.66kHz, 2g sensitivity, 100Hz filter
    setIMU(0x11,0b10001000); //CTRL2_G register with sample rate 1.66kHz, 1000 dps sensitivity
     //CTRL3_C register. IF_INC allows multiple read (default on)
}
void setIMU(char pin, char bits){
    i2c_master_start();
    i2c_master_send(0b11010110); //set Opcode +W
    i2c_master_send(pin); //register address
    i2c_master_send(bits); // send data
    i2c_master_stop();
}

void I2C_read_multiple(unsigned char address, unsigned char reg, unsigned char *data, int length){
    /* Function to read data from sequential registers
     * each register send a byte (8-bit data)
     *address: device op code (7bit)
     *reg: register to start read from
     *data: pointer of stored data array 
     * length: size of storage array (number of registers to read)
     */
    i2c_master_start();
    i2c_master_send((address<<1) | 0); //set device opcode + Write
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send((address<<1) | 1); //set device opcode + Read
    
    int i;
    for(i=0;i<length;i++){
        data[i]=i2c_master_recv(); //get data from each register of slave
        if (i==length-1){
            i2c_master_ack(1);} //indicate more data is required to sent from slave (sequential registers)
        else {
            i2c_master_ack(0); //indicate no more data is required
            } 
        }
    i2c_master_stop();
//    return data;
    }
    
signed short* get_multi_IMU(unsigned char reg, int length,short *output){
    unsigned char read[length];
    
    int i;
    I2C_read_multiple(0b1101011,reg,read,length);
    // reconstruct unsigned char array (8bit each element) to signed short array (16bit each element)           
    for (i=0;i<(length/2);i++){
        output[i]=(read[2*i+1]<<8 | read[2*i]);
    }
    return output;
}

char IMU_test(){  //read from IMU register 'who_am_i '
    i2c_master_start();
    i2c_master_send(0b11010110); //set Opcode +W (MCP23008)
    i2c_master_send(0xF);       //read register WHO_AM_I
    i2c_master_restart();       //restart
    i2c_master_send(0b11010111); //set Opcode +R
    char r = i2c_master_recv(); //get data from slave
    i2c_master_ack(0);          //send nack indicates no more bytes requested from slave
    i2c_master_stop();          //stop
    
    return r;
}


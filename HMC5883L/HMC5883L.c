#include <stdio.h>
#include <string.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <math.h>
#include "HMC5883L.h"



int Multipul_Read(unsigned char regaddr, unsigned char *buf, unsigned char len)
{
    unsigned char WriteBuf[1];
    int rc;
    if (!bcm2835_init())
    return 1;
    WriteBuf[0] = regaddr;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(HMC5883L_Addr);
    bcm2835_i2c_set_baudrate(HMC5883L_Drate);
    bcm2835_i2c_write( WriteBuf, 1);
    
    rc = bcm2835_i2c_read(buf, len);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}

unsigned char Single_Read_HMC5883L(unsigned char REG_Address)
{
    unsigned char WriteBuf[1];
    unsigned char ReadBuf[1];
    int rc;
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(HMC5883L_Addr);
    bcm2835_i2c_set_baudrate(HMC5883L_Drate);
    WriteBuf[0] = REG_Address;
    bcm2835_i2c_write( WriteBuf, 1);
    rc = bcm2835_i2c_read(ReadBuf, 1);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();
    return ReadBuf[0]; 
}

int Single_Write_HMC5883L(unsigned char REG_Address,unsigned char REG_data)
{
    unsigned char WriteBuf[2];
    unsigned char ReadBuf[1];
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(HMC5883L_Addr);
    bcm2835_i2c_set_baudrate(HMC5883L_Drate);

    WriteBuf[0] = REG_Address;
    WriteBuf[1] = REG_data;
    bcm2835_i2c_write( WriteBuf, 2);
    
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}

void Init_HMC5883(void)
{
    Single_Write_HMC5883L(0x02,0x00);  //
}

void Read_HMC5883_FIFO(void)
{
    unsigned char ReadBuf[fifo_width];
    unsigned char i;
    short int x,y,z;
    double angle[3];
    
    Multipul_Read(0x03, ReadBuf, fifo_width);
    
    for(i = 0;i<fifo_width;i++){        
        printf("ReadBuf[%d] = %d\n",i,ReadBuf[i]);
    }
    
    x = (ReadBuf[0]<<8) + ReadBuf[1];
    z = (ReadBuf[2]<<8) + ReadBuf[3];
    y = (ReadBuf[4]<<8) + ReadBuf[5];
    
    angle[0] = atan2((double)y,(double)x) * (180 / 3.14159265) + 180; 
    //angle[0] *=10;
    angle[1] = atan2((double)z,(double)y) * (180 / 3.14159265) + 180; 
    //angle[1] *=10;
    angle[2] = atan2((double)x,(double)z) * (180 / 3.14159265) + 180; 
    //angle[2] *=10;
    
    printf("x = %d, y=%d, z=%d, a1=%lf, a2=%lf, a3=%lf\n",x,y,z,angle[0],angle[1],angle[2]);
 
}


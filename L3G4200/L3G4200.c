#include <stdio.h>
#include <string.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <math.h>
#include "L3G4200.h"

unsigned char Single_Read_L3G4200(unsigned char REG_Address)
{
    unsigned char WriteBuf[1];
    unsigned char ReadBuf[1];
    int rc;
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(L3G4200_Addr);
    bcm2835_i2c_set_baudrate(L3G4200_Drate);
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

int Single_Write_L3G4200(unsigned char REG_Address,unsigned char REG_data)
{
    unsigned char WriteBuf[2];
    unsigned char ReadBuf[1];
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(L3G4200_Addr);
    bcm2835_i2c_set_baudrate(L3G4200_Drate);

    WriteBuf[0] = REG_Address;
    WriteBuf[1] = REG_data;
    bcm2835_i2c_write( WriteBuf, 2);
    
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}

void Init_L3G4200D(void)
{
    Single_Write_L3G4200(CTRL_REG1, 0x0f);
    Single_Write_L3G4200(CTRL_REG2, 0x00);
    Single_Write_L3G4200(CTRL_REG3, 0x08);
    Single_Write_L3G4200(CTRL_REG4, 0x30);	//+-2000dps
    Single_Write_L3G4200(CTRL_REG5, 0x00);
}	

void READ_L3G4200D(void)
{
    unsigned char ReadBuf[fifo_width];
    short int x,y,z;
    unsigned char i;
    float temp_dis[3];
    
    ReadBuf[0]=Single_Read_L3G4200(OUT_X_L);
    ReadBuf[1]=Single_Read_L3G4200(OUT_X_H);
    x =	(ReadBuf[1]<<8)|ReadBuf[0];

    ReadBuf[2]=Single_Read_L3G4200(OUT_Y_L);
    ReadBuf[3]=Single_Read_L3G4200(OUT_Y_H);
    y =	(ReadBuf[3]<<8)|ReadBuf[2];

    ReadBuf[4]=Single_Read_L3G4200(OUT_Z_L);
    ReadBuf[5]=Single_Read_L3G4200(OUT_Z_H);
    z =	(ReadBuf[5]<<8)|ReadBuf[4];
    
    for(i=0;i<fifo_width;i++)
        printf("ReadBuf[%d] = %d\n",i,ReadBuf[i]);
    
    printf("x = %f, y = %f, z = %f\n",x,y,z);

    temp_dis[0] = (float)x*0.07;
    temp_dis[1] = (float)y*0.07;
    temp_dis[2] = (float)z*0.07;
    
    printf("temp_dis[0] = %f, temp_dis[1] = %f, temp_dis[2] = %f\n",temp_dis[0],temp_dis[1],temp_dis[2]);
}

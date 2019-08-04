#include "common.h"
#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char I2C_1_ReadBytes(unsigned char dev_addr, unsigned char reg_addr, \
                                unsigned int baudrate, unsigned char *buf, \
                                unsigned char len)
{
    unsigned char WriteBuf[1];
    int rc;
    if (!bcm2835_init())
        return 1;

    WriteBuf[0] = reg_addr;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(dev_addr);
    bcm2835_i2c_set_baudrate(baudrate);
    bcm2835_i2c_write(WriteBuf, 1);
    
    rc = bcm2835_i2c_read(buf, len);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Error: Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();

    return 0;
}

unsigned char I2C_1_WriteBytes(unsigned char dev_addr, unsigned char reg_addr, \
                                unsigned int baudrate, unsigned char *buf, \
                                unsigned char len)
{
    unsigned char *WriteBuf;

    if (!bcm2835_init())
        return 1;

    WriteBuf = (unsigned char *)malloc(len+1);
    if(WriteBuf == NULL){
        printf("Memory allocate failed.\n");
        return 1;
    }

    memcpy(WriteBuf+1, buf, len);
    WriteBuf[0] = reg_addr;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(dev_addr);
    bcm2835_i2c_set_baudrate(baudrate);

    bcm2835_i2c_write(WriteBuf, len + 1);

    free(WriteBuf);
    
    bcm2835_i2c_end();
    bcm2835_close();

    return 0;
}


/* End of this file. */
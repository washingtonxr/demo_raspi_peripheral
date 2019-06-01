#include "common.h"

unsigned char Start_bcm2835(void)
{
    if (!bcm2835_init()){
        printf("BCM2835 Initializing failed.\n");
        return RUN_FAIL;
    }
    return RUN_OK;
}

unsigned char Stop_bcm2835(void)
{
    bcm2835_close();
    return RUN_OK;
}

unsigned char I2C_Multipul_Read(unsigned char addr, unsigned char regaddr, 
                                unsigned char *buf, unsigned char len)
{
    unsigned char WriteBuf[1];
    int ret;

    if (Start_bcm2835() != RUN_OK)
    return RUN_FAIL;

    WriteBuf[0] = regaddr;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(addr);
    bcm2835_i2c_set_baudrate(I2Cdatarate);
    bcm2835_i2c_write( WriteBuf, 1);
    
    ret = bcm2835_i2c_read(buf, len);
    if(ret != BCM2835_I2C_REASON_OK){
        printf("Read failed.(%d)\n", ret);
    }
    bcm2835_i2c_end();
    bcm2835_close();

    return RUN_OK;
}

unsigned char I2C_Single_Read(unsigned char addr, unsigned char regaddr)
{
    unsigned char WriteBuf[1];
    unsigned char ReadBuf[1];
    int ret;

    if (Start_bcm2835() != RUN_OK)
    return RUN_FAIL;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(addr);
    bcm2835_i2c_set_baudrate(I2Cdatarate);
    WriteBuf[0] = regaddr;
    bcm2835_i2c_write(WriteBuf, 1);
    ret = bcm2835_i2c_read(ReadBuf, 1);
    if(ret != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n", ret);
    }
    bcm2835_i2c_end();
    bcm2835_close();
    return ReadBuf[0]; 
}

unsigned char I2C_Single_Write(unsigned char addr, unsigned char regaddr,
                                unsigned char data)
{
    unsigned char WriteBuf[2];
    unsigned char ReadBuf[1];

    if (Start_bcm2835() != RUN_OK)
    return RUN_FAIL;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(addr);
    bcm2835_i2c_set_baudrate(I2Cdatarate);

    WriteBuf[0] = regaddr;
    WriteBuf[1] = data;
    bcm2835_i2c_write(WriteBuf, 2);
    
    bcm2835_i2c_end();
    bcm2835_close();
    return RUN_OK;
}

/* End of the file. */
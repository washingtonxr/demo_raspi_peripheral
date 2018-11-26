#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <bcm2835.h>
#include "KX022.h"

const struct WriteReg1 kx022Init[] = {
/* register    operate data */
    {0x18   ,       0x00},          //bit7(operating_mode)=0,disable controller and start setup
//  {0x1B   ,       0x52},          //bit7=1,close filter bit1=1,set ODR to 50Hz
//  {0x35   ,       0x4B},          //bit6/5/4 =100,16 samp_avg
    {0x3A   ,       0x54},          //8bit mode - max 84 0x54=84
    {0x3B   ,       0x80},          //sample buffer active & 8-bit FIFO mode
//  {0x18   ,       0x90},          //bit7(operating_mode)=1,enable controller and setup end  8g
    {0x18   ,       0x92},          //bit7(operating_mode)=1,enable controller and setup end  8g
};

void Start_bcm2835(void)
{
    if (!bcm2835_init()){
        printf("BCM2835 Initializing failed.\n");
    }
    return;
}

void Stop_bcm2835(void){
    bcm2835_close();
    return;
}

unsigned char kx022_read_regs(unsigned char register_address, unsigned char * destination, \
                                unsigned char number_of_bytes)
{
    int rc;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(SlaveAddress);
    bcm2835_i2c_set_baudrate(Boundrate);
    printf("I2C Initializing done.\n");
    rc = bcm2835_i2c_read_register_rs(&register_address, destination, number_of_bytes);
    if(!rc){
        printf("Read failed.(%d)\n", rc);
    }
    bcm2835_i2c_end();
    return 1;
}

unsigned char kx022_write_regs(const unsigned char *register_address, const unsigned char *value, \
                                unsigned char number_of_bytes)
{
    int rc;
    unsigned int bno;
    unsigned char txbuf[txbuf_len];

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(SlaveAddress);
    bcm2835_i2c_set_baudrate(Boundrate);
    printf("I2C Initializing done.\n");

    memset(txbuf, 0, sizeof(txbuf));
    memcpy(txbuf, register_address, 1);
    memcpy(txbuf + 1, value, number_of_bytes);

    rc = bcm2835_i2c_write(txbuf, number_of_bytes + 1);
    if(!rc){
        printf("Write failed.(%d)\n", rc);
    }

    printf("reg = %x, bno = %d, value = %x\n",*register_address, *value, bno);
    printf("kx022 initializing done.\n");
    bcm2835_i2c_end();
    return 1;
}

void Init_KX022(void)
{
    unsigned char i = 0;
    unsigned char IsReady = 0;

    printf("Starting Initializing KX022.\n");
    do{
        kx022_read_regs(COTR_reg, &IsReady, 1);
        i+=1;
    }while(IsReady!=0x55 && i<2);
    
    for(i=0; i<sizeof(kx022Init)/((sizeof(unsigned char)) *2); i++) {
        kx022_write_regs(&(kx022Init[i].reg_addr), &(kx022Init[i].data), 1);
    }
    printf("The end of Initializing.\n");
    
    return;
}

















#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <bcm2835.h>

char WriteBuf[1] = {0x00};
char ReadBuf[10] = {0x00,};
int main(int argc, char **argv)
{
    int i = 0;
    int rc = 0;
    unsigned int counter = 10;
    if (!bcm2835_init())
    return 1;
#if 1
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(0x77);
    bcm2835_i2c_set_baudrate(10000);
#endif
    printf("I2C initial done.\n");
    while(i<counter)
    {
        i++;
        printf("i=%d\n",i);
        WriteBuf[0] = i;
        bcm2835_i2c_write( WriteBuf , 1);
        bcm2835_delay(1);
#if 1
        rc = bcm2835_i2c_read( &ReadBuf[0] , 10);
        if(!rc){
            printf("Read failed.\n");
        }else{
            printf("ReadBuf=%d\n",ReadBuf[0]);
        }
        
        bcm2835_delay(1);
#endif
    }

    bcm2835_i2c_end();
    bcm2835_close();

    return 0;
}

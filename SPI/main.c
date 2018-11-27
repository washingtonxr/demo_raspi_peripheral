#include <stdio.h>
#include <string.h>
#include <math.h>       /* For mathmatics lib. */
#include <stdlib.h>     /* For exit. */
#include <unistd.h>     /* For Getopt. */
#include <stdbool.h>
#include <errno.h>
#include <bcm2835.h>
//#include "main.h"
#define data_len    128

#if 0
bcm2835_spi_begin();          //启动spi接口，设置spi对应管脚为复用功能
bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //高位先传输
bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);       //spi模式0     
bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);  //分频，
bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                //设置片选                  
bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);    //设置片选低电平有效

uint8_t  bcm2835_spi_transfer(uint_t value) 传输一个字节
void bcm2835_spi_transfernb(char *tbuf,char *rbuf,uint32_t len) 传输n字节
#endif


int main(int argc, char **argv)
{
    char tx_buf[data_len],rx_buf[data_len];
    unsigned int i;

    memset(tx_buf,0,sizeof(tx_buf));
    memset(rx_buf,0,sizeof(rx_buf));

    for(i=0;i<data_len;i++){
        tx_buf[i] = i;
    }

    bcm2835_init();
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

/* bcm2835_spi_setClockDivider
    BCM2835_SPI_CLOCK_DIVIDER_128   2Mbps
    BCM2835_SPI_CLOCK_DIVIDER_32    8Mbps
    BCM2835_SPI_CLOCK_DIVIDER_16    16Mbps
*/
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    bcm2835_spi_transfernb(tx_buf,rx_buf,data_len);

    for(i=0;i<data_len;i++){
        printf("rx_buf[%03d] = %02x\n",i,rx_buf[i]);
    }
    //printf("\n");

    bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);

    bcm2835_spi_transfernb(tx_buf,rx_buf,data_len);

    for(i=0;i<data_len;i++){
        printf("rx_buf[%03d] = %02x\n",i,rx_buf[i]);
    }

    //printf("\n");
    bcm2835_spi_end();
    bcm2835_close();
    return 0;
}



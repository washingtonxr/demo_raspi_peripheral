#include <stdio.h>
#include <string.h>
#include <bcm2835.h>
#include <math.h>
#include "BMP085.h"

unsigned char ge,shi,bai,qian,wan,shiwan;           //显示变量
int  dis_data;                              //变量

short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

void Init_BMP085()
{
    printf("Starting Initializing BMP085.\n");
    ac1 = Multiple_read(0xAA);
    ac2 = Multiple_read(0xAC);
    ac3 = Multiple_read(0xAE);
    ac4 = Multiple_read(0xB0);
    ac5 = Multiple_read(0xB2);
    ac6 = Multiple_read(0xB4);
    b1 =  Multiple_read(0xB6);
    b2 =  Multiple_read(0xB8);
    mb =  Multiple_read(0xBA);
    mc =  Multiple_read(0xBC);
    md =  Multiple_read(0xBE);
    printf("End of Initializing BMP085.\n");
    printf("ac1=%d,ac2=%d,ac3=%d,ac4=%d,ac5=%d,ac6=%d\n",ac1,ac2,ac3,ac4,ac5,ac6);
    printf("mb=%d,mc=%d,md=%d\n",mb,mc,md);
}

void conversion(long temp_data)  
{
    shiwan=temp_data/100000+0x30;
    temp_data=temp_data%100000;             //取余运算 
    wan=temp_data/10000+0x30;
    temp_data=temp_data%10000;              //取余运算
    qian=temp_data/1000+0x30;
    temp_data=temp_data%1000;               //取余运算
    bai=temp_data/100+0x30;
    temp_data=temp_data%100;                //取余运算
    shi=temp_data/10+0x30;
    temp_data=temp_data%10;                 //取余运算
    ge=temp_data+0x30;

}

static long bmp085_Raw_Temp(void)
{
    unsigned char WriteBuf[2] = {0x00,0x00};

    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(BMP085_SlaveAddress);
    bcm2835_i2c_set_baudrate(BMP085_Boundrate);

    WriteBuf[0] = 0xF4;
    WriteBuf[1] = 0x2E;
    bcm2835_i2c_write( WriteBuf, 2);
    bcm2835_i2c_end();
    bcm2835_delay(5);
    bcm2835_close();

    return (long) Multiple_read(0xF6);
}

float bmp085ReadTemp(void)
{
    unsigned char WriteBuf[2] = {0x00,0x00};
    long ut,x1,x2,b5;
    float Ture_Temp;
    
    ut = bmp085_Raw_Temp();

    //printf("ut = %lu.\n",ut);
    x1 = ((long)ut - ac6) * ac5 >> 15;
    //printf("x1 = %lu.\n",x1);
    x2 = (mc << 11) / (x1 + md);
    //printf("x2 = %lu.\n",x2);
    b5 = x1 + x2;
    Ture_Temp = ((b5 + 8) >> 4) / 10.0;
    return Ture_Temp;
}

static long bmp085_Raw_Pressure(void)
{
	long pressure = 0;
    unsigned char WriteBuf[2] = {0x00,0x00};

    if (!bcm2835_init()){
        printf("BCM2835 Initializing failed.\n");
        return 1;
    }
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(BMP085_SlaveAddress);
    bcm2835_i2c_set_baudrate(BMP085_Boundrate);
    printf("I2C Initializing done.\n");
    
    WriteBuf[0] = 0xF4;
    WriteBuf[1] = 0x34;
    bcm2835_i2c_write( WriteBuf, 2);
    bcm2835_i2c_end();
    bcm2835_delay(5);
    bcm2835_close();
    
	pressure = Multiple_read(0xF6);
	pressure &= 0x0000FFFF;
	
	return pressure;	
	//return (long) bmp085ReadShort(0xF6);
}

long bmp085ReadPressure(void)
{
	long UT, UP, B6, B5, X1, X2, X3, B3, p;
	unsigned long B4, B7;
	long ture_temp;
    long ture_presure;
    
	UT = bmp085_Raw_Temp();
	UP = bmp085_Raw_Pressure();
	
	X1 = ((UT - ac6) * ac5) >> 15;
	X2 = (mc << 11) / (X1 + md);
	
	B5 = X1 + X2;

	B6 = B5 - 4000;
	
	X1 = (b2 * (B6 * B6) >> 12) >> 11;
	X2 = (ac2 * B6) >> 11;
	X3 = X1 + X2;
	
	B3 = ((((ac1 * 4) + X3) << 0) + 2) / 4;
	X1 = (ac3 * B6) >> 13;
	X2 = (b1 * ((B6 * B6) >> 12)) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	
	
	B4 = ac4 * (unsigned long)(X3 + 32768) >> 15;
	B7 = ((unsigned long) UP - B3) * (50000>>0);
	
	if(B7 < 0x80000000) {
		p = (B7 * 2) / B4;
	} else {
		p = (B7 / B4) * 2;
	}
	
	X1 = (p >> 8) * (p >> 8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p = p + ((X1 + X2 + 3791) >> 4);
	
	return p;
}

float bmp180_Altitude(void){
	float p, alt;
	p = bmp085ReadPressure();
    printf("Ture_presure = %f\n",p);
	alt = 44330 * (1 - pow(( (p/100) / BMP180_SEA_LEVEL),1/5.255));
	
	return alt;
}

static short Multiple_read(unsigned char ST_Address)
{   
    unsigned char msb, lsb;
    int rc;
    short _data;
    unsigned char WriteBuf[1] = {0x00};
    unsigned char ReadBuf[2] = {0x00,0x00};
#if 0
    unsigned char RegAddr[1] = {0x00};
#endif
    if (!bcm2835_init())
    return 1;
#if 1
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(BMP085_SlaveAddress);   /* Write data. */
    bcm2835_i2c_set_baudrate(BMP085_Boundrate);
    
    WriteBuf[0] = ST_Address;
    bcm2835_i2c_write(WriteBuf, 1);
    rc = bcm2835_i2c_read(ReadBuf, 2);
    if(!rc){
        printf("Read failed.(%d)\n",rc);
    }
#else
    RegAddr[0] = ST_Address;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(BMP085_SlaveAddress); /* Read data. */
    bcm2835_i2c_set_baudrate(BMP085_Boundrate);
    
    rc = bcm2835_i2c_read_register_rs(RegAddr, ReadBuf, 2);
    if(!rc){
        printf("Read failed.(%d)\n",rc);
    }
#endif

    msb = ReadBuf[0];
    lsb = ReadBuf[1];

    bcm2835_i2c_end();
    bcm2835_close();
    
    _data = msb << 8;
    _data |= lsb;
    return _data;
}

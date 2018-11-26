#ifndef BMP085_H
#define BMP085_H
/* LSB address:0xee. */
#define	BMP085_SlaveAddress     0x77
#define BMP085_Boundrate        10000
#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)
/*
 * Average sea-level pressure in hPa
 */
#define BMP180_SEA_LEVEL 1013.25
typedef unsigned char  BYTE;
typedef unsigned short WORD;

extern void Init_BMP085();
void conversion(long temp_data);
extern float bmp085ReadTemp();
extern long bmp085ReadPressure();
static short Multiple_read(unsigned char ST_Address);
float bmp180_Altitude();
#endif

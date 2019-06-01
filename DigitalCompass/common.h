#ifndef COMMON_H
#define COMMON_H

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define I2Cdatarate     400000
#define fifo_width      6
#define PI              3.141592653589793638
#define RUN_OK          0
#define RUN_FAIL        1

typedef struct accelerometer_data{
    float x;
    float y;
    float z;
}accelerometer_data_t;

typedef struct magnetometer_data{
    short x;
    short y;
    short z;
    double roll;
    double pitch;
    double azimuth;
    double X_h;
    double Y_h;
}magnetometer_data_t;

typedef struct sensor_data{
    accelerometer_data_t acc_data;
    magnetometer_data_t mag_data;
}sensor_data_t;

unsigned char Start_bcm2835(void);
unsigned char Stop_bcm2835(void);

unsigned char I2C_Multipul_Read(unsigned char addr, unsigned char regaddr, 
                                unsigned char *buf, unsigned char len);
unsigned char I2C_Single_Write(unsigned char addr, unsigned char regaddr,
                                unsigned char data);
unsigned char I2C_Single_Read(unsigned char addr, unsigned char regaddr);

#endif
/* End of the file. */
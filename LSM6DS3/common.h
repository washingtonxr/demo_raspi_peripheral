#ifndef COMMON_H
#define COMMON_H

#define AXIS_NUM 3

/* Axis' informantion. */
typedef struct {
    float axis[AXIS_NUM];   /* x,y,z axis' accelerometer data. */
}axis_acc_info_t;

typedef struct {
    float axis[AXIS_NUM];   /* x,y,z axis' gyroscope data. */
}axis_gyr_info_t;

typedef struct {
    float temperature;        /* x,y,z axis' temperature data. */
}axis_tem_info_t;

typedef struct{
    axis_acc_info_t accelerometer;
    axis_gyr_info_t gyroscope;
    axis_tem_info_t temperature;
}axis_database_t;

unsigned char I2C_1_ReadBytes(unsigned char dev_addr, unsigned char reg_addr, \
                                unsigned int baudrate, unsigned char *buf, \
                                unsigned char len);
unsigned char I2C_1_WriteBytes(unsigned char dev_addr, unsigned char reg_addr, \
                                unsigned int baudrate, unsigned char *buf, \
                                unsigned char len);


#endif
/* End of this file. */
#include "LSM6DS3.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

struct WriteRegister{
    unsigned char reg_addr;
    unsigned char data;
};

/* Register configration table for LSM6DS3TR-C. */
struct WriteRegister SensorInit[] = {
    /* register  |  operate data */
    {LSM6DS3_ACC_GYRO_CTRL2_G, 0x1C},       // 陀螺仪12.5hz  2000dps

    {LSM6DS3_ACC_GYRO_CTRL1_XL, 0x60},      // WAKE_UP INTERRUPT Configuration
    {LSM6DS3_ACC_GYRO_TAP_CFG1, 0x90},
    {LSM6DS3_ACC_GYRO_WAKE_UP_DUR, 0x00},
    {LSM6DS3_ACC_GYRO_WAKE_UP_THS, 0x02},

    {LSM6DS3_ACC_GYRO_TAP_THS_6D, 0x40},    // 6D Orientation Configuration
    {LSM6DS3_ACC_GYRO_CTRL8_XL, 0x01},
};

static unsigned char Lsm6ds3_WriteBytes(unsigned char reg_addr, unsigned char *data, unsigned int dlen)
{
    unsigned char ret;
    ret = I2C_1_WriteBytes(LSM6DS3_ACC_GYRO_I2C_ADDRESS, reg_addr, \
                                LSM6DS3_BAUDRATE, data, dlen);
    if(ret != 0){
        printf("Error: I2C_1_WriteBytes failed.\n");
        return 1;
    }
    return 0;
}

static unsigned char Lsm6ds3_ReadBytes(unsigned char reg_addr, unsigned char *data, unsigned int dlen)
{
    unsigned char ret;
    ret = I2C_1_ReadBytes(LSM6DS3_ACC_GYRO_I2C_ADDRESS, reg_addr, \
                                LSM6DS3_BAUDRATE, data, dlen);
    if(ret != 0){
        printf("Error: I2C_1_WriteBytes failed.\n");
        return 1;
    }
    return 0;
}

void Lsm6d3s_Init(void)
{
    unsigned char ret;
    unsigned char buf;
    unsigned char i;

    while(buf != LSM6DS3_ACC_GYRO_WHO_AM_I){
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, &buf, 1);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Info: Chip ID = 0x%02x\n", buf);
        //sleep(1);
    }
//		加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
//		Lsm6d3s_WriteByte(CTRL1_XL,0x20);
//		使能加速度计x,y,z轴
//		Lsm6d3s_WriteByte(CTRL9_XL,0x38);
//		陀螺仪电平触发，加速度计高性能失能
//		Lsm6d3s_WriteByte(CTRL6_C,0x40|0x10);
//		陀螺仪高性能失能
//		Lsm6d3s_WriteByte(CTRL7_G,0x80);
//		加速度计INT2引脚失能,陀螺仪数据INT2使能
//		Lsm6d3s_WriteByte(INT2_CTRL,0x03);

    for(i = 0; i < sizeof(SensorInit)/((sizeof(unsigned char)) *2); i++) {
        Lsm6ds3_WriteBytes((SensorInit[i].reg_addr), &(SensorInit[i].data), 1);
    }

    printf("Info: Initializing LSM6D3S successfull.\n");
}

void Lsm6d3s_GetSingleRawData(void)
{
    unsigned char ret;
    unsigned char buf;
    unsigned char raw_data[256];
    short temp_data;
    unsigned char i;
    
    axis_database_t *sensor_raw_data;
    sensor_raw_data = (axis_database_t *)malloc(sizeof(axis_database_t));
    if(sensor_raw_data == NULL){
        printf("Memory allocate for sensor_raw_data failed.\n");
    }

    printf("Check sensor update--->\n");
    ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_STATUS_REG, &buf, 1);
    if(ret != 0){
        printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
    }

    if(buf&0x01 != 0){  /* Accelerometer new data available. */
        memset(raw_data, 0, 256);
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUTX_L_XL, raw_data, 6);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Accelerometer data:");
#if 0
        for(i = 0; i < 6; i++){
            printf("%d ", raw_data[i]);
        }
        printf("\n");
#endif
        for(i = 0; i < AXIS_NUM; i++){
            temp_data = 0;
            temp_data = (raw_data[2*i+1] << 8)|raw_data[2*i];
            //printf("%d ", temp_data);
            sensor_raw_data->accelerometer.axis[i] = (float)temp_data*0.98/2000;
        }
        //printf("\n");
        for(i = 0; i < AXIS_NUM; i++){
            printf("%f ", sensor_raw_data->accelerometer.axis[i]);
        }
        printf("\n");
    }
    
    if(buf&0x02 != 0){  /* Gyroscope new data available. */
        memset(raw_data, 0, 256);
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUTX_L_G, raw_data, 6);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Gyroscope data:");
#if 0
        for(i = 0; i < 6; i++){
            printf("%d ", raw_data[i]);
        }
        printf("\n");
#endif
        for(i = 0; i < AXIS_NUM; i++){
            temp_data = 0;
            temp_data = (raw_data[2*i+1] << 8)|raw_data[2*i];
            //printf("%d ", temp_data);
            sensor_raw_data->gyroscope.axis[i] = (float)temp_data;
        }
        //printf("\n");
        for(i = 0; i < AXIS_NUM; i++){
            printf("%f ", sensor_raw_data->gyroscope.axis[i]);
        }
        printf("\n");
    }

    if(buf&0x04 != 0){  /* Temperature new data available. */
        memset(raw_data, 0, 256);
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUT_TEMP_L, raw_data, 2);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Temperature data:");
#if 0
        for(i = 0; i < 2; i++){
            printf("%d ", raw_data[i]);
        }
        printf("\n");
#endif
        temp_data = (raw_data[1] << 8)|raw_data[0];
        printf("%d\n", temp_data);
        sensor_raw_data->temperature.temperature = (float)temp_data;
        printf("%f", sensor_raw_data->temperature.temperature);
        printf("\n");
    }
}










/* End of this file. */
#include "LSM6DS3.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"

struct WriteRegister{
    unsigned char reg_addr;
    unsigned char data;
};

/* Register configration table for LSM6DS3TR-C. */
struct WriteRegister SensorInit[] = {
    /*          Register        |   Data */
#if 0
    {LSM6DS3_ACC_GYRO_CTRL1_XL,     0x60},      /* Acceleraometer's ODR = 52Hz, FSG = 2G  */
    {LSM6DS3_ACC_GYRO_CTRL2_G,      0x1C},      /* Gyroscope's ODR = 12.5Hz,FSG = 2000dps */
#else
    {LSM6DS3_ACC_GYRO_CTRL1_XL,     0x40},      /* Acceleraometer's ODR = 104Hz, FSG = 2G  */
    {LSM6DS3_ACC_GYRO_CTRL2_G,      0x4C},      /* Gyroscope's ODR = 104Hz, FSG = 2000dps */
#endif
    {LSM6DS3_ACC_GYRO_TAP_CFG1,     0x90},      /* WAKE_UP INTERRUPT Configuration, HPF applied. */
    {LSM6DS3_ACC_GYRO_WAKE_UP_DUR,  0x00},      /* Timestamp register resolution is 6.4ms. */
    {LSM6DS3_ACC_GYRO_WAKE_UP_THS,  0x02},      /* Single-tap event enabled. */

    {LSM6DS3_ACC_GYRO_TAP_THS_6D,   0x40},      /* 6D Orientation Configuration. 60 degree. */
    {LSM6DS3_ACC_GYRO_CTRL8_XL,     0x01},      /* LPF2 on 6D function selected. */
#if 0
    {LSM6DS3_ACC_GYRO_CTRL5_C,      0x65},      /* Round: 0x11, Self-test in Positive sign self-test. */
#endif
    {LSM6DS3_ACC_GYRO_INT1_CTRL,    0x20},      /* INT1_FULL_FLAG. */
    {LSM6DS3_ACC_GYRO_INT2_CTRL,    0x10},      /* INT2_FIFO_OVR. */
#if 1
    {LSM6DS3_ACC_GYRO_FIFO_CTRL3,   0x09},      /* Gyro & Acc with no decimation. */
    {LSM6DS3_ACC_GYRO_FIFO_CTRL5,   0x21},      /* FIFO's ODR = 104Hz in FIFO mode stop collecting data when it's full. */
#endif

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

static void Lsm6ds3_SoftwareReset(void)
{
    unsigned char ret;
    unsigned char buf;

    ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_CTRL3_C, &buf, 1);
    if(ret != 0){
        printf("Error: Lsm6d3s_ReadBytes failed.(%02x)\n", ret);
    }

    buf = buf | 0x01;
    printf("Info: LSM6DS3_ACC_GYRO_CTRL3_C = 0x%02X\n", buf);
    ret = Lsm6ds3_WriteBytes(LSM6DS3_ACC_GYRO_CTRL3_C, &buf, 1);
    if(ret != 0){
        printf("Error: Lsm6d3s_WriteBytes failed.(%02x)\n", ret);
    }
}

void Lsm6ds3_Init(void)
{
    unsigned char ret;
    unsigned char buf;
    unsigned char i;

    while(buf != LSM6DS3_ACC_GYRO_WHO_AM_I){
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_WHO_AM_I_REG, &buf, 1);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadBytes failed.(%02x)\n", ret);
        }
        printf("Info: Chip ID = 0x%02X\n", buf);
        //sleep(1);
    }

    Lsm6ds3_SoftwareReset();
    
    printf("Info: Start initializing sensor.\n");

    for(i = 0; i < sizeof(SensorInit)/((sizeof(unsigned char)) *2); i++) {
        Lsm6ds3_WriteBytes((SensorInit[i].reg_addr), &(SensorInit[i].data), 1);
        printf("Info: 0x%02x - 0x%02x\n",SensorInit[i].reg_addr, SensorInit[i].data);
    }

    printf("Info: Initializing LSM6D3S successfull.\n");
}

static void Lsm6ds3_GetFIFOStatus(void)
{
    unsigned char i, j;
    unsigned char ret;
    unsigned char fifo_data[4];
    unsigned char tmp_data[2];
    unsigned char data_depth;
    unsigned short raw_data[256];

    ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_FIFO_STATUS1, fifo_data, 4);
    if(ret != 0){
        printf("Error: Lsm6d3s_ReadBytes failed.(%02x)\n", ret);
    }

#if 1
    printf("Info: FIFO_STATUS: 0x%02X\t0x%02X\t0x%02X\t0x%02X\n", fifo_data[0], fifo_data[1], fifo_data[2], fifo_data[3]);
#endif

    data_depth = fifo_data[0];
    if(data_depth > 0){
        
        memset(raw_data, 0, sizeof(raw_data));

        for(i = 0; i < data_depth; i++){
            /* Get data from FIFO. */
            ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_FIFO_DATA_OUT_L, tmp_data, 2);
            if(ret != 0){
                printf("Error: Lsm6d3s_ReadBytes failed.(%02x)\n", ret);
            }
            raw_data[i] = tmp_data[1]<<8 | tmp_data[0];
        }

        printf("Info: Data from FIFO:\n");
        for(i = 0; i < data_depth/6; i++){
            printf("Info:ACC: ");
            for(j = 0; j < 3; j++){
                //printf("%f ", (float)raw_data[6*i + j]*100/1638);
                printf("%f ", (float)raw_data[6*i + j]*0.061/1000);
            }
            printf("\n");

            printf("Info:GYR: ");
            for(j = 0; j < 3; j++){
                //printf("%f ", (float)raw_data[6*i + 3 + j]*100/6104);
                printf("%f ", (float)raw_data[6*i + 3 + j]*70/1000);
            }
            printf("\n");
        }
    }

}

void Lsm6ds3_GetRawDataFromFIFO(void)
{
    Lsm6ds3_GetFIFOStatus();
}

void Lsm6ds3_GetSingleRawData(void)
{
    unsigned char ret;
    unsigned char buf;
    unsigned char raw_data[6];
    short temp_data;
    unsigned char i;
    
    axis_database_t *sensor_raw_data;
    sensor_raw_data = (axis_database_t *)malloc(sizeof(axis_database_t));
    if(sensor_raw_data == NULL){
        printf("Memory allocate for sensor_raw_data failed.\n");
    }
#if 0
    printf("Info: Check sensor update--->\n");
#endif
    ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_STATUS_REG, &buf, 1);
    if(ret != 0){
        printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
    }
#if 0
    printf("STATUS_REG = 0x%02X\n", buf);
#endif
    if(buf&0x01 != 0){  /* Accelerometer new data available. */
        memset(raw_data, 0, sizeof(raw_data));
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUTX_L_XL, raw_data, 6);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        //printf("Accelerometer data:");
        printf("Info:ACC: ");
#if 0
        for(i = 0; i < 6; i++){
            printf("%d ", raw_data[i]);
        }
        printf("\n");
#endif
        for(i = 0; i < AXIS_NUM; i++){
            temp_data = 0;
            temp_data = (raw_data[2*i+1] << 8)|raw_data[2*i];
            sensor_raw_data->accelerometer.axis[i] = (float)temp_data*0.061/1000;
            //sensor_raw_data->accelerometer.axis[i] = (float)temp_data*100/1638;
        }
        for(i = 0; i < AXIS_NUM; i++){
            printf("%f\t", sensor_raw_data->accelerometer.axis[i]);
        }
#if 0
        printf("%f\n", sqrt(pow(sensor_raw_data->accelerometer.axis[0],2) + \
                            pow(sensor_raw_data->accelerometer.axis[1],2) + \
                            pow(sensor_raw_data->accelerometer.axis[2],2)));
#else
        printf("\n");
#endif
    }
    
    if(buf&0x02 != 0){  /* Gyroscope new data available. */
        memset(raw_data, 0, sizeof(raw_data));
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUTX_L_G, raw_data, 6);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Info:GYR: ");
#if 0
        for(i = 0; i < 6; i++){
            printf("%d ", raw_data[i]);
        }
        printf("\n");
#endif
        for(i = 0; i < AXIS_NUM; i++){
            temp_data = 0;
            temp_data = (raw_data[2*i+1] << 8)|raw_data[2*i];
            sensor_raw_data->gyroscope.axis[i] = (float)temp_data*70/1000;
            //sensor_raw_data->gyroscope.axis[i] = (float)temp_data*100/6104;
        }
        for(i = 0; i < AXIS_NUM; i++){
            printf("%f\t", sensor_raw_data->gyroscope.axis[i]);
        }
        printf("\n");
    }

    if(buf&0x04 != 0){  /* Temperature new data available. */
        memset(raw_data, 0, sizeof(raw_data));
        ret = Lsm6ds3_ReadBytes(LSM6DS3_ACC_GYRO_OUT_TEMP_L, raw_data, 2);
        if(ret != 0){
            printf("Error: Lsm6d3s_ReadByte failed.(%02x)\n", ret);
        }
        printf("Info:TEM: ");
#if 0
        for(i = 0; i < 2; i++){
            printf("0x%02X ", raw_data[i]);
        }
        printf("\n");
#endif
        temp_data = (raw_data[1] << 8)|raw_data[0];
        sensor_raw_data->temperature.temperature = (float)temp_data/256 + 25.0;
        printf("%f", sensor_raw_data->temperature.temperature);
        printf("\n");
    }
    free(sensor_raw_data);
}










/* End of this file. */
#include <stdio.h>
#include <string.h>
#include <bcm2835.h>
#include <stdlib.h>
#include <math.h>
#include "ADXL345.h"

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

unsigned char Single_Read_ADXL345(unsigned char REG_Address)
{
    unsigned char WriteBuf[1];
    unsigned char ReadBuf[1];
    int rc;
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(ADXL345_Addr);
    bcm2835_i2c_set_baudrate(ADXL345_Drate);
    WriteBuf[0] = REG_Address;
    bcm2835_i2c_write( WriteBuf, 1);
    rc = bcm2835_i2c_read(ReadBuf, 1);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();
    return ReadBuf[0]; 
}

int Multipul_Read(unsigned char regaddr, unsigned char *buf, unsigned char len)
{
    unsigned char WriteBuf[1];
    int rc;
    if (!bcm2835_init())
    return 1;
    WriteBuf[0] = regaddr;
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(ADXL345_Addr);
    bcm2835_i2c_set_baudrate(ADXL345_Drate);
    bcm2835_i2c_write( WriteBuf, 1);
    
    rc = bcm2835_i2c_read(buf, len);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}
#if 0
int Multiple_read_ADXL345(void)
{
    unsigned char WriteBuf[1] = {0x32};
    unsigned char ReadBuf[6];
    short int raw_data[3];
    float result,single_dir[3];
    unsigned char i;
    int rc;
    memset(ReadBuf,0,sizeof(ReadBuf));
    memset(raw_data,0,sizeof(raw_data));
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(ADXL345_Addr);
    bcm2835_i2c_set_baudrate(ADXL345_Drate);
    bcm2835_i2c_write( WriteBuf, 1);
    rc = bcm2835_i2c_read(ReadBuf, 6);
    if(rc != BCM2835_I2C_REASON_OK ){
        printf("Read failed.(%d)\n",rc);
    }
    bcm2835_i2c_end();
    bcm2835_close();
#if 0
    for(i = 0;i<6;i++){
        printf("ReadBuf[%d] = %d\n",i,ReadBuf[i]);
    }
#endif
    for(i = 0;i<3;i++){
        raw_data[i] = (ReadBuf[2*i+1]<<8) + ReadBuf[2*i];
#if 0
        if(raw_data[i]<0){
            printf("<0\n");
            //raw_data[i]=-raw_data[i];
        }
#endif
        single_dir[i] = (float)raw_data[i]*3.9;
        //printf("raw_data[%d] = %d, single_dir[%d] = %f\n",i,raw_data[i],i,single_dir[i]);
    }
    result = (sqrt(pow(single_dir[0],2)+pow(single_dir[1],2)+pow(single_dir[2],2)) - 1000)/1000;
    printf("result = %fm/s\n",result);
    return 0;
}
#endif
int Single_Write_ADXL345(unsigned char REG_Address,unsigned char REG_data)
{
    unsigned char WriteBuf[2];
    unsigned char ReadBuf[1];
    if (!bcm2835_init())
    return 1;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(ADXL345_Addr);
    bcm2835_i2c_set_baudrate(ADXL345_Drate);

    WriteBuf[0] = REG_Address;
    WriteBuf[1] = REG_data;
    bcm2835_i2c_write( WriteBuf, 2);
    
    bcm2835_i2c_end();
    bcm2835_close();
    return 0;
}

void Init_ADXL345(void)
{
    unsigned char ret;
    Single_Write_ADXL345(0x31,0x08);    // 测量范围,正负16g，13位模式
    Single_Write_ADXL345(0x2C,0x0A);    // 速率设定为12.5 参考pdf13页
    Single_Write_ADXL345(0x2D,0x08);    // 选择电源模式   参考pdf24页
    Single_Write_ADXL345(0x2E,0x80);    // 使能 DATA_READY 中断
    Single_Write_ADXL345(0x1E,0x00);    // X 偏移量 根据测试传感器的状态写入pdf29页
    Single_Write_ADXL345(0x1F,0x00);    // Y 偏移量 根据测试传感器的状态写入pdf29页
    Single_Write_ADXL345(0x20,0x05);    // Z 偏移量 根据测试传感器的状态写入pdf29页
    Single_Write_ADXL345(0x38,0x98);    // 0x38->10 0 11111
    ret = Single_Read_ADXL345(0x00);    // Require Chip's ID.
    printf("ret = 0x%02X\n",ret);
    if(ret!=0xE5){
       printf("Init_ADXL345 error.\n");
    }
}

void Read_ADXL345_FIFO(void)
{
    unsigned char d_depth;
    int rc;
    unsigned char i, j, k = 0;
#if 1
    unsigned char *ReadBuf;
    short int *raw_data;
    float *single_dir;
    float *result;
#else
    unsigned char ReadBuf[32*6];
    short int raw_data[32*3];
    float single_dir[32*3];
    float result[32];
#endif

    printf("Get data:");
    d_depth = Single_Read_ADXL345(0x39);                            // Get FIFO status.
    printf("FIFO STATUS = 0x%02X\n",d_depth);
#if 1
    ReadBuf = (unsigned char *)malloc(sizeof(unsigned char)*d_depth*fifo_width);
    if(ReadBuf == NULL){
        printf("malloc error.\n");
    }
    raw_data = (short int *)malloc(sizeof(short int)*d_depth*3);
    if(raw_data == NULL){
        printf("malloc error.\n");
    }
    single_dir = (float *)malloc(sizeof(float)*d_depth*3);
    if(single_dir == NULL){
        printf("malloc error.\n");
    }
    printf("result addr = 0x%08x\n",result);
    result = (float *)malloc(sizeof(float)*d_depth);
    if(result == NULL){
        printf("malloc error.\n");
    }
    printf("result addr = 0x%08x\n",result);
#endif
    memset(ReadBuf,0,d_depth*fifo_width);
#if 1
    memset(raw_data,0,d_depth*3);
    memset(single_dir,0,d_depth*3);
    memset(result,0,d_depth);
#endif
    for(i = 0; i < d_depth; i++){
        rc = Multipul_Read(0x32, ReadBuf+i*fifo_width, fifo_width); // Get data from fifo.
        if(rc != 0 ){
            printf("Read failed.(%d)\n",rc);
        }
    }

    for(i = 0; i < d_depth; i++){
        for(j = 0; j < fifo_width; j++){
            printf("%d ",ReadBuf[k++]);
        }
#if 1
        raw_data[3*i] = (ReadBuf[6*i+1]<<8) + ReadBuf[6*i];
        raw_data[3*i+1] = (ReadBuf[6*i+3]<<8) + ReadBuf[6*i+2];
        raw_data[3*i+2] = (ReadBuf[6*i+5]<<8) + ReadBuf[6*i+4];

        single_dir[3*i] = (float)raw_data[3*i]*3.9;
        single_dir[3*i+1] = (float)raw_data[3*i+1]*3.9;
        single_dir[3*i+2] = (float)raw_data[3*i+2]*3.9;

        result[i] = (sqrt(pow(single_dir[3*i],2) + pow(single_dir[3*i+1],2) + pow(single_dir[3*i+2],2)) - 1000)/1000;
#endif
        printf("===> %02d: %f\n", i, result[i]);
    }
    printf("result addr = 0x%08x\n",result);
#if 1
    free(result);
    free(single_dir);
    free(raw_data);
    free(ReadBuf);
#endif
}


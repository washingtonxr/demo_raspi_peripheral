#include "adxl345.h"

void Init_ADXL345(void)
{
    unsigned char ret;
    I2C_Single_Write(ADXL345_Addr, 0x31, 0x08);    // 测量范围,正负16g，13位模式
    I2C_Single_Write(ADXL345_Addr, 0x2C, 0x0A);    // 速率设定为12.5 参考pdf13页
    I2C_Single_Write(ADXL345_Addr, 0x2D, 0x08);    // 选择电源模式   参考pdf24页
    I2C_Single_Write(ADXL345_Addr, 0x2E, 0x80);    // 使能 DATA_READY 中断
    I2C_Single_Write(ADXL345_Addr, 0x1E, 0x00);    // X 偏移量 根据测试传感器的状态写入pdf29页
    I2C_Single_Write(ADXL345_Addr, 0x1F, 0x00);    // Y 偏移量 根据测试传感器的状态写入pdf29页
    I2C_Single_Write(ADXL345_Addr, 0x20, 0x05);    // Z 偏移量 根据测试传感器的状态写入pdf29页
    I2C_Single_Write(ADXL345_Addr, 0x38, 0x98);    // 0x38->10 0 11111
    ret = I2C_Single_Read(ADXL345_Addr, 0x00);    // Require Chip's ID.
    printf("ret = 0x%02X\n", ret);
    if(ret != 0xE5){
       printf("Init_ADXL345 error.\n");
    }
}

unsigned char Read_ADXL345_OneData(sensor_data_t *SDB)
{
    unsigned char d_depth;
    int ret;
    unsigned char i, j, k = 0;
    unsigned char *ReadBuf;
    short int *raw_data;
    float *single_dir;
    float *result;

    printf("Infor: Get acceleratometer datalength: ");
    d_depth = I2C_Single_Read(ADXL345_Addr, 0x39);                            // Get FIFO status.
    printf("FIFO STATUS = 0x%02X\n",d_depth);

    /* Temporary memory allocate. */
    ReadBuf = (unsigned char *)malloc(sizeof(unsigned char)*d_depth*fifo_width);
    if(ReadBuf == NULL){
        printf("Error: Memory allocate failed.\n");
    }
    raw_data = (short int *)malloc(sizeof(short int)*d_depth*3);
    if(raw_data == NULL){
        printf("Error: Memory allocate failed.\n");
    }
    single_dir = (float *)malloc(sizeof(float)*d_depth*3);
    if(single_dir == NULL){
        printf("Error: Memory allocate failed.\n");
    }
    
    for(i = 0; i < d_depth; i++){
        ret = I2C_Multipul_Read(ADXL345_Addr, 0x32, ReadBuf + (i*fifo_width), fifo_width); // Get data from fifo.
        if(ret != 0 ){
            printf("Read failed.(%d)\n", ret);
        }
    }
    for(i = 0; i < d_depth; i++){
#if 0
        for(j = 0; j < fifo_width; j++){
            printf("%d ", ReadBuf[k++]);
        }
#endif
        raw_data[3*i] = (ReadBuf[6*i+1]<<8) + ReadBuf[6*i];
        raw_data[3*i+1] = (ReadBuf[6*i+3]<<8) + ReadBuf[6*i+2];
        raw_data[3*i+2] = (ReadBuf[6*i+5]<<8) + ReadBuf[6*i+4];

        single_dir[3*i] = (float)raw_data[3*i]*3.9;
        single_dir[3*i+1] = (float)raw_data[3*i+1]*3.9;
        single_dir[3*i+2] = (float)raw_data[3*i+2]*3.9;
    }

    /* Latch data from temporary buffer. (ADXL345_DNUM)*/
    SDB->acc_data.x = single_dir[3*ADXL345_DNUM];
    SDB->acc_data.y = single_dir[3*ADXL345_DNUM + 1];
    SDB->acc_data.z = single_dir[3*ADXL345_DNUM + 2];

    /* Release memory. */
    free(single_dir);
    free(raw_data);
    free(ReadBuf);

    return RUN_OK;
}

void Read_ADXL345_FIFO(void)
{
    unsigned char d_depth;
    int ret;
    unsigned char i, j, k = 0;
    unsigned char *ReadBuf;
    short int *raw_data;
    float *single_dir;
    float *result;

    printf("Infor: Get acceleratometer datalength: ");
    d_depth = I2C_Single_Read(ADXL345_Addr, 0x39);                            // Get FIFO status.
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
        ret = I2C_Multipul_Read(ADXL345_Addr, 0x32, ReadBuf + (i*fifo_width), fifo_width); // Get data from fifo.
        if(ret != 0 ){
            printf("Read failed.(%d)\n", ret);
        }
    }
    for(i = 0; i < d_depth; i++){
        for(j = 0; j < fifo_width; j++){
            printf("%d ", ReadBuf[k++]);
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


/* End of the file. */
#ifndef ADXL345_H
#define ADXL345_H
#include "common.h"

#define ADXL345_Addr    0x53
#define ADXL345_DNUM    1

void Init_ADXL345();
void Read_ADXL345_FIFO();
unsigned char Read_ADXL345_OneData(sensor_data_t *SDB);

#endif
/* End of the file. */
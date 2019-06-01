#ifndef HMC5883L_H
#define HMC5883L_H
#include "common.h"

#define HMC5883L_Addr    0x1E

void Init_HMC5883();
void Read_HMC5883_FIFO();
unsigned char Read_HMC5883L_OneData(sensor_data_t *SDB);
#endif
/* End of the file. */
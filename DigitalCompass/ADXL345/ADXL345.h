#ifndef ADXL345_H
#define ADXL345_H


#define ADXL345_Addr    0x53
#define ADXL345_Drate   400000
#define fifo_width      6

void Init_ADXL345();
int Multiple_read_ADXL345();
void Read_ADXL345_FIFO();



#endif

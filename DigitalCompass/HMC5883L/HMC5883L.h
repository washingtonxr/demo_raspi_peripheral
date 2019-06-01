#ifndef HMC5883L_H
#define HMC5883L_H


#define HMC5883L_Addr    0x1E
#define HMC5883L_Drate   400000
#define fifo_width      6


void Init_HMC5883();
void Read_HMC5883_FIFO();



#endif

#ifndef COMMON_H
#define COMMON_H

#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define I2Cdatarate     400000
#define fifo_width      6

#define RUN_OK          0
#define RUN_FAIL        1

unsigned char Start_bcm2835(void);
unsigned char Stop_bcm2835(void);

unsigned char I2C_Multipul_Read(unsigned char addr, unsigned char regaddr, 
                                unsigned char *buf, unsigned char len);
unsigned char I2C_Single_Write(unsigned char addr, unsigned char regaddr,
                                unsigned char data);
unsigned char I2C_Single_Read(unsigned char addr, unsigned char regaddr);

#endif
/* End of the file. */
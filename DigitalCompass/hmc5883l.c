#include "hmc5883l.h"
#include "common.h"

void Init_HMC5883(void)
{
    I2C_Single_Write(HMC5883L_Addr, 0x02, 0x00);
}

void Read_HMC5883_FIFO(void)
{
    unsigned char ReadBuf[fifo_width];
    unsigned char i;
    short int x, y, z;
    double angle[3];
    
    I2C_Multipul_Read(HMC5883L_Addr, 0x03, ReadBuf, fifo_width);
    
    for(i = 0; i < fifo_width; i++){        
        printf("ReadBuf[%d] = %d\n",i,ReadBuf[i]);
    }
    
    x = (ReadBuf[0]<<8) + ReadBuf[1];
    z = (ReadBuf[2]<<8) + ReadBuf[3];
    y = (ReadBuf[4]<<8) + ReadBuf[5];
    
    angle[0] = atan2((double)y,(double)x) * (180 / 3.14159265) + 180; 
    //angle[0] *=10;
    angle[1] = atan2((double)z,(double)y) * (180 / 3.14159265) + 180; 
    //angle[1] *=10;
    angle[2] = atan2((double)x,(double)z) * (180 / 3.14159265) + 180; 
    //angle[2] *=10;
    
    printf("x = %d, y=%d, z=%d, a1=%lf, a2=%lf, a3=%lf\n",x,y,z,angle[0],angle[1],angle[2]);
}

/* End of the file. */
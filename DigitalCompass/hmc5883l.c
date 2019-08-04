#include "hmc5883l.h"

static uint8_t HMC5883Lmode;
/** Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see HMC5883L_GetGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
void HMC5883L_SetGain(uint8_t gain)
{
    // use this method to guarantee that bits 4-0 are set to zero, which is a
    // requirement specified in the datasheet; 
    uint8_t tmp = gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1);
    I2C_Single_Write(HMC5883L_DEFAULT_ADDRESS, tmp, HMC5883L_RA_CONFIG_B);
}

/** Set measurement mode.
 * @param newMode New measurement mode
 * @see HMC5883L_GetMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
void HMC5883L_SetMode(uint8_t newMode)
{
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet; 
    uint8_t tmp = HMC5883Lmode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1);
    I2C_Single_Write(HMC5883L_DEFAULT_ADDRESS, tmp, HMC5883L_RA_MODE);
    HMC5883Lmode = newMode; // track to tell if we have to clear bit 7 after a read
}

void Init_HMC5883(void)
{
#if 0
    I2C_Single_Write(HMC5883L_Addr, 0x02, 0x00);
#else
    /* write CONFIG_A register. */
    uint8_t tmp = (HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1))
            | (HMC5883L_RATE_15 << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1))
            | (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1));
    I2C_Single_Write(HMC5883L_DEFAULT_ADDRESS, tmp, HMC5883L_RA_CONFIG_A);

    /* write CONFIG_B register. */
    HMC5883L_SetGain(HMC5883L_GAIN_1090);

    /* write MODE register. */
    HMC5883L_SetMode(HMC5883L_MODE_SINGLE);
#endif
}

unsigned char Read_HMC5883L_OneData(sensor_data_t *SDB)
{
    unsigned char ReadBuf[fifo_width];
    unsigned char i;
    //double angle[3];

    I2C_Multipul_Read(HMC5883L_Addr, 0x03, ReadBuf, fifo_width);

    SDB->mag_data.x = (ReadBuf[0]<<8) + ReadBuf[1];
    SDB->mag_data.z = (ReadBuf[2]<<8) + ReadBuf[3];
    SDB->mag_data.y = (ReadBuf[4]<<8) + ReadBuf[5];
#if 0
    angle[0] = atan2((double)y,(double)x) * (180 / 3.14159265) + 180; 
    angle[1] = atan2((double)z,(double)y) * (180 / 3.14159265) + 180; 
    angle[2] = atan2((double)x,(double)z) * (180 / 3.14159265) + 180; 
#endif
    return RUN_OK;
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
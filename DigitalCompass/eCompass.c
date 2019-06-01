#include "eCompass.h"
#include "common.h"
#include "adxl345.h"
#include "hmc5883l.h"

unsigned int sys_cnt;
int main(unsigned char argc, unsigned char **argv)
{
	int16_t mag_x, mag_y, mag_z;
	int16_t acc_x, acc_y, acc_z;
	double roll;
	double pitch;
	double azimuth;
	double X_h, Y_h;
	int8_t x, y;
	int8_t x_old = 0, y_old = 0;
    
    if(Start_bcm2835() != RUN_OK){
        printf("Start_bcm2835 initializing failed.\n");
        return RUN_FAIL;
    }
    
    Init_ADXL345();
    Init_HMC5883();

    printf("Hello eCompass.\n");
    while(1){
        printf("Info: %d\n", sys_cnt++);
        Read_HMC5883_FIFO();
        Read_ADXL345_FIFO();
        sleep(1);
    }
    
    if(Stop_bcm2835() != RUN_OK){
        printf("Stop_bcm2835 stop failed.\n");
        return RUN_FAIL;
    }
    return RUN_OK;
}

/* End of the file. */
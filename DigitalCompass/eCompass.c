#include "eCompass.h"
#include "common.h"
#include "adxl345.h"
#include "hmc5883l.h"

unsigned int sys_cnt;
int main(unsigned char argc, unsigned char **argv)
{
	sensor_data_t *SensorData;

    /* Initialize bcm2835. */
    if(Start_bcm2835() != RUN_OK){
        printf("Error: Start_bcm2835 initializing failed.\n");
        return RUN_FAIL;
    }

    /* Memory allocate for data. */
    SensorData = (sensor_data_t *)malloc(sizeof(sensor_data_t));
    if(SensorData == NULL){
        printf("Error: Memory allocate failed.\n");
        return RUN_FAIL;
    }
    memset(SensorData, 0, sizeof(sensor_data_t));

    /* Sensor initializing process. */
    Init_ADXL345();
    Init_HMC5883();

    printf("Infor: Hello eCompass.\n");

    /* Sensor test. */
    Read_HMC5883_FIFO();
    Read_ADXL345_FIFO();
    sleep(1);
    
    while(1){
        printf("Info: %d\n", sys_cnt++);

        Read_ADXL345_OneData(SensorData);
        printf("Infor: ACC: %f\t%f\t%f\n", SensorData->acc_data.x, SensorData->acc_data.y, SensorData->acc_data.z);
 
        Read_HMC5883L_OneData(SensorData);

		/* Calculate pitch and roll, in the range (-pi,pi) */
		SensorData->mag_data.pitch = atan2((double)-SensorData->acc_data.x, sqrt(pow((long)SensorData->acc_data.z, 2) \
                                                                            + pow((long)SensorData->acc_data.y, 2)));
		SensorData->mag_data.roll = atan2((double)SensorData->acc_data.y, sqrt(pow((long)SensorData->acc_data.z, 2) \
                                                                            + pow((long)SensorData->acc_data.x, 2)));
        printf("Infor: pitch:%f, roll:%f\n", SensorData->mag_data.pitch, SensorData->mag_data.roll);

		/* Calculate Azimuth:
		 * Magnetic horizontal components, after compensating for Roll(r) and Pitch(p) are:
		 * X_h = X*cos(p) + Y*sin(r)*sin(p) + Z*cos(r)*sin(p)
		 * Y_h = Y*cos(r) - Z*sin(r)
		 * Azimuth = arcTan(Y_h/X_h)
		 */
		SensorData->mag_data.X_h = (double)SensorData->mag_data.x*cos(SensorData->mag_data.pitch) \
                        + (double)SensorData->mag_data.y*sin(SensorData->mag_data.roll)*sin(SensorData->mag_data.pitch) \
                        + (double)SensorData->mag_data.z*cos(SensorData->mag_data.roll)*sin(SensorData->mag_data.pitch);
        
		SensorData->mag_data.Y_h = (double)SensorData->mag_data.y*cos(SensorData->mag_data.roll) \
                        - (double)SensorData->mag_data.z*sin(SensorData->mag_data.roll);
        printf("Infor: X_h:%f\tX_h:%f\n", SensorData->mag_data.X_h, SensorData->mag_data.Y_h);

		SensorData->mag_data.azimuth = atan2(SensorData->mag_data.Y_h, SensorData->mag_data.X_h);
		if(SensorData->mag_data.azimuth < 0) {	/* Convert Azimuth in the range (0, 2pi) */
			SensorData->mag_data.azimuth = 2*PI + SensorData->mag_data.azimuth;
		}

        printf("Infor: azimuth(w):%f -> %f\n", SensorData->mag_data.azimuth, (SensorData->mag_data.azimuth*180)/PI);
        sleep(1);
    }

    /* Deinit bcm2835 module. */
    if(Stop_bcm2835() != RUN_OK){
        printf("Error: Stop_bcm2835 stop failed.\n");
        return RUN_FAIL;
    }
    return RUN_OK;
}

/* End of the file. */
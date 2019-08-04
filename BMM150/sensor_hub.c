#include "sensor_hub.h"

#include "bmm150_defs.h"
#include "bmm150.h"

static struct bmm150_dev bmm150dev;

int8_t BMM150_read_sensor_data(struct bmm150_dev *dev)
{
	int8_t rslt;

	/* Mag data for X,Y,Z axis are stored inside the
	bmm150_dev structure in float format */
	rslt = bmm150_read_mag_data(dev);

	/* Print the Mag data */
	printf("Info: Magnetometer data:\n");
	printf("Info: MAG X : %0.2f \t MAG Y : %0.2f \t MAG Z : %0.2f \n"
		,dev->data.x, dev->data.y, dev->data.z);
	
	return rslt;
}

static int8_t BMM150_perform_self_tests(struct bmm150_dev *dev)
{
	int8_t rslt;

	/* Perform Normal Self test */
	rslt = bmm150_perform_self_test(BMM150_NORMAL_SELF_TEST, dev);
	printf("Info: BMM150NORMAL SELF TEST RESULT :  %d",rslt);
	
	/* Validate normal self test result */
	if (rslt == BMM150_OK) {
		printf("Info: Normal Self test passed \n");
	} else {
		printf("Error: Normal Self test failed \n");
	}
	/* Perform Advanced Self test */
	rslt |= bmm150_perform_self_test(BMM150_ADVANCED_SELF_TEST, dev);
	printf("Info: ADVANCED SELF TEST RESULT : %d\n",rslt);

	/* Validate Advanced self test result */
	if (rslt == BMM150_OK) {
		printf("Info: Advanced Self test passed \n");
	} else {
		printf("Error: Advanced Self test failed \n");
	}
	
	return rslt;
}

int8_t BMM150_Init(void)
{
    int8_t rslt            = BMM150_OK;
    
    DebugPrintf("Info: BBM150 initializing.\n");
        
    bmm150dev.dev_id        = BMM150_DEFAULT_I2C_ADDRESS;
    bmm150dev.intf          = BMM150_I2C_INTF;
    bmm150dev.read          = (bmm150_com_fptr_t)I2C_1_ReadBytes;
    bmm150dev.write         = (bmm150_com_fptr_t)I2C_1_WriteBytes;
    bmm150dev.delay_ms      = vTaskDelay;
    
    /* Initialize BNI160 sensor */
    rslt = bmm150_init(&bmm150dev);
    if(rslt != BMM150_OK){
        printf("Error: BMM150 initializing failed(%d).\n", rslt);
        return rslt;
    }
    
    /* Perform self test. */
    rslt = BMM150_perform_self_tests(&bmm150dev);
    if(rslt != BMM150_OK){
        printf("Error: BMM150 perform self test failed(%d).\n", rslt);
        return rslt;
    }

    /* Configure to normal mode. */
#if 0
    if(rslt == BMM150_OK) /* BMM150_OK initialization successful */
#endif
    {
        /* Setting the power mode as normal */
        bmm150dev.settings.pwr_mode = BMM150_NORMAL_MODE;
        rslt = bmm150_set_op_mode(&bmm150dev);

        /* Setting the preset mode as Low power mode 
        i.e. data rate = 10Hz XY-rep = 1 Z-rep = 2*/
        bmm150dev.settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
        rslt = bmm150_set_presetmode(&bmm150dev);
    }
    
    printf("Info: BBM150 configuration result(%d).\n", rslt);
    return rslt;
}

void BMM150_data_print(void)
{
    BMM150_read_sensor_data(&bmm150dev);
}






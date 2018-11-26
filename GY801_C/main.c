#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BMP085.h"


int main(unsigned char argc, unsigned char **argv)
{
    float temperature;
    long presure;
    float altitude;
    printf("Hello world.\n");
    Init_BMP085();
    temperature = bmp085ReadTemp();
    presure = bmp085ReadPressure();
    altitude = bmp180_Altitude();
    bmp180_Altitude();
    printf("Temperature = %f, Presure = %lu.\n",temperature,presure);
    printf("Altitude = %f\n",altitude);
    printf("The end.\n");
    
    return 0;
}


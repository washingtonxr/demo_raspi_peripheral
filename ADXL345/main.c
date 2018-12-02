#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "ADXL345.h"


int main(unsigned char argc, unsigned char **argv)
{
    unsigned int i;
    
    printf("Hello world.\n");
    Init_ADXL345();

    while(1){
        printf("Counter = %d\n",i++);
        Read_ADXL345_FIFO();
        sleep(1);
    }

    printf("The end.\n");
    
    return 1;
}


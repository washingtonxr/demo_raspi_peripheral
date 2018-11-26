#include <stdio.h>
#include <string.h>
#include <math.h>
#include "KX022.h"


int main(unsigned char argc, unsigned char **argv)
{
    unsigned char data_length;
    unsigned char xyz_counts;
    unsigned int read_counter;
    
    printf("Hello world.\n");

    Start_bcm2835();
    Init_KX022();

    while(1){
        printf("Counter = %d\n",read_counter++);
        kx022_read_regs(SlaveAddress, &data_lenght, 1);
        if(data_lenght >150)
            data_lenght = 150;


        sleep(1);
    }
    Stop_bcm2835();
    printf("No support to be here.\n");
    printf("The end.\n");
    
    return 0;
}


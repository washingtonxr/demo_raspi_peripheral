#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "KX022.h"
/* This is a test for KX022. */

int main(unsigned char argc, unsigned char **argv)
{
    unsigned char data_length;
    unsigned char xyz_counts;
    unsigned int read_counter;
    unsigned char a,b;
    unsigned char sensor_buf[buf_len];
    unsigned char clear = 0;

    printf("Hello world.\n");

    Start_bcm2835();
    Init_KX022();

    while(1){
        //printf("Counter=%d;",read_counter++);
        kx022_read_regs(SlaveAddress, &data_length, 1);

        if(data_length >150){
            data_length = 150;
        }
        
        a = data_length / 30;
        b = data_length %30;
#if 0
        printf("a = %d, b = %d.\n", a, b);
#endif
        for(unsigned int i = 0; i <a; i++){
            kx022_read_regs(KX022_DATA_reg, &sensor_buf[i*30], 30);
        }
        if(b != 0)
            kx022_read_regs(KX022_DATA_reg, &sensor_buf[a*30], b);
#if 0
        printf("Data:\n");
        for(unsigned int i = 0; i <(a*30 + b ); i++){
            printf("%02x ",sensor_buf[i]);
        }
        printf("\n");
#endif
        kx022_read_regs(0x3e, &clear, 1);
        
        xyz_counts = db2axis(sensor_buf, data_length);
#if 0
        printf("xyz_counts = %02x\n",xyz_counts);
#endif
        usleep(500000);
    }
    
    Stop_bcm2835();
    printf("No support to be here.\n");
    printf("The end.\n");
    
    return 0;
}


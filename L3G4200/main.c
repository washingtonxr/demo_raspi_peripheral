#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "L3G4200.h"


int main(unsigned char argc, unsigned char **argv)
{
    unsigned int i;
    
    printf("Hello world.\n");
    Init_L3G4200D();

    while(1){
        printf("Counter = %d\n",i++);
        READ_L3G4200D();
        sleep(1);
    }

    printf("The end.\n");
    
    return 1;
}


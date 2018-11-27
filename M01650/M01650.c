#include <stdio.h>
#include <string.h>
#include <math.h>
#include <bcm2835.h>
#include <unistd.h>
#include "M01650.h"

static void Reset_bcm(void)
{
    /* Set as Ouput */
    bcm2835_gpio_fsel(RESET_PIN, BCM2835_GPIO_FSEL_OUTP);
    
    bcm2835_gpio_write(RESET_PIN, LOW);
    bcm2835_delay(1);     /* delay for ms. */
    
    bcm2835_gpio_write(RESET_PIN, HIGH);
    bcm2835_delay(1);
    
    return;
}

void Start_bcm2835(void)
{
    bcm2835_init();
    return;
}

void Stop_bcm2835(void){
    bcm2835_close();
    return;
}

static int Single_Write(unsigned char Reg, unsigned char Com)
{
    unsigned char WriteBuf[2] = {0x00,0x00};
    int rc;
    
    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(SlaveAddress);
    bcm2835_i2c_set_baudrate(Boundrate);

    WriteBuf[0] = Reg;
    WriteBuf[1] = Com;
    rc = bcm2835_i2c_write( WriteBuf, 2);
    if(!rc == 0){
        printf("Write Command failed.\n");
        return rc;
    }
    bcm2835_i2c_end();

    return 0;
}

void Clean_Screen(void)
{
    unsigned char page_number, column_number;
    for(page_number = 0; page_number < Page_Full_No; page_number++)   
    {
        Single_Write(Config_Addr, 0xB0 + page_number);
        Single_Write(Config_Addr, 0x00);
        Single_Write(Config_Addr, 0x10);
        for(column_number = 0; column_number < Column_Full_No; column_number++){
            Single_Write(Data_Addr, 0x00);
        }
    }
}

int Write_Sceen(unsigned char *data)
{
    unsigned char page_number,column_number;
    unsigned int i = 0;
    for(page_number = 0; page_number < Page_Full_No; page_number++)   
    {
        Single_Write(Config_Addr, 0xB0 + page_number);  /* Set Page Address. */
        Single_Write(Config_Addr, 0x00);                /* Set Column Address. */
        Single_Write(Config_Addr, 0x10);                /* Left start. */
       
        for(column_number = 0; column_number < Column_Full_No; column_number++)
        {
            Single_Write(Data_Addr, data[i++]);// 
            //printf("i=%d\n",i);
            //usleep(10000);
        }
    }
    Single_Write(Config_Addr, 0xaf); //Display ON
    return 0;
}

void Init_M01650(void)
{
    Reset_bcm();
    printf("Starting Initializing M01650.\n");
    Single_Write(Config_Addr, 0xae); // Display OFF
    Single_Write(Config_Addr, 0xa8); // Set Multiplex Ration
    Single_Write(Config_Addr, 0x3f);
    Single_Write(Config_Addr, 0xd5); // Set Frame Frequency
    Single_Write(Config_Addr, 0xf1); // 104Hz
    Single_Write(Config_Addr, 0xc0); // Set Common scan direction
    Single_Write(Config_Addr, 0xd3); // Set Display Offset
    Single_Write(Config_Addr, 0x60);
    Single_Write(Config_Addr, 0xdc); // Set Display Start Line
    Single_Write(Config_Addr, 0x00);
    Single_Write(Config_Addr, 0x20); // Set Page Addressing Mode
    Single_Write(Config_Addr, 0x81); // Set Contrast Control
    Single_Write(Config_Addr, 0x90);
    Single_Write(Config_Addr, 0xa0); // Set Segment Re-map
    Single_Write(Config_Addr, 0xa4); // Set Entire Display OFF/ON
    Single_Write(Config_Addr, 0xa6); // Set Normal/Reverse Display
    Single_Write(Config_Addr, 0xad); // Set External VPP
    Single_Write(Config_Addr, 0x8a);
    Single_Write(Config_Addr, 0xd9); // Set Phase Leghth
    Single_Write(Config_Addr, 0x22);
    Single_Write(Config_Addr, 0xdb); // Set Vcomh voltage
    Single_Write(Config_Addr, 0x35);
    Clean_Screen();
    Single_Write(Config_Addr, 0xaf); // Display ON

    printf("End of Initializing M01650.\n");
    return;
}















// blink.c
//
// Example program for bcm2835 library
// Blinks a pin on an off every 0.5 secs
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o blink blink.c -l bcm2835
// sudo ./blink
//
// Or you can test it before installing with:
// gcc -o blink -I ../../src ../../src/bcm2835.c blink.c
// sudo ./blink
//
// Author: Mike McCauley
// Copyright (C) 2011 Mike McCauley
// $Id: RF22.h,v 1.21 2012/05/30 01:51:25 mikem Exp $
#include <bcm2835.h>
#include <stdio.h>

// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN_BB RPI_V2_GPIO_P1_40
#define PIN_EN RPI_V2_GPIO_P1_38

static unsigned int ulf_transmit_id[2] = {0xff824006, 0x18C350BC};

void delayus(unsigned int time){
    unsigned int i;
    for(i = 0; i < (time*168); i++){
        ;
    }
}

static unsigned char ULF_BO_BIT;
static unsigned int ULF_BO_BYTE;
static unsigned int ULF_Recv_Num;
void ULF_BB()
{
    unsigned char i,j;
    ULF_BO_BYTE = ulf_transmit_id[0];
    for(i = 0; i < 32; i++){
        ULF_BO_BIT = (ULF_BO_BYTE >> 31) & 0x1;
        if(ULF_BO_BIT){
            bcm2835_gpio_write(PIN_BB, HIGH);
        }else{
            bcm2835_gpio_write(PIN_BB, LOW);
        }
        delayus(200);
        if(ULF_BO_BIT){
            bcm2835_gpio_write(PIN_BB, LOW);
        }else{
            bcm2835_gpio_write(PIN_BB, HIGH);
        }
        delayus(200);
        ULF_BO_BYTE <<= 1;
    }
    ULF_BO_BYTE = ulf_transmit_id[1];
    for(i = 0; i < 32; i++){
        ULF_BO_BIT = (ULF_BO_BYTE >> 31) & 0x1;
        if(ULF_BO_BIT){
            bcm2835_gpio_write(PIN_BB, HIGH);
        }else{
            bcm2835_gpio_write(PIN_BB, LOW);
        }
        delayus(200);
        if(ULF_BO_BIT){
            bcm2835_gpio_write(PIN_BB, LOW);
        }else{
            bcm2835_gpio_write(PIN_BB, HIGH);
        }
        delayus(200);
        ULF_BO_BYTE <<= 1;
    }
}

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
//    bcm2835_set_debug(1);
    if (!bcm2835_init())
      return 1;
    // Set the pin to be an output
    bcm2835_gpio_fsel(PIN_BB, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_EN, BCM2835_GPIO_FSEL_OUTP);
    
    bcm2835_gpio_write(PIN_EN, LOW);
    delayus(1);
    bcm2835_gpio_write(PIN_BB, HIGH);
    delayus(1);
    bcm2835_gpio_write(PIN_BB, LOW);
    delayus(1);
    bcm2835_gpio_write(PIN_EN, HIGH);
    
    while (1)
    {
        bcm2835_gpio_write(PIN_EN, LOW);
        ULF_BB();
        bcm2835_gpio_write(PIN_EN, HIGH);
    }
    bcm2835_close();
    return 0;
}
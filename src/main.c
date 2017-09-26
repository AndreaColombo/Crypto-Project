#include <stm32f401xe.h>
#include <stdint.h>
#include <stdio.h>
#include "rijndael-alg-fst.h"
#include "serial.h"


/*
 * The green LED is connected to port A5,
 * -> see schematic of NUCLEO-F401RE board
 */
#define LED_GPIO        GPIOA
#define LED_PIN         5

void wait_button(void);
/**
 * Quick 'n' dirty delay
 *
 * @param time the larger it is the longer it will block
 */
static void delay(unsigned time) {
    for (unsigned i=0; i<time; i++)
        for (volatile unsigned j=0; j<20000; j++);
}

static const u8 aes_test_user_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static const u8 pt[16] = {
'A','A','A','A','A','A','A','A',
'A','A','A','A','A','A','A','A'
};
static const u8 expected_ct[16] = {
0x6c,0x2a,0x3f,0xd9,0x3a,0xa7,0xb4,0x17,0xc5,0x43,0x8d,0x26,0xa4,0x61,0x95,0x19
};

u8 ct[16];
u32 rk[128];
/**
 * Hello world blinky program
 *
 * @return never
 */
int main(void) {

    
    int del=200;
    
    int nr=rijndaelKeySetupEnc(rk, aes_test_user_key,128);
    rijndaelEncrypt(rk, nr, pt, ct);

    int cnt=1;
    for(int i=0;i<16 && cnt;i++)
        if(ct[i]!=expected_ct[i])
           cnt=0;
    /*
     * Turn on the GPIOA unit,
     * -> see section 6.3.9 in the manual
     */
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
    //int j=serialPort();
    
    /*
     * Set LED-Pin as output
     * Note: For simplicity this assumes the pin was configured
     * as input before, as it is when it comes out of reset.
     * -> see section 8.4.1 in the manual
     */
    LED_GPIO->MODER=0;
    LED_GPIO->MODER |= (0b01 << (LED_PIN << 1));
    
    if(cnt)
       del=200;
    else
        del=2000;
    //wait_button();
    while(1) {

        /*
         * LED on (drive the pin high)
         * A GPIO output pin is set to high by writing 1 to the
         * corresponding bit in the lower half of the BSRR register
         * -> see section 8.4.7 in the manual
         */
        LED_GPIO->BSRR = (1 << LED_PIN);

        delay(del);
        //write("criptandoooooo\r\n");
        /*
         * LED off (drive the pin low)
         * A GPIO output pin is set to low by writing 1 to the
         * corresponding bit in the upper half of the BSRR register
         * -> see section 8.4.7 in the manual
         */
        LED_GPIO->BSRR = (1 << (LED_PIN + 16));

        delay(del);
    }
}

void wait_button(void) {
  while (!((LED_GPIO->IDR) & (1<<0))) {}
}


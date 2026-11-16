#include <xc.h>
#include "libsae22.h"
#include <stdio.h>

void IO_Initialize(void);

void main(void) {
    IO_Initialize();
    SAE22_Initialize;

    while (1) {
        #define LED4_SetHigh();
        __delay_ms(500);
        #define LED3_SetHigh();
        __delay_ms(500);
        #define LED1_SetHigh();
        __delay_ms(500);
        #define LED0_SetHigh();
        __delay_ms(500);
    }
}

void IO_Initialize(void) {

  
}


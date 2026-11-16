#include <xc.h>
#include "libsae22.h"
#include <stdio.h>

void IO_Initialize(void);

void main(void) 
{
    //Initialisation des entrées/sorties
    IO_Initialize();
    //Initialisation de l'oscillateur interne, de l'EUSART
    SAE22_Initialize();
    
    while(1)
    {
        if (RC4==0)
        {
          PORTB = 1 << ( RC3 *2 +  RC0 );
        }
        
        
    }
}

void IO_Initialize(void)
{
    ANSELC=0;
    ANSELB=0;
    ANSELA=0;
    TRISA = 0b11111111;
    TRISB = 0b11110000;
    TRISC = 0b11111111;
    IOCBN4 = 1;
    IOCIE = 1;
    GIE = 1;
    
}
void __interrupt() int_handler(void)
{
    if (IOCBF4 == 1)
    {
        IOCBF4 = 0;
        PORTB=0;
    }
}
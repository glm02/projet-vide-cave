#include <xc.h>
#include "../lib/info2.h"

unsigned char compteur = 0;

void main(void)
{
    SAE22_initialize();

    TRISB = 0b00010000;   // RB4 entrée, RB3..RB0 sorties
    TRISC = 0b00000001;   // RC0 entrée
    PORTB = 0;

    PR2 = 124;
    T2CON = 0b01001111;   // Timer2 ON, Prescaler 16, Postscaler 5
    TMR2IE = 1;
    PEIE = 1;
    GIE = 1;

    while(1)
    {
        if (RB4 == 1)
        {
            compteur = 0;
            PORTB = (PORTB & 0b11110000);
            while(RB4 == 1);
        }
    }
}

void interrupt inthandler(void)
{
    static unsigned char i = 0;

    if (TMR2IF == 1)
    {
        TMR2IF = 0;
        if (i == 9)        // 10 interruptions = 40 ms
        {
            i = 0;

            if (RC0 == 0)
            {
                compteur++;
                if (compteur > 6) compteur = 0;
            }
            else
            {
                if (compteur == 0) compteur = 6;
                else compteur--;
            }

            PORTB = (PORTB & 0b11110000) | (compteur & 0b00001111);
        }
        else
        {
            i++;
        }
    }
}






#include <xc.h>
#include "../lib/info2.h"

unsigned char etat = 0;
unsigned char pause = 0;

void main(void)
{
    SAE22_initialize();

    TRISB = 0b00000000;   // LEDs sorties
    TRISC = 0b00011000;   // RC3 et RC4 entrées
    PORTB = 0b00000110;   // motif départ : D2 et D3 allumées
    etat = 1;

    PR2 = 124;
    T2CON = 0b01001111;   // Timer2 ON
    TMR2IE = 1;
    PEIE = 1;

    IOCCN4 = 1;           // IOC front descendant sur RC4
    IOCIE = 1;

    GIE = 1;

    while(1);             // tout géré dans l'ISR
}

void interrupt inthandler(void)
{
    static unsigned char i = 0;

    if (TMR2IF == 1)
    {
        TMR2IF = 0;
        if (i == 14)      // 15 interruptions = 60 ms
        {
            i = 0;

            if (pause == 0)
            {
                etat = !etat;

                if (RC3 == 0)
                {
                    if (etat == 1) PORTB = 0b00000110;  // D2 D3
                    else           PORTB = 0b00000000;
                }
                else
                {
                    if (etat == 1) PORTB = 0b00001001;  // D1 D4
                    else           PORTB = 0b00000000;
                }
            }
        }
        else
        {
            i++;
        }
    }

    if (IOCCF4 == 1)
    {
        IOCCF4 = 0;
        pause = !pause;
    }
}
// Projet vide-cave - IUT Lyon 1 GEII
// Test : le potentiometre sur RA0 remplace le capteur de courant.

#include <xc.h>
#include "mcc_generated_files/system/system.h"
#include "lcd.h"

#define SEUIL_BAS   91
#define SEUIL_HAUT  173

#define DEMARRAGE  0
#define MARCHE     1
#define DEFAUT     2

volatile uint8_t Top = 0;   // mis a 1 toutes les 100 ms par le Timer2

void main(void)
{
    uint8_t Etat = DEMARRAGE;
    uint8_t Compteur = 0;   // temps passe, en fois 100 ms
    uint16_t Courant;

    SYSTEM_Initialize();
    LCD_Initialize();

    // Timer2 : 0,25 us x 16 x 250 x 10 = 10 ms
    PR2 = 249;
    T2CON = 0b01001110;     // postscaler 1:10, Timer2 ON, prescaler 1:16
    TMR2IF = 0;
    TMR2IE = 1;
    PEIE = 1;
    GIE = 1;

    OPTO_TRIAC_SetHigh();   // on demarre le moteur
    LCD_WriteText("Demarrage");

    while(1) // ne se termine jamais
    {
        if(Top == 1)        // toutes les 100 ms
        {
            Top = 0;
            Compteur = Compteur + 1;
            Courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;

            if(Etat == DEMARRAGE && Compteur >= 50)    // 5 s : le pic de courant est passe
            {
                Compteur = 0;
                LCD_Clear();
                LCD_WriteText("En marche");
                Etat = MARCHE;
            }
            else if(Etat == MARCHE)
            {
                if(Courant >= SEUIL_BAS && Courant <= SEUIL_HAUT)    // courant normal
                {
                    Compteur = 0;
                }
                else if(Compteur >= 100)    // anormal depuis 10 s
                {
                    OPTO_TRIAC_SetLow();    // on coupe le moteur
                    LCD_Clear();
                    if(Courant > SEUIL_HAUT)
                    {
                        LCD_WriteText("BLOQUE");
                    }
                    else
                    {
                        LCD_WriteText("A VIDE");
                    }
                    Etat = DEFAUT;          // on n'en sort que par le bouton reset
                }
            }
        }
    }
}

// Appelee toutes les 10 ms
void __interrupt() int_handler(void)
{
    static uint8_t nb_10ms = 0;

    if(TMR2IF == 1)
    {
        TMR2IF = 0;
        nb_10ms = nb_10ms + 1;
        if(nb_10ms >= 10)   // 10 x 10 ms = 100 ms
        {
            nb_10ms = 0;
            Top = 1;
        }
    }
}

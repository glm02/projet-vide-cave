// Projet vide-cave - IUT Lyon 1 GEII - version MCC Classic
// Test : le potentiometre sur RA0 remplace le capteur de courant.

#include "mcc_generated_files/mcc.h"
#include "lcd.h"

#define SEUIL_BAS   91
#define SEUIL_HAUT  173

#define DEMARRAGE  0
#define MARCHE     1
#define DEFAUT     2

volatile uint8_t Top = 0;   // mis a 1 toutes les 100 ms par le Timer2

void IT_Timer2(void);

void main(void)
{
    uint8_t Etat = DEMARRAGE;
    uint8_t Compteur = 0;   // temps passe, en fois 100 ms
    uint16_t Courant;

    SYSTEM_Initialize();
    LCD_Initialize();

    // Enregistrement AVANT d'autoriser les IT
    TMR2_SetInterruptHandler(IT_Timer2);

    INTERRUPT_GlobalInterruptEnable();     // GIE = 1
    INTERRUPT_PeripheralInterruptEnable(); // PEIE = 1

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

// Appelee par MCC toutes les 100 ms
void IT_Timer2(void)
{
    Top = 1;
}

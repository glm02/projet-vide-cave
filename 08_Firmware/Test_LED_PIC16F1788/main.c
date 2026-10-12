/* Premier test de l'ancienne carte G206 : LED D4 sur RB0 (DIP28 broche 21).
 * Cible : PIC16F1788 (pas PIC16LF1788), compilateur MPLAB XC8.
 * Configuration nouvelle pour ce test : l'ancien programme n'a pas été lu.
 * Tester en basse tension, partie secteur/moteur deconnectee.
 */
#include <xc.h>

/* Frequence REELLE configuree plus bas ; utilisee par __delay_ms(). */
#define _XTAL_FREQ 8000000UL

#pragma config FOSC = INTOSC   // Oscillateur interne : RA6/RA7 restent disponibles
#pragma config WDTE = OFF      // Pas de watchdog pour ce premier test
#pragma config PWRTE = ON      // Temporisation de demarrage
#pragma config MCLRE = ON      // Bouton SW4 = reset sur MCLR
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config WRT = OFF
#pragma config VCAPEN = OFF    // RA6 sert au LCD ; pas de VCAP externe sur la carte
#pragma config PLLEN = OFF
#pragma config STVREN = ON
#pragma config BORV = LO
#pragma config LPBOR = OFF
#pragma config LVP = ON        // Conserver l'entree de programmation basse tension

static void initialiser_carte(void)
{
    OSCCONbits.SPLLEN = 0;
    OSCCONbits.IRCF = 0b1110;  // HFINTOSC = 8 MHz
    OSCCONbits.SCS = 0b10;     // Selection de l'oscillateur interne
    while (!OSCSTATbits.HFIOFR) {
        /* Attendre que l'oscillateur interne soit pret. */
    }

    ANSELA = 0;               // Entrees/sorties numeriques pour ce test
    ANSELB = 0;
    ADCON0 = 0;               // Capteur pas encore utilise
    CM1CON0 = 0;
    CM2CON0 = 0;
    CM3CON0 = 0;
    CM4CON0 = 0;

    /* Charger les niveaux avant d'activer les sorties. */
    LATA = 0;
    LATB = 0;
    LATC = 0;
    TRISA = 0xFF;             // LCD pas encore pilote
    TRISB = 0xFF;             // RB6/RB7 restent disponibles pour ICSP
    TRISC = 0xFF;

    TRISBbits.TRISB0 = 0;     // 0 = sortie : LED D4 via R10
    LATBbits.LATB2 = 0;       // Commande optotriac active-haut maintenue a l'arret
    TRISBbits.TRISB2 = 0;
}

void main(void)
{
    initialiser_carte();

    while (1) {
        LATBbits.LATB0 = 1;   // Allumer la LED
        __delay_ms(500);
        LATBbits.LATB0 = 0;   // Eteindre la LED
        __delay_ms(500);
    }
}

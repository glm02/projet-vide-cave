/*
 * main_test_efficace.c  -  Projet vide-cave (IUT Lyon 1 GEII)
 *
 * TEST : mesure de la valeur efficace d'une tension alternative 50 Hz
 *        (GBF sur RA0 qui simule la sortie du capteur ACS756).
 *
 * MODE SIMULATION (SIMULATION = 1) : la carte ne peut pas etre modifiee, donc le PIC
 *   FABRIQUE lui-meme une sinusoide 50 Hz (table de 100 points) centree sur 2,5 V.
 *   Le POTENTIOMETRE regle l'AMPLITUDE : il joue le role du courant du moteur.
 *   -> potar au minimum = courant faible (a vide), potar au maximum = courant fort (bloque).
 * MODE REEL (SIMULATION = 0) : on mesure vraiment RA0 (capteur ACS756 sur la carte finale).
 *
 * Principe :
 *   1) on echantillonne UNE periode complete : 100 mesures, une toutes les 200 us
 *      (100 x 200 us = 20 ms = 1 periode du 50 Hz) -> base de temps = Timer2
 *   2) moyenne des 100 points  = decalage (centre de la sinusoide)
 *   3) methode 1 : V_eff = crete / racine(2)          (vrai pour une sinusoide)
 *      methode 2 : V_eff = racine( somme(x - moy)^2 / N )   (vraie valeur efficace)
 *   4) moyenne glissante sur 8 periodes pour stabiliser l'affichage
 *   5) affichage dans RealTerm (printf, 9600 bauds)
 *
 * CAN utilise en 8 bits : resultat aligne a gauche -> on garde l'octet haut (>> 8)
 *   quantum q = 5 V / 256 = 19,5 mV par point
 *
 * Reglages MCC necessaires :
 *   - ADC   : RA0 = Capteur_COURANT, voie negative ADNREF, horloge FOSC/16, resultat aligne a GAUCHE
 *   - TMR2  : periode 200 us, interruption NON activee (on scrute TMR2IF comme en INFO2 TD4)
 *   - UART  : 9600 bauds, "Redirect Printf to UART" coche
 */

#include <xc.h>
#include <stdio.h>
#include "mcc_generated_files/system/system.h"

/* ---------- Constantes ---------- */
#define N_ECH          100     // nombre d'echantillons par periode (20 ms / 200 us)
#define N_MOYENNE      8       // moyenne glissante sur 8 periodes (diviser par 8 = >> 3)
#define FENETRE_MIN    0       // fenetre de controle (en simulation le signal va de 1 a 255)
#define FENETRE_MAX    255     // remettre 51 et 154 (1 V - 3 V) pour un test au GBF
#define AFFICHER_ECHANTILLONS 0   // 1 = envoie aussi les 100 points (pour tracer la courbe sous Excel)
#define SIMULATION     1       // 1 = sinusoide fabriquee par le PIC, amplitude reglee par le potar
                               // 0 = vraie mesure sur RA0 (carte finale + capteur)
#define CENTRE         128     // 2,5 V en points 8 bits : sortie du capteur a 0 A

/* ---------- Table de la sinusoide (mode simulation) ---------- */
const int8_t sinus[N_ECH] = {          // 100 points d'une sinusoide d'amplitude 127 (1 periode)
       0,    8,   16,   24,   32,   39,   47,   54,   61,   68,
      75,   81,   87,   93,   98,  103,  107,  111,  115,  118,
     121,  123,  125,  126,  127,  127,  127,  126,  125,  123,
     121,  118,  115,  111,  107,  103,   98,   93,   87,   81,
      75,   68,   61,   54,   47,   39,   32,   24,   16,    8,
       0,   -8,  -16,  -24,  -32,  -39,  -47,  -54,  -61,  -68,
     -75,  -81,  -87,  -93,  -98, -103, -107, -111, -115, -118,
    -121, -123, -125, -126, -127, -127, -127, -126, -125, -123,
    -121, -118, -115, -111, -107, -103,  -98,  -93,  -87,  -81,
     -75,  -68,  -61,  -54,  -47,  -39,  -32,  -24,  -16,   -8
};

/* ---------- Variables globales ---------- */
uint8_t echantillons[N_ECH];         // une periode de mesures (100 octets de RAM)
uint8_t historique[N_MOYENNE];       // les 8 dernieres valeurs efficaces
uint8_t index_historique = 0;        // case a remplir dans l'historique

/* ---------- Prototypes ---------- */
uint8_t  Lire_CAN_8bits(void);
void     Echantillonner_Periode(void);
uint8_t  Calcul_Moyenne(void);
uint8_t  Calcul_Crete(void);
uint8_t  Racine_Entiere(uint16_t x);
uint8_t  Calcul_Efficace(uint8_t moyenne);
uint8_t  Moyenne_Glissante(uint8_t nouvelle_valeur);
uint16_t Points_vers_mV(uint8_t points);
uint8_t  Compter_Hors_Fenetre(void);

/* ======================================================================= */
void main(void)
{
    uint8_t moyenne;
    uint8_t crete;
    uint8_t eff_methode1;
    uint8_t eff_methode2;
    uint8_t eff_lisse;
    uint8_t hors_fenetre;
    uint8_t i;

    SYSTEM_Initialize();     // MCC : horloge 16 MHz, broches, CAN, Timer2, UART

    printf("\r\n--- Test valeur efficace (CAN 8 bits, 100 pts / 20 ms) ---\r\n");

    while (1) // ne se termine jamais
    {
        /* 1) Mesure d'une periode complete (pas de printf pendant ce temps !) */
        Echantillonner_Periode();

        /* 2) Calculs */
        moyenne      = Calcul_Moyenne();
        crete        = Calcul_Crete();
        eff_methode1 = (uint8_t)(((uint16_t)crete * 181) >> 8);   // crete / racine(2)  (181/256 = 0,707)
        eff_methode2 = Calcul_Efficace(moyenne);
        eff_lisse    = Moyenne_Glissante(eff_methode2);
        hors_fenetre = Compter_Hors_Fenetre();

        /* 3) Affichage dans RealTerm */
        printf("Moy=%u (%u mV)  Crete=%u  Veff1=%u  Veff2=%u pts  ->  Veff=%u mV",
               moyenne, Points_vers_mV(moyenne), crete,
               eff_methode1, eff_methode2, Points_vers_mV(eff_lisse));

        if (hors_fenetre > 0)
        {
            printf("  ATTENTION %u pts hors fenetre", hors_fenetre);
        }
        printf("\r\n");

        if (AFFICHER_ECHANTILLONS == 1)
        {
            for (i = 0; i < N_ECH; i++)
            {
                printf("%u\r\n", echantillons[i]);
            }
        }

        __delay_ms(300);      // environ 3 affichages par seconde
    }
}

/* ======================================================================= */
/* Une conversion du CAN, ramenee sur 8 bits (resultat aligne a gauche -> octet haut) */
uint8_t Lire_CAN_8bits(void)
{
    return (uint8_t)(ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8);
}

/* Remplit echantillons[] : une mesure toutes les 200 us, cadencee par le Timer2 (scrutation de TMR2IF) */
void Echantillonner_Periode(void)
{
    uint8_t i;
    uint8_t amplitude;

    amplitude = Lire_CAN_8bits() >> 1;   // potar 0-255 -> amplitude 0-127 points (simulation)

    TMR2IF = 0;                      // on repart d'un drapeau propre
    for (i = 0; i < N_ECH; i++)
    {
        while (TMR2IF == 0)          // attente du debordement du Timer2 (200 us)
        {}
        TMR2IF = 0;                  // RAZ le plus tot possible

        if (SIMULATION == 1)
        {
            // echantillon = centre + sinus x amplitude / 128   (division par 128 = >> 7)
            echantillons[i] = (uint8_t)(CENTRE + (((int16_t)sinus[i] * amplitude) >> 7));
        }
        else
        {
            echantillons[i] = Lire_CAN_8bits();   // vraie mesure du capteur
        }
    }
}

/* Moyenne des 100 points = centre de la sinusoide (decalage) */
uint8_t Calcul_Moyenne(void)
{
    uint16_t somme = 0;              // max 100 x 255 = 25 500 -> tient dans un uint16_t
    uint8_t i;

    for (i = 0; i < N_ECH; i++)
    {
        somme = somme + echantillons[i];
    }
    return (uint8_t)(somme / N_ECH);
}

/* Methode 1 : demi-ecart crete-a-crete = (max - min) / 2 */
uint8_t Calcul_Crete(void)
{
    uint8_t maxi = 0;
    uint8_t mini = 255;
    uint8_t i;

    for (i = 0; i < N_ECH; i++)
    {
        if (echantillons[i] > maxi)
        {
            maxi = echantillons[i];
        }
        if (echantillons[i] < mini)
        {
            mini = echantillons[i];
        }
    }
    return (uint8_t)((maxi - mini) / 2);
}

/* Racine carree entiere : plus grand r tel que r x r <= x  (x <= 16 129 donc r <= 127) */
uint8_t Racine_Entiere(uint16_t x)
{
    uint16_t r = 0;

    while ((r + 1) * (r + 1) <= x)
    {
        r = r + 1;
    }
    return (uint8_t)r;
}

/* Methode 2 : vraie valeur efficace = racine( somme des (x - moyenne)^2 / N ) */
uint8_t Calcul_Efficace(uint8_t moyenne)
{
    uint32_t somme_carres = 0;       // max 100 x 127^2 = 1 612 900 -> il FAUT un uint32_t
    int16_t  ecart;                  // x - moyenne peut etre negatif -> type SIGNE
    uint8_t  i;

    for (i = 0; i < N_ECH; i++)
    {
        ecart = (int16_t)echantillons[i] - (int16_t)moyenne;
        somme_carres = somme_carres + (uint16_t)(ecart * ecart);
    }
    return Racine_Entiere((uint16_t)(somme_carres / N_ECH));
}

/* Moyenne des 8 dernieres valeurs efficaces (division par 8 = decalage >> 3) */
uint8_t Moyenne_Glissante(uint8_t nouvelle_valeur)
{
    uint16_t somme = 0;
    uint8_t i;

    historique[index_historique] = nouvelle_valeur;   // on remplace la plus ancienne
    index_historique = index_historique + 1;
    if (index_historique >= N_MOYENNE)
    {
        index_historique = 0;
    }

    for (i = 0; i < N_MOYENNE; i++)
    {
        somme = somme + historique[i];
    }
    return (uint8_t)(somme >> 3);
}

/* Conversion points 8 bits -> millivolts : mV = points x 5000 / 256 */
uint16_t Points_vers_mV(uint8_t points)
{
    return (uint16_t)(((uint32_t)points * 5000) >> 8);   // 255 x 5000 = 1 275 000 -> calcul en uint32_t
}

/* Nombre d'echantillons en dehors de la fenetre 1 V - 3 V */
uint8_t Compter_Hors_Fenetre(void)
{
    uint8_t compte = 0;
    uint8_t i;

    for (i = 0; i < N_ECH; i++)
    {
        if ((echantillons[i] < FENETRE_MIN) || (echantillons[i] > FENETRE_MAX))
        {
            compte = compte + 1;
        }
    }
    return compte;
}

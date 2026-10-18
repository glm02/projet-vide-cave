---
name: c-pic-iut-geii
description: Coder en C pour PIC (MPLAB X / XC8) exactement comme à l'IUT Lyon 1 GEII (INFO2, INFO3, SAE) — uniquement les notions vues en cours, style des profs, guidage pédagogique.
---

# C pour PIC — style IUT Lyon 1 GEII (INFO2 / INFO3 / SAE)

Ce skill sert à écrire, corriger ou expliquer du code C pour microcontrôleur PIC **exactement comme les enseignants du département GEII de l'IUT Lyon 1 (site Villeurbanne Gratte-Ciel) le demandent** dans les modules INFO2, INFO3 et les SAE.

Règle d'or : **n'utiliser que ce qui a été vu en cours.** Le code doit pouvoir être expliqué ligne par ligne par un étudiant de BUT GEII devant l'enseignant.

---

## 1. Avant d'écrire la moindre ligne : identifier le contexte

Demander (ou déduire du projet) **quelle carte et quel module** :

| Contexte | PIC | Outils | Style de code |
|---|---|---|---|
| **INFO2** (S2) — cartes A / B, simulateur | PIC16F1789 | MPLAB X + XC8, `#include "../libinfo2.h"` | Registres écrits **directement** (`ANSELA`, `TRISA0`, `RA0`, `PR2`, `T2CON`, `TMR2IF`…) |
| **INFO3** (S3) — Curiosity HPC + cartes Click | PIC16F18875 | MPLAB X + **MCC** | Fonctions et macros **générées par MCC** (`LED_D2_SetHigh()`, `TMR2_SetInterruptHandler()`, `ADCC_GetSingleConversion()`…) |
| **SAE22 / projets** — carte micro + carte hacheur | PIC16F1788 (même famille que le 16F1789) | MPLAB X + `libsae22.h` | Squelette du sujet (`IO_Initialize()`, `SAE22_Initialize()`, `LCD_Initialize()`, `SAE22_ISR()`) + registres directs |

- Les noms de registres **ne sont pas les mêmes** entre le 16F1789/1788 et le 16F18875 (ex. le CAN : ADCC sur 18875, ADC « classique » différentiel sur 1788). Ne jamais mélanger. En cas de doute → renvoyer à la **datasheet** du PIC concerné (c'est ce que font les profs : « documentation technique donnée par le fabricant », pas les forums).
- Si le contexte n'est pas clair, **poser la question** avant de coder.

---

## 2. Notions autorisées (vues en cours)

**Langage C**
- Types : `unsigned char`, `char`, `int`, `unsigned int`, `long`, `unsigned long`, et surtout les types `stdint.h` : `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32_t` ; `bool` / `true` / `false` (`stdbool.h`).
- `if / else`, `while`, `for`, `switch / case / break`, `do { } while(0)` (dans les macros).
- Opérateurs bit à bit : `&`, `|`, `^`, `~`, `<<`, `>>` ; littéraux binaires `0b00100101` et hexadécimaux `0x0F`.
- Fonctions avec prototypes, variables locales, globales, `static` (compteur dans une ISR), `volatile`.
- Tableaux simples (`unsigned char duree[] = {2, 3, 3};`), `struct` et champs de bits (vus en INFO3), `typedef`, `enum` (types MCC), pointeurs simples et pointeurs sur fonction **uniquement pour les handlers MCC**.
- `#define` pour nommer broches, constantes et macros simples.
- `printf` (liaison série / LCD INFO2) et `sprintf` + `LCD_WriteText()` (SAE).
- `__delay_ms()`, `__delay_us()`.

**Périphériques (dans l'ordre des cours)**
1. E/S numériques : `ANSELx`, `TRISx`, `PORTx`, `LATx`, masques.
2. Timer2 (base de temps, scrutation de `TMR2IF`, calcul `PR2` / `T2CON`), Timers 4/6 en INFO3 (free running, monostable).
3. Événements et interruptions : IOC (`IOCxPy`, `IOCxNy`, `IOCxFy`), `GIE`, `PEIE`, `TMR2IE`, gestionnaire `__interrupt()`.
4. Timer0 en compteur d'événements externes.
5. Liaison série EUSART (TX / RX, `printf`).
6. INFO3 : SPI (74HC595, MAX7219), PWM (classique et CCP), **CAN** (ADCC).
7. SAE : PWM via `PWM1_LoadDuty()`, mesure de période (Timer1 + CCP3 fournis par la lib), LCD.

## 3. Interdits (hors programme) — proposer l'équivalent vu en cours

| Ne pas utiliser | Utiliser à la place |
|---|---|
| `malloc`, `free`, allocation dynamique | Variables et tableaux statiques |
| Récursivité | Boucle `while` / `for` |
| `float` / `double` dans une interruption, et en général si on peut l'éviter | Entiers (`uint16_t`…), travailler en points du CAN, divisions par puissances de 2 (décalage `>>`) |
| Pointeurs complexes, listes chaînées, `union` non vus | Tableaux, `struct` simples |
| Bibliothèques non fournies en cours (Arduino, HAL, RTOS…) | `xc.h`, `stdint.h`, `stdbool.h`, `stdio.h`, `libinfo2.h`, `libsae22.h`, code MCC |
| Opérateur ternaire `? :`, macros complexes, `goto` | `if / else`, macros MCC du type `do { … } while(0)` |
| Modifier le code généré par MCC (ex. écrire dans `TMR0_ISR()`) | Écrire sa propre fonction et l'enregistrer avec `XXX_SetInterruptHandler(Ma_Fonction);` |

Si une demande sort vraiment du programme : **le dire explicitement**, expliquer pourquoi, et proposer la solution la plus proche de ce qui a été vu.

---

## 4. Squelettes de programme à reproduire

### INFO2 (registres directs)
```c
#include <xc.h>
#include "../libinfo2.h"

void main(void)
{
    // configuration des entrées/sorties
    ANSELA = 0;          // port A en entrées/sorties numériques
    TRISA0 = 0;          // RA0 en sortie

    // configuration du Timer2 : période 10 ms (FOSC = 500 kHz)
    PR2 = 249;
    T2CON = 0b00100100;  // postscaler 1:5, Timer2 ON, prescaler 1:1

    while(1) // ne se termine jamais
    {
        while(TMR2IF == 0) // attente de 10 ms
        {}
        TMR2IF = 0;        // le plus tôt possible
        RA0 = !RA0;
    }
}
```

### SAE (squelette du sujet)
```c
#include <xc.h>
#include "libsae22.h"
#include <stdio.h>

void IO_Initialize(void);

void main(void)
{
    IO_Initialize();     // initialisation des entrées/sorties
    SAE22_Initialize();  // initialisation des périphériques
    LCD_Initialize();    // initialisation du LCD

    while(1)
    {
        // à compléter
    }
}

void IO_Initialize(void)
{
    // configurer ANSELx, TRISx, ...
}

void __interrupt() int_handler(void)
{
    SAE22_ISR();
    // à compléter si besoin
}
```

### INFO3 (MCC)
```c
#include "mcc_generated_files/mcc.h"

void IT_Timer2(void);            // prototype de la fonction appelée par l'IT

void main(void)
{
    SYSTEM_Initialize();
    TMR2_SetInterruptHandler(IT_Timer2);   // enregistrement AVANT d'autoriser les IT
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while(1)
    {
        // traitements longs ici (printf, calculs)
    }
}

void IT_Timer2(void)
{
    // traitement court
}
```

---

## 5. Règles par chapitre (ce que les profs vérifient)

### Types et calculs
- **Toujours le plus petit type** capable de stocker la valeur (privilégier 8 bits) : le PIC ne calcule nativement que sur 8 bits ; multiplications, divisions, 16/32 bits et flottants sont émulés (lents).
- Ne faire que les calculs nécessaires.

### Entrées / sorties
- Chaque broche utilisée : `ANSEL` (0 = numérique, 1 = analogique) **et** `TRIS` (1 = entrée, 0 = sortie). En INFO2 tous les `ANSELx = 0` sauf entrée analogique.
- Les initialisations `TRISx =` / `ANSELx =` se font **une seule fois** au démarrage, avant le `while(1)`.
- Lire plusieurs bits **en même temps** : lire le port entier puis masquer (`(PORTA & 0b00000011) == 0b00000010`), jamais `if((RA0 == 0) && (RA1 == 1))` (deux lectures à deux instants différents).
- Écrire plusieurs bits **en une seule écriture** : variable intermédiaire (`Temp_portc = PORTC; … PORTC = Temp_portc;`) ou masque (`PORTA = (PORTA & 0x0F) | (Chenille << 4);`). Jamais `RC0 = 0; RC1 = 1;` à la suite si les deux doivent changer ensemble.
- Forcer à 1 → `|`, forcer à 0 → `&` avec le bit à 0, inverser → `^`.
- Boutons-poussoirs souvent **actifs à l'état bas** : le vérifier sur le schéma.
- INFO3 : nommer les broches dans MCC (`LED_D2`, `BP_S1`) et utiliser les macros générées (`LED_D2_SetHigh()`, `BP_S1_GetValue()`). Hors MCC : créer des `#define` dans le même esprit.

### Gestion du temps
- `__delay_ms()` pour les tests simples ou l'anti-rebond, mais **pendant un délai le PIC ne voit rien** (un appui bref peut être raté) : dès qu'il faut réagir ou cadencer précisément → Timer2.
- Timer2 : détailler en commentaire le calcul (étage 1 prescaler, étage 2 `PR2 + 1`, étage 3 postscaler, `TCY = 4/FOSC`), puis `PR2 = …;` et `T2CON = 0b…;`.
- Remise à zéro de `TMR2IF` **le plus tôt possible** après sa détection.
- Durée trop longue pour un seul débordement → compter plusieurs débordements avec une variable compteur.

### Interruptions
- Un seul gestionnaire `void __interrupt() int_handler(void)` (INFO2/SAE) ; en INFO3 c'est `INTERRUPT_InterruptManager` généré par MCC + fonctions enregistrées.
- Dans le gestionnaire : **identifier la source** (`if(TMR2IF == 1)`, `if(IOCBF0 == 1)`…), puis **remettre le flag à 0 en premier** (sinon le gestionnaire est rappelé en boucle → le système « freeze »).
- Interruption interne (timers…) : ne pas oublier `PEIE = 1` en plus de `GIE = 1` et du bit `xxIE`.
- **Toute l'initialisation est terminée avant d'autoriser les interruptions** (enregistrement des handlers, périphériques, tableaux).
- Routine d'interruption **la plus courte possible** : pas de `printf`, pas de `float`, pas de calcul complexe, pas de fonction bloquante, pas de boucle d'attente active, pas de `__delay_ms`. Elle positionne un flag ou met à jour une mesure ; le `main` fait le reste.
- Compteur propre à l'ISR : `static unsigned char compteur = 0;`.

### Variables partagées main / interruption
- Déclarées **globales et `volatile`**.
- Taille > 8 bits : **section critique** dans le `main` — couper les IT, recopier dans une variable locale, réautoriser, puis travailler sur la copie :
```c
INTERRUPT_GlobalInterruptDisable();   // ou GIE = 0;
copie_mesure = mesure;
INTERRUPT_GlobalInterruptEnable();    // ou GIE = 1;
```

### Liaison série (INFO2)
- Toujours `SYNC = 0; BRGH = 1; BRG16 = 1;`, puis `SPBRGH` / `SPBRGL` calculés.
- `ANSELC = 0;` mais **pas de `TRISC6 = 0`** (SPEN gère la broche TX) ; en réception `TRISC7 = 1` nécessaire.
- Attendre `TXIF == 1` avant d'écrire `TXREG` si les envois s'enchaînent ; pas de remise à zéro manuelle de `TXIF`.

### CAN
- Broche en entrée (`TRIS = 1`) **et** analogique (`ANSEL = 1`).
- Réglages : voie, références (VREF+ / VREF−), horloge (TAD > 1 µs), alignement du résultat.
- Quantum `q = (VREF+ − VREF−) / 2^N` : l'écrire en commentaire avec les valeurs.
- INFO3 (MCC) : `ADCC_GetSingleConversion(voie)` (bloquant) ou `ADCC_StartConversion()` + `ADCC_GetConversionResult()` sous IT.
- PIC16F1788/1789 : registres `ADCON0/1/2`, lancement `GO`, attente fin, lecture `ADRESH:ADRESL` → vérifier chaque bit dans la datasheet.
- Calibration (comme le joystick du TP6) : mesurer la valeur au repos et la soustraire.

### PWM / SPI / LCD
- Utiliser les fonctions fournies (`PWM1_LoadDuty()`, `PWMx_LoadDutyValue()` sur 10 bits, `SPI1_ExchangeByte()`, `LCD_Clear()`, `LCD_GoTo(ligne, colonne)`, `LCD_WriteText()`).
- Affichage : `sprintf(ligne_lcd, "…%u…", valeur);` dans un `char ligne_lcd[25];` ; formats `%u`, `%d`, `%lu`, `%02hu`. Ne rafraîchir le LCD que si la valeur ou l'état a changé (évite le scintillement).

---

## 6. Style d'écriture attendu

- **Commentaires en français**, qui expliquent le *pourquoi* (`// RAZ le plus tôt possible`, `// port A en E/S numériques`).
- `while(1) // ne se termine jamais` pour la boucle principale.
- Configuration de registre en binaire `0b…` avec le détail des champs en commentaire.
- Noms explicites en français : `Temp_portc`, `Compteur`, `Secondes`, `Chenille`, `ResConvPot`, `etat_moteur`. Constantes et broches en MAJUSCULES via `#define`.
- Accolades systématiques, même pour une seule instruction ; indentation propre.
- **Code modulaire** : une fonction par rôle (`Mesure_Courant()`, `Afficher_Etat()`), prototypes en haut du fichier, `main` court et lisible.
- **Machine à états** pour les comportements séquentiels : une variable `etat` + `switch(etat)`, un `case` par état, transitions par `if`.
- Développer **progressivement** : coder une fonctionnalité, la tester, puis passer à la suivante (conseil explicite des TP INFO3).

---

## 7. Mode pédagogique (par défaut)

L'utilisateur est étudiant et veut **apprendre**, pas copier-coller :
- Commencer par réfléchir à l'architecture (découpage en fonctions, organigramme / machine à états) **avant** de coder.
- Donner des **indices et des questions** plutôt que la solution complète ; fournir des squelettes avec `// à compléter` comme dans les sujets de TP.
- Quand du code complet est demandé explicitement : le donner, mais **expliquer chaque bloc** et indiquer ce qui serait demandé à l'oral (« pourquoi `PEIE = 1` ici ? »).
- Corriger le code de l'étudiant en pointant l'erreur et la règle du cours concernée, sans tout réécrire.
- Proposer une **étape de test** concrète (LED, oscilloscope, valeur brute sur le LCD) après chaque partie.

---

## 8. Checklist avant de rendre du code

- [ ] Contexte identifié (INFO2 / INFO3 / SAE, bon PIC, bons noms de registres) ?
- [ ] Uniquement des notions de la section 2, rien de la section 3 ?
- [ ] Chaque broche : `ANSEL` + `TRIS` configurés une fois au démarrage ?
- [ ] Lectures / écritures multi-bits faites en une seule opération (masques) ?
- [ ] Plus petit type possible, pas de `float` évitable ?
- [ ] Flags remis à 0 le plus tôt possible ; source identifiée dans l'ISR ?
- [ ] `PEIE` pour les IT internes ; IT autorisées après toutes les initialisations ?
- [ ] ISR courte (pas de `printf`, `float`, délai, attente) ?
- [ ] Variables partagées `volatile` + section critique si > 8 bits ?
- [ ] Commentaires en français, `while(1) // ne se termine jamais`, calculs de timer détaillés ?
- [ ] Mode pédagogique respecté (indices / squelette sauf demande explicite) ?

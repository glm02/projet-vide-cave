# Premier programme C : LED sur l'ancienne carte G206

Ouvrir [main.c](main.c). Cible déduite du schéma : **PIC16F1788, boîtier DIP28**. Vérifier que le marquage du composant monté correspond avant de programmer. Le dossier ne contient pas de projet MPLAB `.X` préconfiguré pour un programmateur précis : le créer avec l'assistant ci-dessous, puis ajouter ce fichier.

Le test fait clignoter **D4 sur RB0, broche 21**, par R10 : 500 ms allumée, 500 ms éteinte. La sortie RB2 est maintenue à zéro. Aucun programme de protection moteur ou de mesure n'est actif. Les autres lignes sont laissées en entrée ; LCD, UART et capteur ne sont pas encore pilotés.

## Ouvrir dans MPLAB X

1. `File → New Project → Microchip Embedded → Standalone Project`.
2. Device : `PIC16F1788` ; ne pas sélectionner une référence voisine.
3. Hardware Tool : ton PICkit, avec son numéro de série. Si le modèle reste inconnu, utiliser `Simulator` pour compiler uniquement.
4. Compiler : `XC8 v3.10`, installé sur ce PC. Projet `Test_LED_G206`, emplacement dans `08_Firmware` ; créer un dossier projet `.X` distinct de ce dossier de sources.
5. Dans le projet : clic droit `Source Files → Add Existing Item`, sélectionner le `main.c` de ce dossier avec un chemin relatif. Retirer tout autre `main()` de ce premier projet.
6. `Clean and Build Project` : le résultat attendu est `BUILD SUCCESSFUL`.
7. Après vérification du raccordement, utiliser `Make and Program Device` pour écrire le programme. Cette action remplace le programme déjà présent dans le PIC.

[Assistant officiel de création de projet](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/ides/x/projects/creating/project/).

Le PC dispose actuellement de **MPLAB X 6.30 et XC8 3.10**. **PICkit 3 : utiliser MPLAB X 6.20 ou une version compatible plus ancienne** ; 6.20 est la dernière version prenant en charge ce programmateur. Ne pas désinstaller automatiquement 6.30. [Compatibilité annoncée par Microchip](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide).

## Raccordement ICSP d'après le schéma

| Contact J4 de la carte | Signal | Broche PIC DIP28 |
|---|---|---:|
| 1 | MCLR/VPP | 1 |
| 2 | +5 V / VDD | 20 |
| 3 | GND / VSS | 8 et 19 |
| 4 | ICSPDAT / PGD | 28 / RB7 |
| 5 | ICSPCLK / PGC | 27 / RB6 |
| 6 | Non utilisé | — |

Les contacts 7/8 du connecteur physique ne sont pas des signaux supplémentaires du PICkit. **Ce tableau décrit J4, pas nécessairement l'ordre du câble d'un PICkit à 8 contacts.** Vérifier le repère de broche 1 et le brochage officiel du modèle de PICkit utilisé avant de raccorder ; ne pas décaler l'adaptateur.

Premier test en basse tension, sans secteur ni moteur raccordé. Si la carte est alimentée par J1 avec une source 5 V externe, sélectionner la cible alimentée extérieurement dans MPLAB et laisser l'alimentation de cible par PICkit désactivée. La liaison VDD reste nécessaire pour la détection de tension. N'activer l'alimentation par PICkit qu'après vérification de sa capacité, de la consommation et de l'absence d'autre source.

Le PICkit n'a pas été identifié dans l'inventaire USB effectué et aucune programmation n'a été exécutée. Ce n'est pas une preuve que le matériel est débranché : vérifier la détection du numéro de série dans MPLAB.

## Comprendre les premières lignes C

- `#include <xc.h>` donne accès aux registres et noms de bits de ce PIC.
- `#pragma config` configure les options persistantes programmées avec le firmware ; ce n'est pas une récupération automatique de l'ancien firmware.
- `OSCCON` règle l'horloge à 8 MHz. `_XTAL_FREQ` indique la même fréquence aux fonctions de délai ; changer seulement cette constante ne change pas l'horloge réelle.
- `ANSELB = 0` rend RB0 numérique. `TRISBbits.TRISB0 = 0` configure la broche en sortie (`1` signifie entrée).
- `LATBbits.LATB0` commande le niveau de sortie sans lire puis réécrire les autres broches via `PORTB`.
- `while (1)` répète le programme indéfiniment.

L'oscillateur interne, CLKOUT désactivé et VCAP désactivé libèrent RA6/RA7 pour le LCD de cette carte. MCLR reste activé pour SW4 et la programmation. Les noms des options ont été lus dans le pack Microchip PIC12-16F1xxx installé ; ces réglages ne prétendent pas reproduire les anciennes valeurs du PIC.

## Compilation locale sans ouvrir l'IDE

```powershell
powershell -ExecutionPolicy Bypass -File .\compiler.ps1
```

Ce script compile avec XC8 et le pack installés ; il **ne programme pas le PIC**. Adapter ses paramètres de version sur un autre PC. Les fichiers générés restent dans `build/`, exclus de Git.

## Suite du développement

Après observation du clignotement sur la vraie carte : LCD 16×2 avec le brochage exact du schéma, UART de diagnostic, acquisition ADC, offset/RMS, puis états d'alarme et arrêt. Les liaisons capteur/PIC et commande optotriac doivent être corrigées matériellement avant d'utiliser ces fonctions. Les délais bloquants de ce premier test seront remplacés par une base de temps pour le programme final.

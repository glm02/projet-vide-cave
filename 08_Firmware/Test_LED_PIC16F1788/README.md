# Premier programme C : LED sur l'ancienne carte G206

Ouvrir [main.c](main.c). Cible déduite du schéma : **PIC16F1788, boîtier DIP28**. Vérifier que le marquage du composant monté correspond avant de programmer. Le dossier ne contient pas de projet MPLAB `.X` préconfiguré pour un programmateur précis : le créer avec l'assistant ci-dessous, puis ajouter ce fichier.

Le test fait clignoter **D4 sur RB0, broche 21**, par R10 : 500 ms allumée, 500 ms éteinte. La sortie RB2 est maintenue à zéro. Aucun programme de protection moteur ou de mesure n'est actif. Les autres lignes sont laissées en entrée ; LCD, UART et capteur ne sont pas encore pilotés.

## Ouvrir dans MPLAB X

1. `File → New Project → Microchip Embedded → Standalone Project`.
2. Device : `PIC16F1788` ; ne pas sélectionner une référence voisine.
3. Hardware Tool : **MPLAB SNAP (PG164100)**, avec son numéro de série. Si aucun numéro n'est détecté, utiliser `Simulator` pour compiler uniquement puis résoudre la détection USB.
4. Compiler : `XC8 v3.10`, installé sur ce PC. Projet `Test_LED_G206`, emplacement dans `08_Firmware` ; créer un dossier projet `.X` distinct de ce dossier de sources.
5. Dans le projet : clic droit `Source Files → Add Existing Item`, sélectionner le `main.c` de ce dossier avec un chemin relatif. Retirer tout autre `main()` de ce premier projet.
6. `Clean and Build Project` : le résultat attendu est `BUILD SUCCESSFUL`.
7. Après vérification du raccordement, utiliser `Make and Program Device` pour écrire le programme. Cette action remplace le programme déjà présent dans le PIC.

[Assistant officiel de création de projet](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/ides/x/projects/creating/project/).

Le PC dispose actuellement de **MPLAB X 6.30 et XC8 3.10**. **PICkit 3 : utiliser MPLAB X 6.20 ou une version compatible plus ancienne** ; 6.20 est la dernière version prenant en charge ce programmateur. Ne pas désinstaller automatiquement 6.30. [Compatibilité annoncée par Microchip](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide).

Le matériel précisé par l'équipe est **PG164100 = MPLAB SNAP**, utilisable avec cette version 6.30. Le cas PICkit 3 ci-dessus ne s'applique donc pas à notre matériel. Le pack Snap installé référence le PIC16F1788. [Fiche outil Microchip](https://www.microchip.com/en-us/development-tool/PG164100).

## Raccordement ICSP d'après le schéma

| Contact J4 de la carte | Signal | Broche PIC DIP28 |
|---|---|---:|
| 1 | MCLR/VPP | 1 |
| 2 | +5 V / VDD | 20 |
| 3 | GND / VSS | 8 et 19 |
| 4 | ICSPDAT / PGD | 28 / RB7 |
| 5 | ICSPCLK / PGC | 27 / RB6 |
| 6 | Non utilisé | — |

Le SNAP utilise, en ICSP, le même ordre pour ses contacts 1 à 5 : MCLR, VDD, GND, PGD, PGC. Relier ces contacts aux contacts J4 de même numéro ; 6/7/8 ne sont pas nécessaires ici. Vérifier le repère de broche 1 et l'orientation du câble ; ne pas décaler l'adaptateur. [Brochage officiel SNAP](https://onlinedocs.microchip.com/oxy/GUID-671CAF55-DB13-4D9A-8881-F58C984111B5-en-US-4/GUID-ECE487E3-CCF2-49C8-97A6-01CEEDE5FD22.html).

Premier test en basse tension, sans secteur ni moteur raccordé. **Le SNAP n'alimente pas la carte** : fournir le 5 V externe à J1. Sa broche VDD détecte la tension de la cible ; elle n'est pas une sortie d'alimentation. Relier la masse commune et laisser la cible réglée comme alimentée extérieurement. [Guide SNAP, alimentation et programmation](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/UserGuides/MPLAB-Snap-In-Circuit-Debugger-Users-Guide-50002787.pdf).

Le SNAP n'a pas encore été identifié dans l'inventaire USB effectué et aucune programmation n'a été exécutée. Ce n'est pas une preuve que le matériel est débranché : vérifier la détection du numéro de série dans MPLAB, ainsi que le câble Micro-USB qui doit transporter les données.

**Programmation basse tension obligatoire avec le SNAP.** Le code préparé conserve `#pragma config LVP = ON`. Si l'ancien firmware a déjà programmé le PIC avec LVP désactivé, modifier le nouveau code ne suffit pas : le SNAP ne pourra pas entrer en programmation. Il faudra réactiver LVP avec un programmateur capable de programmation haute tension, par exemple un PICkit approprié. La programmation haute tension signifie une procédure ICSP spécifique sur MCLR, jamais alimenter VDD avec une tension élevée. [PIC16F1788, CONFIG2 et section 29](https://ww1.microchip.com/downloads/en/DeviceDoc/40001675C.pdf).

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

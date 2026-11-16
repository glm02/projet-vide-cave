# Projet principal GEII : MPLAB X et MCC Melody

Le projet utilise **MCC Generator/Melody** avec `Projet_vide_cave.mc3`. Cible : PIC16F1788 ; compilateur XC8 3.10 ; outil MPLAB SNAP PG164100.

Le test ajouté manuellement par l'assistant a été retiré à la demande de l'équipe. Le fichier `.mc3`, le choix du SNAP et les réglages saisis dans MPLAB sont conservés. Horloge, ports, configuration et pilotes doivent être produits par MCC.

## Méthode retenue

1. Configurer les périphériques et broches dans MCC selon le cours et le schéma.
2. Sauvegarder puis cliquer **Generate**. Aucun dossier C généré n'était encore présent lors du nettoyage.
3. Conserver la structure produite par MCC ; Melody appelle normalement `SYSTEM_Initialize()` au démarrage.
4. Ajouter le code applicatif dans les emplacements prévus, avec les fonctions réellement présentes dans les en-têtes générés. Les noms et l'organisation doivent suivre l'exemple du cours GEII ; aucun style particulier n'est supposé à partir du seul nom GEII.
5. Compiler avec **Clean and Build Project**. Générer et compiler ne nécessite pas de connexion au SNAP.

[Génération MCC](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mcc/config/) · [Architecture Melody](https://onlinedocs.microchip.com/oxy/GUID-5A03F818-B7FC-4062-9792-57D08543B586-en-US-11/GUID-E1095A63-3FC9-4F82-8375-1F98E8C09F61.html).

## Brochage du schéma pour MCC

À confronter à la carte montée :

| Fonction | Port PIC | Broche DIP28 |
|---|---|---:|
| Courant, ADC prévu | RA0 / AN0 | 2 |
| LCD RS, RW, E | RA1, RA2, RA3 | 3, 4, 5 |
| LCD D4, D5, D6, D7 | RA4, RA5, RA7, RA6 | 6, 7, 9, 10 |
| LED D4 via R10 | RB0 | 21 |
| Buzzer prévu via R9 | RB1 | 22 |
| Commande optotriac prévue | RB2 | 23 |
| UART TX, RX | RC6, RC7 | 17, 18 |
| ICSP CLK, DAT | RB6, RB7 | 27, 28 |
| Reset SW4 | MCLR | 1 |

Configurer dans MCC l'oscillateur interne et CLKOUT/VCAP désactivés pour libérer RA6/RA7 du LCD, selon le montage et le cours. Garder LVP activé avec le SNAP. Les ruptures capteur/PIC et commande/opto relevées au schéma restent à corriger matériellement.

## SNAP et incident USB

Le SNAP n'alimente pas la cible : 5 V externe sur J1 pour ce montage. Contacts ICSP 1–5 vers J4 1–5 : MCLR, VDD de détection, GND, PGD, PGC ; vérifier le sens. Première mise au point sans secteur ni moteur raccordé. [Guide SNAP](https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/UserGuides/MPLAB-Snap-In-Circuit-Debugger-Users-Guide-50002787.pdf).

Le 12 octobre, le téléchargement de firmware a échoué : timeout puis transmission sur endpoint 2, erreur `-10121`. Le journal indique environ 300 secondes d'attente après l'envoi du bootloader, puis une perte de communication avec le SNAP. Ce n'est pas une erreur de compilation C ni une preuve de panne du PIC cible.

Snap_TP 2.12.1787 attend application 02.04.10 et bootloader 01.01.55 ; le SNAP a annoncé 02.01.06 et 01.00.00 avant l'échec. Cela explique la mise à jour automatique demandée par MPLAB. Le firmware réellement présent après l'échec n'a pas été relu avec succès. **Aucune nouvelle mise à jour ni récupération forcée n'est autorisée à ce stade.** Pour utiliser un ancien firmware, il faut identifier un pack compatible ; aucune version n'est choisie au hasard. [Choisir le pack d'outil](https://support.microchip.com/s/article/MPLAB-X-IDE---Specifying-Tool-Pack-Version).

Après `Connection Failed`, si aucun téléchargement n'est actif : vérifier câble USB de données, connexion directe sans hub et reconnexion du SNAP. Ne pas interrompre une écriture active. Les journaux `queuelogs`, sorties binaires et préférences privées restent locaux.

Il faudra générer avec MCC puis recompiler. **Le HEX du test manuel précédent n'est plus la version à utiliser.** La configuration et les fichiers MCC générés doivent être versionnés pour le travail à deux.

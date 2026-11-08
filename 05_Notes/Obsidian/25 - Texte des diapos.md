---
tags: [vide-cave, oral, diapo]
date: 2026-11-08
---
# Texte des diapos

Retour : [[00 - Accueil]] · Ce que je dis à l'oral : [[22 - Soutenance - Consignes et texte]]

Le texte ci-dessous est à copier dans les diapos. Les crochets sont à compléter.

## Titre

**Surveillance d'une pompe de vide-cave par mesure du courant**

- Rafaël [NOM] et Jules [NOM]
- BUT GEII 2, SAé 31/42, groupe 311
- Encadrant : [nom]
- IUT Lyon 1, département GEII, 2026-2027

## Plan

1. Le besoin et le cahier des charges
2. Le synoptique et les composants
3. Les essais et le code
4. Le bilan : fait, reste à faire, retards
5. Les améliorations et la conclusion

## Le problème

- Une pompe de vide-cave peut rester longtemps à l'arrêt.
- Elle est dans une cave, on ne va pas la vérifier tous les jours.
- Si elle est bloquée, on le découvre quand la cave est inondée.

**Objectif : connaître l'état du moteur sans descendre à la cave, en mesurant son courant.**

## Synthèse du cahier des charges

**Le système doit :**

- mesurer le courant du moteur ;
- reconnaître 3 états : en marche, bloqué, à vide ;
- laisser passer le pic de courant du démarrage ;
- couper le moteur en cas de défaut ;
- faire démarrer la pompe tous les 3 jours ;
- afficher l'état sur un LCD, avec une LED.

**Les contraintes :**

- pompe en 230 V, partie commande en 5 V : les deux doivent être isolées ;
- usage domestique, dans une maison individuelle ;
- utiliser les composants fournis par les profs.

**Ce qui n'est pas dans le projet :** la protection en amont (disjoncteur), d'après le professeur.

**Pour plus tard :** une alerte par mail avec un ESP32.

## Le principe

| Courant | État du moteur | Ce que fait le système |
|---|---|---|
| Trop fort pendant 10 s | Bloqué | Coupe et affiche BLOQUE |
| Normal | En marche | Laisse tourner |
| Trop faible pendant 10 s | À vide | Coupe et affiche A VIDE |
| Fort pendant 5 s au démarrage | Démarrage normal | Attend |

## Synoptique simple

**Secteur 230 V → Triac → Moteur → Capteur de courant → PIC → LCD et LED**

Et le PIC commande le triac par l'optotriac.

## Synoptique détaillé : les signaux

**Partie 230 V**

| Signal | De | Vers | Ce que c'est |
|---|---|---|---|
| PHASE, NEUTRE | Secteur | BTA12, moteur | 230 V alternatif |
| I_MOTEUR | Moteur | ACS756 (IP+, IP−) | Courant de la pompe |
| GACHETTE | MOC3023 (4 et 6) | BTA12 (G), par R3 | Déclenche le triac |

**Passage entre le 230 V et le 5 V, sans contact électrique**

| Composant | Il isole par | Sens |
|---|---|---|
| ACS756 | Effet Hall | Du 230 V vers le 5 V |
| MOC3023 | Lumière | Du 5 V vers le 230 V |
| 6N136 | Lumière | Du 230 V vers le 5 V |

**Partie 5 V**

| Signal | De | Vers | Ce que c'est |
|---|---|---|---|
| AN0 | ACS756 (VIOUT) | PIC, RA0, broche 2 | Tension : 2,5 V + 20 mV/A |
| RB2 | PIC, broche 23 | MOC3023 (1), par R7 | Commande du moteur : 0 ou 5 V |
| ZERO | 6N136 (6) | PIC, broche à choisir | Passage par zéro de la tension |
| LCD_RS, LCD_RW, LCD_E | PIC, RA1, RA2, RA3 | LCD | Commande du LCD |
| LCD_D4 à LCD_D7 | PIC, RA4, RA5, RA7, RA6 | LCD | Données en mode 4 bits |
| RB0 | PIC, broche 21 | LED D4, par R10 | État du moteur |
| TX, RX | PIC, RC6, RC7 | J2 | Liaison série vers RealTerm |
| ICSPCLK, ICSPDAT, MCLR | PIC, RB6, RB7, MCLR | J4 | Programmation avec le SNAP |
| +5V, GND | J1 | Toute la carte | Alimentation |

## Les composants

Ils nous ont été donnés par les profs.

| Composant | Référence | Rôle |
|---|---|---|
| Microcontrôleur | PIC16F1788 | Lit le courant, décide, commande |
| Capteur de courant | ACS756 | Courant → tension, 20 mV/A autour de 2,5 V |
| Triac | BTA12-600 | Laisse passer ou coupe le 230 V |
| Optotriac | MOC3023 | Déclenche le triac, isole le PIC |
| Optocoupleur | 6N136 | Repère le passage par zéro |
| Afficheur | LCD 16 × 2 (162B) | Affiche l'état du moteur |

Images : dossier `Soutenance vide-cave/Photos composants` du Bureau.

## Ce qui a changé depuis la pré-étude

- Relais → triac et optotriac
- ESP32 → PIC16F1788 (l'ESP32 reste pour plus tard)
- Ajout de l'optocoupleur 6N136
- Plus de flotteur : le courant indique s'il y a de l'eau
- Carte de la SAE22 reprise et modifiée

## La lecture analogique

- Le CAN transforme une tension en nombre.
- 8 bits : de 0 à 255, q = 5 V / 256 = 19,5 mV
- 12 bits : de 0 à 4095, q = 5 V / 4096 = 1,22 mV
- 2,5 V donne 128 en 8 bits, 2048 en 12 bits.

## Essais réalisés

**Essai 1 : lecture analogique, sur la carte SAE22**

- Potentiomètre sur RA0 : plus je tourne, plus la tension et le nombre montent.
- Le PIC envoie le nombre au PC par la liaison série, je le lis sur RealTerm.

| Valeur RealTerm | Sur 8 bits | Tension |
|---|---|---|
| 65520 | 255 | 4,98 V |
| 56368 | 220 | 4,30 V |
| 44592 | 174 | 3,40 V |
| 23792 | 92 | 1,80 V |
| 3312 | 12 | 0,23 V |
| 2544 | 9 | 0,18 V |

**Résultat :** la lecture marche, de 9 à 255. On coupe en 3 zones : seuils à 91 et 173.

**Essai 2 : deux seuils sur des LED**

- Une LED d'un côté du potentiomètre, l'autre LED de l'autre côté.

**Essai 3 : le programme à 4 états**

- [À compléter après le test sur la carte. Sinon, écrire : écrit, pas encore testé.]

**Pas encore testé :** le vrai capteur, le triac et le 230 V.

## Le code : la lecture analogique

```c
#define SEUIL_BAS   91
#define SEUIL_HAUT  173

courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;
```

- On lit la tension sur RA0.
- `>> 8` : on garde une valeur de 0 à 255.
- On compare cette valeur à deux seuils.

## Le code : la décision

```c
if(courant > SEUIL_HAUT || courant < SEUIL_BAS)
{
    if(compteur >= 100)         // depuis 10 s
    {
        OPTO_TRIAC_SetLow();    // on coupe le moteur
    }
}
else
{
    compteur = 0;
}
```

- Courant anormal pendant 10 s : on coupe.
- Courant normal : le compteur repart de zéro.
- Un pic isolé ne coupe donc pas la pompe.

## Le code : le temps

```c
void IT_Timer2(void)      // appelée toutes les 10 ms
{
    nb_10ms = nb_10ms + 1;
    if(nb_10ms >= 10)     // 10 x 10 ms = 100 ms
    {
        nb_10ms = 0;
        top_100ms = 1;
    }
}
```

- Le Timer2, réglé dans MCC, déclenche une interruption toutes les 10 ms.
- Toutes les 100 ms, le `main` mesure et décide.

## Le code : les états

**ARRET → DEMARRAGE → MARCHE → DEFAUT**

| État | Ce que fait le programme |
|---|---|
| ARRET | Moteur coupé, démarre au bout de 2 s pour le test |
| DEMARRAGE | Attend 5 s : le pic de courant est normal |
| MARCHE | Compare le courant aux seuils |
| DEFAUT | Moteur coupé, affiche BLOQUE ou A VIDE |

## Ce qui est fait, ce qui reste à faire

| Bloc | Fait | Reste à faire |
|---|---|---|
| Cahier des charges, pré-étude | Terminé | |
| Composants | Choisis, nomenclature écrite | Fixer les dernières résistances |
| Mesure | Lecture testée au potentiomètre | Brancher l'ACS756, passer à 12 bits |
| Commande | Schéma KiCad | Tester le triac et l'optotriac |
| Détection du zéro | 6N136 au schéma | Fixer les valeurs, relier au programme |
| Programme | 4 états et interruption écrits | Vrais seuils, lancement tous les 3 jours |
| Carte | Schéma et typons en cours | Finir, imprimer, souder |
| Dossier | Rapport de conception en cours | Compléter et rendre |

## Les retards

**Pourquoi :**

- On a cherché des composants sur internet alors qu'ils étaient à l'IUT.
- Il a fallu refaire des schémas KiCad quand les composants ont changé.
- Le programmateur SNAP nous a bloqués, il a fallu l'aide des profs.
- On n'avait pas encore vu la lecture analogique en cours.
- Le code et les essais moteur sont en retard sur le planning.

**Comment on rattrape :**

- Avancer bloc par bloc : capteur et triac, puis optocoupleur, puis LED et LCD.
- Jules sur la carte, Rafaël sur le code, en parallèle.
- La date de fin ne change pas.

Montrer ici le Gantt : `Gantt_vide-cave_a_jour.xlsx`.

## Les pistes d'amélioration

**Mesure**

- Un capteur plus petit : celui de 100 A donne peu de signal pour une pompe d'environ 1 A.
- Le réglage du zéro au démarrage : mesurer la tension à 0 A, puis la retirer.
- Le déphasage entre la tension et le courant, pour mieux reconnaître la marche à vide.

**Carte**

- Un dissipateur sur le triac.
- Un fusible temporisé.
- Un boîtier étanche, car une cave peut être humide.

**Programme et utilisation**

- Le courant affiché sur le LCD.
- Un bouton de marche forcée et un bouton de remise à zéro.
- Le buzzer pour les alarmes.

**Communication**

- Un ESP32 pour envoyer une alerte par mail quand le moteur est bloqué.

## Les outils utilisés

| Outil          | Pour quoi faire                                |
| -------------- | ---------------------------------------------- |
| MPLAB X et XC8 | Écrire et compiler le code en C                |
| MCC Classic | Configurer l'horloge, les broches, le CAN et le Timer2 |
| MPLAB SNAP     | Programmer le PIC                              |
| RealTerm       | Lire les valeurs envoyées par la liaison série |
| KiCad          | Faire le schéma et le typon                    |
| draw.io        | Dessiner les synoptiques                       |
| GitHub         | Partager les fichiers à deux                   |
| Obsidian       | Garder nos notes et nos décisions              |
| Google Drive   | Partager les documents et les datasheets       |
| Google Sheets  | Faire le Gantt                                 |
| Kanban         | Suivre les tâches                              |

## Conclusion

**Ce qu'on a :** le cahier des charges, les composants, le schéma en cours, la lecture analogique testée, un programme écrit.

**Ce qu'il reste :** imprimer la carte, tester avec le vrai capteur en 230 V, régler les seuils.

**Ce que j'ai appris :**

- la lecture analogique et MCC ;
- les interruptions dans un vrai projet ;
- comment on commande un moteur 230 V en restant isolé.

**Ce qu'on ferait autrement :** demander tout de suite aux profs les composants disponibles.

**Répartition :** Rafaël : composants, broches du PIC, code. Jules : KiCad, schéma, typons.

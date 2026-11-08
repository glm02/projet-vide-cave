---
tags: [vide-cave, oral, code, memo]
date: 2026-11-08
---
# Fiche oral : expliquer mon code

Retour : [[00 - Accueil]]

À lire ce soir, puis à redire à voix haute sans regarder. Le code à montrer est `main.c`, la version
simplifiée : [[24 - Mon code simple ligne par ligne]].

## 1. Les 3 notions à savoir

### Un bit, 8 bits, 12 bits

Un bit, c'est une case qui vaut 0 ou 1. Plus on a de cases, plus on peut écrire
de nombres différents :

| Nombre de bits | Calcul | Valeurs possibles |
|---|---|---|
| 1 bit | 2¹ | 0 ou 1 |
| 8 bits | 2⁸ = 256 | de 0 à 255 |
| 12 bits | 2¹² = 4096 | de 0 à 4095 |
| 16 bits | 2¹⁶ = 65 536 | de 0 à 65 535 |

> **À dire :** « Plus il y a de bits, plus la mesure est fine. C'est comme une règle :
> 8 bits, c'est une règle avec 256 graduations ; 12 bits, la même règle avec 4096
> graduations. La longueur ne change pas, les graduations sont plus fines. »

Le mot sert à deux choses différentes, il ne faut pas les mélanger :
- le **CAN 12 bits** : il donne un résultat de 0 à 4095 ;
- un **compteur 8 bits** : c'est un Timer qui compte de 0 à 255 au maximum, puis repart de 0.

### `uint8_t`

Ça se lit morceau par morceau :

| Morceau | Veut dire |
|---|---|
| `u` | *unsigned*, sans signe : pas de nombre négatif |
| `int` | entier : pas de virgule |
| `8` | sur 8 bits |
| `_t` | c'est un type |

Donc `uint8_t` = un nombre entier de **0 à 255**. `uint16_t` = de 0 à 65 535.

> **À dire :** « J'utilise `uint8_t` parce que mes valeurs tiennent entre 0 et 255.
> On prend le plus petit type qui suffit : le PIC calcule nativement sur 8 bits, le reste
> est plus lent. »

### La lecture analogique

Le PIC ne comprend que des nombres, mais le capteur donne une tension. Le **CAN**
(convertisseur analogique numérique) fait la traduction : il découpe la plage de 0 à
5 V en petites marches, et il donne le numéro de la marche.

Le **quantum**, c'est la hauteur d'une marche :

| | 8 bits | 12 bits |
|---|---|---|
| Quantum | 5 V / 256 = **19,5 mV** | 5 V / 4096 = **1,22 mV** |
| 2,5 V donne | 128 | 2048 |
| Avec le capteur à 20 mV/A, un point vaut | **≈ 1 A** | **≈ 61 mA** |

> **À dire :** « Le CAN transforme la tension en nombre. Pour le test au potentiomètre,
> 8 bits suffisent. Mais avec le vrai capteur à 20 mV par ampère, sur 8 bits un point vaut
> déjà 1 ampère, et notre pompe consomme à peu près ça. Donc on passera à 12 bits. »

**La formule :** nombre = tension / 5 V × 256 (en 8 bits).
Exemple : 3,38 V / 5 × 256 = 173, c'est mon `SEUIL_HAUT`.

## 2. Mon code

L'explication complète est dans [[24 - Mon code simple ligne par ligne]].

## 3. Les questions pièges

**Pourquoi 8 bits alors que le PIC fait du 12 bits ?**
> Pour le test au potentiomètre, ça suffit et c'est plus simple : 3 zones sur 256 valeurs.
> Avec le vrai capteur, il faudra les 12 bits.

**Pourquoi une interruption et pas un `__delay_ms` ?**
> Pendant un délai, le PIC ne fait rien d'autre. Avec le Timer2 en interruption, il reste libre entre deux mesures.

**Là, tu lis une seule valeur : le courant est alternatif, non ?**
> Oui, et c'est la limite de cette version : une seule mesure tombe n'importe où sur la sinusoïde.
> La prochaine étape, c'est de faire plusieurs mesures sur une période de 20 ms. Le 6N136 sert
> à savoir quand les démarrer.

**Le 6N136, il sert à quoi exactement ?**
> À repérer le passage par zéro de la tension du secteur, en restant isolé. Ça permet de
> déclencher les mesures au bon moment et, plus tard, de mesurer le déphasage entre
> la tension et le courant, utile pour reconnaître la marche à vide.

**Le MOC3023, il commute au zéro ?**
> Non, c'est un modèle *random-phase* : il s'amorce dès qu'on allume sa LED, à n'importe quel
> moment de la sinusoïde. Il tient 400 V crête, et le secteur fait 325 V crête (230 × √2) :
> la marge n'est pas énorme. Il s'amorce avec 5 mA au maximum, on vise environ 10 mA.

**Comment calcules-tu R7 ?**
> (5 V − 1,5 V de LED) / 10 mA = 350 Ω, je prends 330 Ω, soit 10,6 mA. C'est au-dessus des 5 mA
> et une sortie du PIC peut le fournir. Valeur à valider sur la carte.

**Pourquoi la pompe n'est-elle pas encore branchée ?**
> La carte n'est pas encore imprimée. On avance bloc par bloc : d'abord la lecture et le
> code sur la carte SAE22 avec un potentiomètre, ensuite capteur et triac, puis le reste.

**Ton capteur 100 A, ce n'est pas trop ?**
> Si, mais c'est celui que les profs nous ont donné. Pour une pompe d'environ 1 A, il donne seulement
> 20 mV, donc il faudra passer en 12 bits. Un capteur plus petit donnerait plus de signal : c'est une
> amélioration possible.

**C'est quoi MCC ?**
> L'outil de MPLAB qui configure le PIC et génère le code : horloge, broches, CAN, liaison série.
> Je choisis les réglages, il écrit les fonctions, comme `ADC_GetConversion` ou `LED_SetHigh`.

**Qu'est-ce qui marche vraiment aujourd'hui ?**
> La lecture analogique au potentiomètre, avec les valeurs lues sous RealTerm et mises dans
> un tableau, et un programme à deux seuils qui allume des LED. La machine à états est écrite.
> Je n'ai pas encore de résultat avec le vrai capteur et le 230 V.

## 4. Attention

- **Ne pas dire « MOC3041 »** : c'était dans l'ancien diaporama et la note 10. Nous, c'est le MOC3023.
- **Dire 230 V** (pas 130 V).
- **Dire « optotriac » pour le MOC3023** et **« optocoupleur » pour le 6N136**.
- **Réglage dans MCC :** l'horloge du CAN doit être à `FOSC/16` : à 16 MHz, ça donne TAD = 1 µs. `FOSC/2` serait trop rapide.

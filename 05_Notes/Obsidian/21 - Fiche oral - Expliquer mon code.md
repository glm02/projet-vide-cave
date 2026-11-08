---
tags: [vide-cave, oral, code, memo]
date: 2026-11-08
---
# Fiche oral : expliquer mon code

Retour : [[00 - Accueil]]

À lire ce soir, puis à redire à voix haute sans regarder. Le code à montrer est
`main_simple.c`. Le fichier `main.c` ne compile pas (il contient deux `main`).

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

## 2. Mon code, morceau par morceau

### La lecture du courant

```c
uint8_t Lire_Courant(void)
{
    return (uint8_t)(ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8);
}
```

- `uint8_t Lire_Courant(void)` : la fonction ne reçoit rien (`void`) et renvoie un nombre de 0 à 255.
- `ADC_GetConversion(...)` : fonction générée par MCC. Elle choisit l'entrée, attend un peu, lance la conversion, attend la fin et renvoie le résultat.
- `Capteur_COURANT` : le nom que j'ai donné à RA0 dans MCC.
- `negChannel_ADNREF` : la tension de référence basse, ici la masse. La mesure se fait par rapport à 0 V.
- `>> 8` : le CAN mesure sur 12 bits, le résultat est rangé à gauche dans 16 bits. Décaler de 8 vers la droite garde les 8 bits les plus importants.
- `(uint8_t)` : je range le résultat dans 8 bits.

```text
Résultat sur 16 bits :  1000 0000 0000 0000   (2,5 V)
Après >> 8          :   0000 0000 1000 0000   = 128
```

> **À dire :** « Si demain on change de capteur, je n'ai que l'intérieur de cette fonction à modifier. »

### Les noms au début du fichier

```c
#define SEUIL_BAS   91
#define SEUIL_HAUT  173
#define DUREE_10S   100
```

`#define` donne un nom à une valeur. Le compilateur remplace le nom par la valeur. Ça
rend le code lisible et je change une valeur à un seul endroit. 91 et 173 découpent le
potentiomètre en 3 zones : ce sont des seuils de test, à refaire avec le vrai capteur.

```c
#define MOTEUR_ON()  do { OPTO_TRIAC_SetHigh(); LED_SetHigh(); } while(0)
```

`MOTEUR_ON()` met RB2 à 1 (donc l'optotriac, donc le triac) et allume la LED. La LED me
permet de voir la commande sans avoir de 230 V. `do { } while(0)` permet juste d'utiliser
la macro comme une instruction normale, avec ses deux actions groupées.

### Le programme principal

```c
SYSTEM_Initialize();   // MCC : horloge, broches, CAN
LCD_Initialize();      // LCD
MOTEUR_OFF();          // sécurité : moteur coupé au démarrage

while (1) // ne se termine jamais
{
    courant = Lire_Courant();
    switch (etat) { ... }
    __delay_ms(100);
}
```

Toutes les 100 ms : je lis le courant, je regarde dans quel état je suis, j'agis.

### La machine à états

Une variable `etat` retient où on en est. `switch (etat)` exécute le `case` qui
correspond. `break` sort du `switch`.

| État | Ce que fait le moteur | Comment on en sort |
|---|---|---|
| `ETAT_ARRET` | Coupé | Après 2 s (pour le test) → DEMARRAGE |
| `ETAT_DEMARRAGE` | En marche | À 3 s si le courant est redescendu → MARCHE ; à 5 s s'il est encore trop fort → BLOQUE |
| `ETAT_MARCHE` | En marche | 10 s trop fort → BLOQUE ; 10 s trop faible → A_VIDE |
| `ETAT_BLOQUE` | Coupé | Seulement par le bouton reset |
| `ETAT_A_VIDE` | Coupé | Seulement par le bouton reset |

**Le calcul des durées :** la boucle dure 100 ms. 100 tours × 100 ms = 10 s. 30 tours = 3 s. 50 tours = 5 s.

```c
if (courant > SEUIL_HAUT)
{
    compteur_haut = compteur_haut + 1;   // un tour de plus avec un courant trop fort
    compteur_bas = 0;
}
```

> **À dire :** « Je ne coupe pas au premier dépassement. Je compte combien de temps ça dure.
> Comme ça, un pic isolé ne coupe pas la pompe. Si le courant redevient normal, je
> remets le compteur à zéro. »

### L'affichage

```c
if (etat != etat_precedent)   // != veut dire « différent de »
{
    Afficher_Etat(etat);
    etat_precedent = etat;
}
```

Je n'écris sur le LCD que si l'état a changé. Sinon l'écran clignote.

`etat_precedent` commence à 255, une valeur impossible : ça force le premier affichage.

## 3. Les questions pièges

**Pourquoi 8 bits alors que le PIC fait du 12 bits ?**
> Pour le test au potentiomètre, ça suffit et c'est plus simple : 3 zones sur 256 valeurs.
> Avec le vrai capteur, il faudra les 12 bits.

**Pourquoi un `__delay_ms(100)` ? Ce n'est pas bloquant ?**
> Si, pendant ces 100 ms le PIC ne fait rien d'autre. Pour ce test, c'est suffisant et simple.
> Pour la suite, je veux passer par une interruption de Timer, pour qu'il puisse faire
> autre chose en même temps.

**Là, tu lis une seule valeur : le courant est alternatif, non ?**
> Oui, et c'est la limite de cette version. Une seule mesure tombe n'importe où sur la sinusoïde.
> C'est pour ça que j'ai écrit un autre programme de test, `main_test_efficace.c` : 100 mesures
> sur une période de 20 ms, une toutes les 200 µs, puis le calcul de la valeur efficace.
> Pour l'instant il tourne en simulation, le PIC fabrique lui-même la sinusoïde. Et c'est
> aussi le rôle du 6N136 : savoir quand démarrer les mesures.

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
> Si. Pour une pompe d'environ 1 A, il donne seulement 20 mV. Passer en 12 bits aide. Un capteur
> plus petit, comme un ACS712 5 A à 185 mV/A, donnerait 9 fois plus de signal : c'est une amélioration possible.

**C'est quoi MCC ?**
> L'outil de MPLAB qui configure le PIC et génère le code : horloge, broches, CAN, liaison série.
> Je choisis les réglages, il écrit les fonctions, comme `ADC_GetConversion` ou `LED_SetHigh`.

**Qu'est-ce qui marche vraiment aujourd'hui ?**
> La lecture analogique au potentiomètre, avec les valeurs lues sous RealTerm et mises dans
> un tableau, et un programme à deux seuils qui allume des LED. La machine à états est écrite.
> Je n'ai pas encore de résultat avec le vrai capteur et le 230 V.

## 4. Attention

- **Ne pas dire « MOC3041 » ni « Timer2 toutes les 10 ms »** : c'était dans l'ancien diaporama et la note 10, ce n'est pas ce qu'on a.
- **Dire 230 V** (pas 130 V).
- **Dire « optotriac » pour le MOC3023** et **« optocoupleur » pour le 6N136**.
- **Si le jury veut voir le projet dans MPLAB :** le projet compile `main.c`, qui contient deux `main` et ne compile pas. Montrer `main_simple.c` dans l'éditeur, ou enlever le premier programme de `main.c` avant.
- **Réglage à vérifier dans MCC :** le fichier généré `adc.c` indique une horloge du CAN à `FOSC/2`, alors que ton commentaire demande `FOSC/16`. À 16 MHz, `FOSC/16` donne TAD = 1 µs, le minimum recommandé ; `FOSC/2` donne 0,125 µs, c'est trop rapide et la mesure peut être moins précise.

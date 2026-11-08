---
tags: [vide-cave, oral, code]
date: 2026-11-08
---
# Mon code simple, ligne par ligne

Retour : [[00 - Accueil]] · Notions : [[21 - Fiche oral - Expliquer mon code]]

Le code est dans `Projet_vide_cave_1.X/main.c`. L'ancien est gardé dans
`main_avant_simplification.c.txt`.

**En une phrase :** toutes les 100 ms, le PIC lit le courant, regarde dans quel état il est, et
décide s'il faut changer d'état.

## 1. Les noms au début

```c
#define SEUIL_BAS   91
#define SEUIL_HAUT  173

#define DUREE_ARRET      20    // 20 x 100 ms = 2 s
#define DUREE_DEMARRAGE  50    // 50 x 100 ms = 5 s
#define DUREE_DEFAUT     100   // 100 x 100 ms = 10 s

#define ARRET      0
#define DEMARRAGE  1
#define MARCHE     2
#define BLOQUE     3
#define A_VIDE     4
```

`#define` donne un nom à un nombre. C'est plus lisible, et je change la valeur à un seul endroit.

- **Les seuils** viennent des mesures RealTerm : voir [[23 - Mesures RealTerm expliquees]].
- **Les durées** sont en tours de boucle. Un tour dure 100 ms.
- **Les états** sont juste des numéros avec un nom.

## 2. Les variables

```c
uint8_t etat = ARRET;
uint8_t etat_affiche = ARRET;
uint8_t compteur = 0;
uint8_t courant;
```

| Variable | Elle retient |
|---|---|
| `etat` | où en est le moteur |
| `etat_affiche` | ce qui est écrit sur le LCD |
| `compteur` | le nombre de tours passés |
| `courant` | la dernière mesure, de 0 à 255 |

`uint8_t` : un nombre entier de 0 à 255. `DUREE_DEFAUT` vaut 100, ça rentre.

## 3. Le démarrage du PIC

```c
SYSTEM_Initialize();
LCD_Initialize();
Moteur_Arret();
Afficher_Etat(etat);
```

Ça ne s'exécute qu'une fois. `SYSTEM_Initialize` vient de MCC : horloge, broches et CAN. Ensuite je
coupe le moteur, par sécurité, et j'affiche l'état.

## 4. La boucle

```c
while(1) // ne se termine jamais
{
    courant = Lire_Courant();
    switch(etat) { ... }
    if(etat != etat_affiche) { ... }
    __delay_ms(100);
}
```

1. Je lis le courant.
2. Je fais ce que demande l'état actuel.
3. J'écris sur le LCD si l'état a changé. `!=` veut dire « différent de ». Si on écrivait tout le temps, l'écran clignoterait.
4. J'attends 100 ms, puis je recommence.

## 5. Le `switch`

`switch(etat)` va directement au `case` qui correspond à l'état. `break` sort du `switch`.

### ARRET

```c
case ARRET:
    compteur = compteur + 1;
    if(compteur >= DUREE_ARRET)
    {
        compteur = 0;
        Moteur_Marche();
        etat = DEMARRAGE;
    }
    break;
```

Je compte les tours. Après 2 s, je démarre le moteur. Le 2 s, c'est pour le test : à la fin, ce sera tous les 3 jours.

### DEMARRAGE

```c
case DEMARRAGE:
    compteur = compteur + 1;
    if(compteur >= DUREE_DEMARRAGE)
    {
        compteur = 0;
        if(courant > SEUIL_HAUT)
        {
            Moteur_Arret();
            etat = BLOQUE;
        }
        else
        {
            etat = MARCHE;
        }
    }
    break;
```

Au démarrage, le moteur tire beaucoup de courant, c'est normal. J'attends donc 5 s avant de regarder.
Si c'est encore trop fort, le moteur est bloqué. Sinon, il est en marche.

### MARCHE

```c
case MARCHE:
    if(courant > SEUIL_HAUT)
    {
        compteur = compteur + 1;
        if(compteur >= DUREE_DEFAUT)
        {
            Moteur_Arret();
            etat = BLOQUE;
        }
    }
    else if(courant < SEUIL_BAS)
    {
        compteur = compteur + 1;
        if(compteur >= DUREE_DEFAUT)
        {
            Moteur_Arret();
            etat = A_VIDE;
        }
    }
    else
    {
        compteur = 0;
    }
    break;
```

- Courant trop fort ou trop faible : je compte.
- Après 100 tours, soit 10 s de suite : je coupe.
- Courant normal : je remets le compteur à zéro.

Comme ça, un pic de courant isolé ne coupe pas la pompe.

### BLOQUE et A_VIDE

```c
case BLOQUE:
    break;
```

Il n'y a rien à faire : le moteur est déjà coupé. On n'en sort que par le bouton reset.

## 6. Les fonctions

### Le moteur

```c
void Moteur_Marche(void)
{
    OPTO_TRIAC_SetHigh();
    LED_SetHigh();
}
```

`OPTO_TRIAC_SetHigh()` est généré par MCC : RB2 passe à 1. La LED montre la commande sans 230 V.
`void` veut dire que la fonction ne reçoit rien et ne renvoie rien.

### La mesure

```c
uint8_t Lire_Courant(void)
{
    uint16_t mesure;

    mesure = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF);
    return (uint8_t)(mesure >> 8);
}
```

1. `ADC_GetConversion` vient de MCC. Elle lance la conversion sur RA0 et renvoie le résultat sur 16 bits.
2. `>> 8` décale de 8 bits vers la droite, comme une division par 256. Il reste une valeur de 0 à 255.
3. `(uint8_t)` dit qu'on range ce résultat sur 8 bits.

### L'affichage

`Afficher_Etat` efface le LCD, écrit « Etat moteur : » sur la ligne 1, puis le nom de l'état sur la ligne 2.

## 7. Comment le tester sur la carte

Dans MPLAB : **Clean and Build**, puis programmer le PIC. Ensuite, avec le potentiomètre :

| Potentiomètre | Ce que tu dois voir |
|---|---|
| Au milieu | Arret, puis Demarrage après 2 s, puis En marche 5 s plus tard, et la LED allumée |
| Au milieu, puis en haut pendant 10 s | BLOQUE - coupe, et la LED éteinte |
| Bouton reset, puis au milieu, puis en bas pendant 10 s | A VIDE - coupe, et la LED éteinte |
| En haut dès le départ | BLOQUE - coupe après le démarrage |

## 8. Si le jury demande

**Pourquoi un `uint16_t` dans `Lire_Courant` ?**
> Parce que le résultat du CAN est rangé sur 16 bits. Ensuite, je ne garde que 8 bits.

**Pourquoi un seul compteur ?**
> À chaque changement d'état, je le remets à zéro. Il sert donc à compter le temps dans l'état en cours.

**Pourquoi des fonctions ?**
> Chaque fonction a un rôle, et le `main` reste lisible. Avec le vrai capteur, je n'aurai que `Lire_Courant` à modifier.

**Ton programme compte 3 jours ?**
> Pas encore. Là, il attend 2 s pour le test. Pour 3 jours, il faudra une base de temps avec un Timer.

**Le `__delay_ms`, ça ne bloque pas le PIC ?**
> Si, pendant 100 ms il ne fait rien d'autre. C'est simple et ça suffit pour le test, mais pour la suite on veut passer par un Timer.

**Il y a de l'ESP32, un flotteur, le passage par zéro dans ce code ?**
> Non. Ce programme teste la logique avec le potentiomètre. Le reste viendra une fois la carte fabriquée.

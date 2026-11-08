---
tags: [vide-cave, oral, code]
date: 2026-11-08
---
# Mon code simple, ligne par ligne

Retour : [[00 - Accueil]] · Notions : [[21 - Fiche oral - Expliquer mon code]]

Le code est dans `Projet_vide_cave_1.X/main.c`.

**En une phrase :** le Timer2 déclenche une interruption toutes les 10 ms. Toutes les 100 ms, le
programme lit le courant et décide quoi faire du moteur selon son état.

## 1. Les noms au début

```c
#define SEUIL_BAS   91
#define SEUIL_HAUT  173

#define DUREE_ARRET      20    // 2 s
#define DUREE_DEMARRAGE  50    // 5 s
#define DUREE_DEFAUT     100   // 10 s

#define ARRET      0
#define DEMARRAGE  1
#define MARCHE     2
#define DEFAUT     3
```

`#define` donne un nom à un nombre.

- **Les seuils** viennent des mesures RealTerm : voir [[23 - Mesures RealTerm expliquees]].
- **Les durées** se comptent en fois 100 ms : 20 × 100 ms = 2 s.
- **Les états** sont des numéros avec un nom.

## 2. Les variables

```c
volatile uint8_t top_100ms = 0;   // globale, partagée avec l'interruption

uint8_t etat = ARRET;             // l'état du moteur
uint8_t compteur = 0;             // le temps passé, en fois 100 ms
uint16_t courant;                 // la mesure
```

- **`uint8_t`** : nombre entier de 0 à 255. Il suffit pour `etat` et `compteur`.
- **`uint16_t`** : nombre entier sur 16 bits. Le CAN renvoie son résultat sur 16 bits.
- **`volatile`** : la variable change dans l'interruption. Le mot dit au compilateur de la relire à chaque fois.

## 3. Le démarrage du PIC

```c
SYSTEM_Initialize();
LCD_Initialize();
Moteur_Arret();
LCD_WriteText("Arret");
```

`SYSTEM_Initialize` vient de MCC : horloge, broches et CAN. Je coupe le moteur par sécurité.

## 4. Le réglage du Timer2

```c
PR2 = 249;
T2CON = 0b01001110;   // postscaler 1:10, Timer2 ON, prescaler 1:16
```

**Le calcul :** FOSC = 16 MHz, donc TCY = 4 / FOSC = 0,25 µs.

0,25 µs × 16 (prescaler) × 250 (PR2 + 1) × 10 (postscaler) = **10 ms**

```c
TMR2IF = 0;   // le drapeau part de 0
TMR2IE = 1;   // j'autorise l'interruption du Timer2
PEIE = 1;     // le Timer2 est un périphérique
GIE = 1;      // j'autorise les interruptions, en dernier
```

`GIE` vient en dernier, une fois que tout est initialisé.

## 5. L'interruption

```c
void __interrupt() int_handler(void)
{
    static uint8_t nb_10ms = 0;

    if(TMR2IF == 1)
    {
        TMR2IF = 0;               // RAZ en premier
        nb_10ms = nb_10ms + 1;
        if(nb_10ms >= 10)         // 10 x 10 ms = 100 ms
        {
            nb_10ms = 0;
            top_100ms = 1;
        }
    }
}
```

1. Le PIC arrête ce qu'il fait et vient ici toutes les 10 ms.
2. Je vérifie que c'est bien le Timer2, puis je remets le drapeau à 0. Sinon le PIC revient ici sans arrêt.
3. `static` : `nb_10ms` garde sa valeur d'un appel à l'autre.
4. Au bout de 10 fois, je mets `top_100ms` à 1 pour prévenir le `main`.

L'interruption est courte : elle ne lit rien et n'écrit pas sur le LCD. Le `main` fait le travail.

## 6. La boucle

```c
while(1) // ne se termine jamais
{
    if(top_100ms == 1)
    {
        top_100ms = 0;
        courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;
        compteur = compteur + 1;
        switch(etat) { ... }
    }
}
```

- **`top_100ms`** : je le remets à 0 pour attendre les 100 ms suivantes.
- **`ADC_GetConversion`** vient de MCC. Elle lit la tension sur RA0.
- **`>> 8`** garde les 8 bits de gauche, comme une division par 256. Il reste une valeur de 0 à 255.
- **`compteur`** compte le temps passé dans l'état, il augmente toutes les 100 ms.

Le PIC ne reste pas bloqué à attendre : il pourrait faire autre chose dans le `while(1)`.

## 7. Le `switch`

`switch(etat)` va au `case` de l'état actuel. `break` sort du `switch`.

### ARRET

```c
case ARRET:
    if(compteur >= DUREE_ARRET)
    {
        compteur = 0;
        Moteur_Marche();
        LCD_Clear();
        LCD_WriteText("Demarrage");
        etat = DEMARRAGE;
    }
    break;
```

Après 2 s, je démarre le moteur et je change d'état. Le 2 s, c'est pour le test : à la fin, ce sera 3 jours.

### DEMARRAGE

```c
case DEMARRAGE:
    if(compteur >= DUREE_DEMARRAGE)
    {
        compteur = 0;
        if(courant > SEUIL_HAUT)
        {
            Moteur_Arret();
            LCD_Clear();
            LCD_WriteText("BLOQUE");
            etat = DEFAUT;
        }
        else
        {
            LCD_Clear();
            LCD_WriteText("En marche");
            etat = MARCHE;
        }
    }
    break;
```

Au démarrage, le moteur tire beaucoup de courant, c'est normal. J'attends 5 s avant de regarder. Si
c'est encore trop fort, il est bloqué. Sinon, il est en marche.

### MARCHE

```c
case MARCHE:
    if((courant >= SEUIL_BAS) && (courant <= SEUIL_HAUT))
    {
        compteur = 0;
    }
    else if(compteur >= DUREE_DEFAUT)
    {
        Moteur_Arret();
        LCD_Clear();
        if(courant > SEUIL_HAUT)
        {
            LCD_WriteText("BLOQUE");
        }
        else
        {
            LCD_WriteText("A VIDE");
        }
        etat = DEFAUT;
    }
    break;
```

- **Courant normal :** je remets le compteur à zéro.
- **Courant anormal :** le compteur continue. Au bout de 10 s, je coupe et j'affiche la cause.

Un pic isolé ne coupe donc pas la pompe, car le compteur repart de zéro dès que le courant redevient normal.

### DEFAUT

```c
case DEFAUT:
    break;
```

Le moteur est déjà coupé. On n'en sort que par le bouton reset.

**Pourquoi le LCD est écrit dans les `case` :** on écrit seulement quand l'état change. Comme ça, l'écran ne clignote pas.

## 8. Les fonctions du moteur

```c
void Moteur_Marche(void)
{
    OPTO_TRIAC_SetHigh();   // RB2 à 1 : l'optotriac déclenche le triac
    LED_SetHigh();          // RB0 à 1 : la LED s'allume
}
```

Ces macros viennent de MCC. La LED montre la commande sans 230 V. `Moteur_Arret` fait l'inverse.

## 9. Comment le tester sur la carte

Dans MPLAB : **Clean and Build**, puis programmer le PIC. Ensuite, avec le potentiomètre :

| Potentiomètre | Ce que tu dois voir |
|---|---|
| Au milieu | Arret, Demarrage après 2 s, En marche 5 s plus tard, LED allumée |
| Au milieu, puis en haut pendant 10 s | BLOQUE, LED éteinte |
| Bouton reset, au milieu, puis en bas pendant 10 s | A VIDE, LED éteinte |
| En haut dès le départ | BLOQUE après le démarrage |

## 10. Si le jury demande

**Pourquoi une interruption plutôt qu'un `__delay_ms` ?**
> Pendant un délai, le PIC ne fait rien d'autre. Avec le Timer2 en interruption, il est libre entre deux
> mesures, et le temps reste précis.

**Pourquoi `PEIE = 1` ?**
> Le Timer2 est un périphérique. Il faut `TMR2IE` pour le Timer2, `PEIE` pour les périphériques et `GIE` pour tout.

**Pourquoi remettre `TMR2IF` à 0 ?**
> Sinon, le PIC repart dans l'interruption dès qu'il en sort, et le programme se bloque.

**Pourquoi l'interruption est-elle si courte ?**
> Elle coupe le programme principal. Elle prévient juste le `main`, qui fait le travail.

**Pourquoi `volatile` ?**
> `top_100ms` est modifiée dans l'interruption et lue dans le `main`. Le mot oblige à la relire à chaque fois.

**Pourquoi pas de section critique ?**
> `top_100ms` fait 8 bits : le PIC la lit en une seule fois.

**Pourquoi 10 ms et pas directement 100 ms ?**
> Le Timer2 n'y arrive pas. Au maximum, 0,25 µs × 64 × 256 × 16 ≈ 65 ms. Donc je compte 10 fois 10 ms.

**Pourquoi le Timer2 n'est pas configuré dans MCC ?**
> Je l'ai réglé directement avec les registres, comme en cours. MCC configure l'horloge, les broches et le CAN.

**Ton programme compte 3 jours ?**
> Pas encore, il attend 2 s pour le test. Avec le même `top_100ms`, je pourrai compter les secondes, les minutes et les heures jusqu'à 72.

**Tu n'as plus de fonction `Lire_Courant` ?**
> Non, c'est une seule ligne. Avec le vrai capteur, c'est cette ligne que je changerai.

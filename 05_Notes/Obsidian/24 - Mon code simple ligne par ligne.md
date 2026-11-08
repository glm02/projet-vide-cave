---
tags: [vide-cave, oral, code]
date: 2026-11-08
---
# Mon code simple

Retour : [[00 - Accueil]] · Mesures : [[23 - Mesures RealTerm expliquees]]

**En une phrase :** toutes les 100 ms, le programme lit le courant, le compare à deux seuils et décide s'il coupe le moteur.

Le code est dans `Projet_vide_cave_1.X` :

- `main_MCC_Classic.c` : le Timer2 est réglé dans MCC. C'est la version à utiliser une fois le projet généré en Classic. **Elle n'a pas encore été compilée avec les vrais fichiers de MCC.**
- `main.c` : le même programme, avec le Timer2 réglé dans le code. Il compile avec le projet actuel, mais il est un peu plus long.

## 1. La lecture analogique, avec mon potentiomètre

**L'idée :** le potentiomètre donne une tension entre 0 et 5 V. Le PIC ne comprend que des nombres. Le CAN transforme donc la tension en nombre.

**Plus je tourne le potentiomètre, plus la tension monte, plus le nombre monte.**

| Position du potentiomètre | Tension | Nombre sur 8 bits |
|---|---|---|
| Au minimum | 0,18 V | 9 |
| Vers le milieu | 1,80 V | 92 |
| Plus haut | 3,40 V | 174 |
| Au maximum | 4,98 V | 255 |

**Comment j'ai relevé les valeurs :**

1. Le PIC lit RA0 avec `ADC_GetConversion`.
2. Il envoie le nombre au PC par la liaison série, avec `printf`.
3. Je le lis sur RealTerm, je tourne le potentiomètre et je note les valeurs.

```c
valeur = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF);
printf("CAN=%u\r\n", valeur);
```

**Ce que je choisis ensuite :** la valeur va de 9 à 255. Je coupe en 3 zones égales, ce qui donne mes seuils : **91** et **173**.

**Pas besoin d'expliquer l'intérieur de `ADC_GetConversion`** : c'est une fonction générée par MCC. Je sais qu'elle lit la tension et qu'elle me renvoie un nombre.

## 2. Les 4 petits morceaux à montrer

### Les seuils

```c
#define SEUIL_BAS   91
#define SEUIL_HAUT  173
```

> Je donne un nom à mes deux seuils. Je les ai trouvés avec le potentiomètre, et je les changerai avec le vrai capteur.

### La lecture

```c
Courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;
```

> Je lis la tension sur RA0. Le `>> 8` divise par 256 : j'ai une valeur de 0 à 255.

### La décision

```c
if(Courant >= SEUIL_BAS && Courant <= SEUIL_HAUT)    // courant normal
{
    Compteur = 0;
}
else if(Compteur >= 100)    // anormal depuis 10 s
{
    OPTO_TRIAC_SetLow();    // on coupe le moteur
}
```

> Si le courant est normal, je remets le compteur à zéro. Sinon, il continue de monter : au bout de 10 secondes, je coupe. Comme ça, un pic isolé ne coupe pas la pompe.

`&&` veut dire « et ».

### Le temps, avec l'interruption

```c
TMR2_SetInterruptHandler(IT_Timer2);     // avant d'autoriser les IT
INTERRUPT_GlobalInterruptEnable();       // GIE = 1
INTERRUPT_PeripheralInterruptEnable();   // PEIE = 1

void IT_Timer2(void)      // appelée par MCC toutes les 100 ms
{
    Top = 1;
}
```

> Le Timer2 est réglé dans MCC. Toutes les 100 ms, il appelle ma fonction, qui met `Top` à 1. Le `main` voit ce drapeau, le remet à 0, mesure et décide.

## 3. Le programme en entier

```c
// Projet vide-cave - IUT Lyon 1 GEII - version MCC Classic
// Test : le potentiometre sur RA0 remplace le capteur de courant.

#include "mcc_generated_files/mcc.h"
#include "lcd.h"

#define SEUIL_BAS   91
#define SEUIL_HAUT  173

#define DEMARRAGE  0
#define MARCHE     1
#define DEFAUT     2

volatile uint8_t Top = 0;   // mis a 1 toutes les 100 ms par le Timer2

void IT_Timer2(void);

void main(void)
{
    uint8_t Etat = DEMARRAGE;
    uint8_t Compteur = 0;   // temps passe, en fois 100 ms
    uint16_t Courant;

    SYSTEM_Initialize();
    LCD_Initialize();

    // Enregistrement AVANT d'autoriser les IT
    TMR2_SetInterruptHandler(IT_Timer2);

    INTERRUPT_GlobalInterruptEnable();     // GIE = 1
    INTERRUPT_PeripheralInterruptEnable(); // PEIE = 1

    OPTO_TRIAC_SetHigh();   // on demarre le moteur
    LCD_WriteText("Demarrage");

    while(1) // ne se termine jamais
    {
        if(Top == 1)        // toutes les 100 ms
        {
            Top = 0;
            Compteur = Compteur + 1;
            Courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;

            if(Etat == DEMARRAGE && Compteur >= 50)    // 5 s : le pic de courant est passe
            {
                Compteur = 0;
                LCD_Clear();
                LCD_WriteText("En marche");
                Etat = MARCHE;
            }
            else if(Etat == MARCHE)
            {
                if(Courant >= SEUIL_BAS && Courant <= SEUIL_HAUT)    // courant normal
                {
                    Compteur = 0;
                }
                else if(Compteur >= 100)    // anormal depuis 10 s
                {
                    OPTO_TRIAC_SetLow();    // on coupe le moteur
                    LCD_Clear();
                    if(Courant > SEUIL_HAUT)
                    {
                        LCD_WriteText("BLOQUE");
                    }
                    else
                    {
                        LCD_WriteText("A VIDE");
                    }
                    Etat = DEFAUT;          // on n'en sort que par le bouton reset
                }
            }
        }
    }
}

// Appelee par MCC toutes les 100 ms
void IT_Timer2(void)
{
    Top = 1;
}
```

**Les 3 états :**

| État | Ce qu'il fait | Quand il change |
|---|---|---|
| `DEMARRAGE` | Moteur en marche, attend | Après 5 s : le pic de courant est passé |
| `MARCHE` | Compare le courant aux seuils | Anormal pendant 10 s : coupe le moteur |
| `DEFAUT` | Moteur coupé, affiche BLOQUE ou A VIDE | Seulement avec le bouton reset |

**Les durées :** `Compteur` augmente toutes les 100 ms. 50 fois = 5 s, 100 fois = 10 s.

**Les variables :**

| Variable | Type | Pourquoi |
|---|---|---|
| `Etat`, `Compteur` | `uint8_t` | De 0 à 255, ça suffit |
| `Courant` | `uint16_t` | Le CAN renvoie 16 bits |
| `Top` | `volatile uint8_t` | Modifiée dans l'interruption, lue dans le `main` |

**Le réglage du Timer2 dans MCC :** prescaler 1:16, postscaler 1:10, période 10 ms, interruption activée, **Callback Function Rate à 10** : la fonction est appelée toutes les 10 × 10 ms = 100 ms.

Le calcul de la période : 0,25 µs × 16 × 250 × 10 = 10 ms.

## 4. Comment le tester sur la carte

Dans MPLAB : **Clean and Build**, puis programmer le PIC. Le moteur est commandé dès le démarrage, donc **pas de 230 V pour ce test**. Avec le potentiomètre :

| Potentiomètre | Ce que tu dois voir |
|---|---|
| Au milieu | Demarrage, puis En marche après 5 s |
| Puis en haut pendant 10 s | BLOQUE |
| Bouton reset, au milieu, puis en bas pendant 10 s | A VIDE |

## 5. Si le jury demande

**C'est quoi le CAN ?**
> Le convertisseur analogique numérique. Il transforme une tension en nombre.

**Le CAN fait combien de bits ?**
> 12 bits sur ce PIC. Je garde les 8 bits les plus importants avec le `>> 8`, ça suffit pour le test au potentiomètre.

**Et avec le vrai capteur ?**
> Il donne 20 mV par ampère. En 8 bits, un point vaut 19,5 mV, soit environ 1 A : c'est trop juste. Il faudra passer en 12 bits.

**Pourquoi une interruption ?**
> Pour que le PIC ne reste pas bloqué dans une attente. Il mesure toutes les 100 ms et reste libre entre deux.

**Pourquoi l'interruption ne fait qu'une ligne ?**
> Une interruption doit être courte. Elle prévient le `main`, qui fait le travail.

**Pourquoi `volatile` ?**
> `Top` change dans l'interruption. Le mot oblige le programme à la relire à chaque fois.

**Qu'est-ce que tu as réglé dans MCC ?**
> L'horloge, les broches, le CAN et le Timer2. Moi, j'écris la logique du programme.

**Ton programme compte 3 jours ?**
> Pas encore, il démarre tout de suite pour le test. Il faudra ajouter un état d'arrêt et compter les heures jusqu'à 72.

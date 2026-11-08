---
tags: [vide-cave, oral, code]
date: 2026-11-08
---
# Mon code simple

Retour : [[00 - Accueil]] · Mesures : [[23 - Mesures RealTerm expliquees]]

**En une phrase :** toutes les 100 ms, le programme lit le courant, le compare à deux seuils et décide s'il coupe le moteur.

Le code est dans `Projet_vide_cave_1.X` :

- `main_MCC_Classic.c` : le Timer2 est réglé dans MCC. C'est la version à utiliser une fois le projet généré en Classic. **Elle n'a pas encore été compilée avec les vrais fichiers de MCC.**
- `main.c` : le même programme, avec le Timer2 réglé dans le code. Il compile avec le projet actuel.

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
courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;
```

> Je lis la tension sur RA0. Le `>> 8` divise par 256 : j'ai une valeur de 0 à 255.

### La décision

```c
if(courant > SEUIL_HAUT || courant < SEUIL_BAS)   // courant anormal
{
    if(compteur >= 100)                           // depuis 10 s
    {
        OPTO_TRIAC_SetLow();                      // on coupe le moteur
    }
}
else                                              // courant normal
{
    compteur = 0;
}
```

> Si le courant est trop fort ou trop faible, je laisse le compteur monter. Au bout de 10 secondes, je coupe. S'il redevient normal, je remets à zéro. Comme ça, un pic isolé ne coupe pas la pompe.

`||` veut dire « ou ».

### Le temps, avec l'interruption

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

> Le Timer2 déclenche une interruption toutes les 10 ms. Au bout de 10 fois, je préviens le `main` : c'est le moment de mesurer.

## 3. Comment marche tout le programme

Une variable `etat` retient où en est le moteur.

| État | Ce qu'il fait | Quand il change |
|---|---|---|
| `ARRET` | Moteur coupé | Après 2 s (pour le test) : démarre le moteur |
| `DEMARRAGE` | Moteur en marche | Après 5 s : le pic de courant est passé |
| `MARCHE` | Compare le courant aux seuils | Anormal pendant 10 s : coupe le moteur |
| `DEFAUT` | Moteur coupé, affiche BLOQUE ou A VIDE | Seulement avec le bouton reset |

Chaque état est un simple `if` :

```c
if(etat == ARRET) { ... }
else if(etat == DEMARRAGE) { ... }
else if(etat == MARCHE) { ... }
```

**Les durées :** `compteur` augmente toutes les 100 ms. 20 fois = 2 s, 50 fois = 5 s, 100 fois = 10 s.

**Les variables :**

| Variable | Type | Pourquoi |
|---|---|---|
| `etat`, `compteur`, `nb_10ms` | `uint8_t` | De 0 à 255, ça suffit |
| `courant` | `uint16_t` | Le CAN renvoie 16 bits |
| `top_100ms` | `volatile uint8_t` | Modifiée dans l'interruption, lue dans le `main` |

**Le réglage du Timer2 dans MCC :** prescaler 1:16, postscaler 1:10, période 10 ms, interruption activée.

Le calcul : 0,25 µs × 16 × 250 × 10 = 10 ms.

## 4. Comment le tester sur la carte

Dans MPLAB : **Clean and Build**, puis programmer le PIC. Ensuite, avec le potentiomètre :

| Potentiomètre | Ce que tu dois voir |
|---|---|
| Au milieu | Arret, Demarrage après 2 s, En marche 5 s plus tard, LED allumée |
| Puis en haut pendant 10 s | BLOQUE, LED éteinte |
| Bouton reset, au milieu, puis en bas pendant 10 s | A VIDE, LED éteinte |

## 5. Si le jury demande

**C'est quoi le CAN ?**
> Le convertisseur analogique numérique. Il transforme une tension en nombre.

**Le CAN fait combien de bits ?**
> 12 bits sur ce PIC. Je garde les 8 bits les plus importants avec le `>> 8`, ça suffit pour le test au potentiomètre.

**Et avec le vrai capteur ?**
> Il donne 20 mV par ampère. En 8 bits, un point vaut 19,5 mV, soit environ 1 A : c'est trop juste. Il faudra passer en 12 bits.

**Pourquoi une interruption ?**
> Pour que le PIC ne reste pas bloqué dans une attente. Il mesure toutes les 100 ms et reste libre entre deux.

**Pourquoi `volatile` ?**
> `top_100ms` change dans l'interruption. Le mot oblige le programme à la relire à chaque fois.

**Qu'est-ce que tu as réglé dans MCC ?**
> L'horloge, les broches, le CAN et le Timer2. Moi, j'écris la logique du programme.

**Ton programme compte 3 jours ?**
> Pas encore, il attend 2 s pour le test. Il faudra compter les heures jusqu'à 72.

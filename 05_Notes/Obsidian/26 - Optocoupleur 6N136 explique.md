---
tags: [vide-cave, oral, composants]
date: 2026-11-08
---
# L'optocoupleur 6N136 expliqué

Retour : [[00 - Accueil]] · Code : [[24 - Mon code simple ligne par ligne]]

**En une phrase :** le 6N136 dit au PIC à quel moment la tension du secteur passe par zéro, sans
aucun fil entre le 230 V et le 5 V.

![[Schema 6N136.png]]

L'image est aussi sur le Bureau : `Soutenance vide-cave/Schema_6N136.png`.

## À quoi il sert

Le courant de la pompe est une sinusoïde : il change tout le temps. Si je lis une valeur au hasard,
je peux tomber près du maximum ou près de zéro.

Le 6N136 donne un repère qui revient toutes les 20 ms. Le PIC sait alors quand démarrer ses mesures.
Plus tard, il servira aussi à mesurer le déphasage entre la tension et le courant.

**Ne pas confondre :**

| Composant | Sens | Rôle |
|---|---|---|
| MOC3023, optotriac | Du PIC vers le 230 V | Commander le triac |
| 6N136, optocoupleur | Du 230 V vers le PIC | Repérer le passage par zéro |

## Comment il marche

Dedans, il y a une LED et un transistor, séparés. C'est la lumière qui fait le lien.

1. **La tension est positive :** la LED s'allume, le transistor conduit, RB4 passe à **0 V**.
2. **La tension est négative :** la LED s'éteint, le transistor bloque, la résistance de 22 kΩ ramène RB4 à **5 V**.
3. **Le signal passe de 5 V à 0 V :** la tension vient de passer par zéro en montant. Le PIC a son repère.

## Où il se branche

| Broche du 6N136 | Reliée à | Pourquoi |
|---|---|---|
| 2, anode de la LED | PHASE, par 2 résistances de 47 kΩ 1 W | Limitent le courant de la LED |
| 3, cathode de la LED | NEUTRE | Retour du courant |
| 2 et 3 | Une diode 1N4148 en parallèle, à l'envers | Protège la LED pendant l'alternance négative |
| 8 | +5 V | Alimente la partie transistor |
| 6, sortie | RB4 du PIC, et 22 kΩ vers +5 V | Le signal, et le rappel à 5 V |
| 6 et GND | Condensateur de 1 nF | Filtre les parasites |
| 5 | GND | Masse |
| 1, 4, 7 | Rien | Non utilisées |

**Le calcul :** I crête = 325 V / 94 kΩ ≈ 3,5 mA.
Puissance dans une résistance : (230 V / 94 kΩ)² × 47 kΩ ≈ 0,28 W. Le 1 W laisse de la marge.

**Attention :** le neutre et la masse du PIC ne sont jamais reliés.

## Le code

**Pas encore dans ton `main.c` :** la carte avec le 6N136 n'existe pas encore. Ce code compile, mais il n'a pas été testé.

On utilise une **interruption sur changement d'état (IOC)**, vue en cours.

### 1. La configuration, avant `GIE = 1`

```c
ANSB4 = 0;    // RB4 en numérique
TRISB4 = 1;   // RB4 en entrée
IOCBN4 = 1;   // interruption quand RB4 passe de 1 à 0
IOCBF4 = 0;   // le drapeau part de 0
IOCIE = 1;    // j'autorise les interruptions IOC
```

`IOCBN4` : le N veut dire front négatif, donc de 1 vers 0.

### 2. Dans l'interruption

```c
volatile uint8_t top_zero = 0;

void __interrupt() int_handler(void)
{
    if(IOCBF4 == 1)       // c'est le 6N136
    {
        IOCBF4 = 0;       // RAZ en premier
        top_zero = 1;     // je préviens le main
    }

    if(TMR2IF == 1)       // c'est le Timer2
    {
        // le code d'aujourd'hui, sans changement
    }
}
```

Il n'y a qu'un seul gestionnaire d'interruption. Je regarde donc d'où vient l'interruption avec les drapeaux.

### 3. Dans le `main`

```c
if(top_zero == 1)
{
    top_zero = 0;
    courant_crete = Mesure_Crete();
}
```

### 4. La mesure sur une période

```c
uint16_t Mesure_Crete(void)
{
    uint16_t mesure;
    uint16_t maxi = 0;
    uint8_t i;

    for(i = 0; i < 20; i = i + 1)    // 20 mesures, une par ms, donc 20 ms environ
    {
        mesure = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 4;
        if(mesure > maxi)
        {
            maxi = mesure;
        }
        __delay_ms(1);
    }
    return maxi;
}
```

- **`>> 4`** garde les 12 bits : valeur de 0 à 4095, c'est ce qu'il faut pour le vrai capteur.
- **`maxi`** garde la plus grande valeur vue sur la période, donc le haut de la sinusoïde.
- **Le courant :** à 0 A le capteur donne 2,5 V, soit 2048. Donc `maxi - 2048` donne le courant crête, avec 1 point ≈ 61 mA.

## Ce qu'il reste à faire sur le schéma

Sur le schéma KiCad du 2 novembre :

- [ ] Donner leurs valeurs à R1, R2, R5 et C2 : 47 kΩ 1 W, 47 kΩ 1 W, 22 kΩ et 1 nF.
- [ ] Relier R1 et R2 à la phase et à la broche 2, et la broche 3 au neutre.
- [ ] Ajouter la diode de protection.
- [ ] Relier la broche 8 et R5 au +5 V.
- [ ] Relier la broche 6 à une entrée du PIC.

**À décider avec Jules :** la broche du PIC. J'ai pris RB4 parce qu'elle peut faire de l'IOC, mais sur la carte SAE22 elle sert au bouton BP_2.

**À vérifier :**

- les valeurs de 47 kΩ, 22 kΩ et 1 nF viennent de votre nomenclature, la diode est une proposition ;
- la tension inverse maximale de la LED, dans la datasheet du 6N136, que le dossier n'a pas encore ;
- le signal à l'oscilloscope, d'abord avec une tension réduite ou avec l'aide d'un prof, car c'est du 230 V.

## À dire à l'oral

> On a ajouté un optocoupleur 6N136 pour repérer le passage par zéro de la tension. Côté secteur, sa LED
> s'allume pendant l'alternance positive. Côté 5 V, le transistor passe alors la sortie à 0 V. Il n'y a
> aucun fil entre les deux, c'est la lumière qui fait le lien. Le PIC reçoit ainsi un repère toutes les
> 20 ms, par une interruption, pour lancer ses mesures au bon moment. Il n'est pas encore relié sur le
> schéma final et pas encore testé.

## Si le jury demande

**Pourquoi 2 résistances et pas une seule ?**
> Pour répartir la tension et la chaleur : 325 V crête au total, et environ 0,28 W chacune.

**Pourquoi une diode ?**
> La LED n'accepte qu'une petite tension à l'envers. Pendant l'alternance négative, la diode conduit et la protège.

**Pourquoi seulement un repère toutes les 20 ms, et pas à chaque zéro ?**
> La LED ne s'allume que sur une alternance. C'est plus simple et un repère par période suffit.

**Le repère tombe exactement au zéro ?**
> Non. Il faut un peu de courant pour que le transistor conduise, donc il arrive un peu après. Pour lancer
> une mesure, ce n'est pas gênant. Pour le déphasage, il faudra mesurer ce décalage à l'oscilloscope.

**Pourquoi une résistance de 22 kΩ sur la sortie ?**
> Quand le transistor bloque, la sortie ne serait reliée à rien. La résistance la ramène à 5 V.

**Le déphasage, tu le mesures comment ?**
> Avec le temps entre le zéro de la tension et le zéro du courant : φ = 360° × Δt / 20 ms. Ce n'est pas encore fait.

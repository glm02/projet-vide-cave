---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# `main_simple.c` expliqué bloc par bloc

Retour : [[14 - Memo code - Commencer ici]] · Bases : [[15 - Memo code - Les bases du C]]

## 1. Ce que ce fichier essaie de faire

Il lit un potentiomètre pour représenter une mesure faible, normale ou forte.
Il commande une sortie, montre un état sur le LCD et mémorise les défauts.
C'est un programme de test : les seuils du potentiomètre ne sont pas encore
les seuils mesurés sur une vraie pompe.

```text
Préparer le PIC et le LCD
        ↓
Lire la mesure → Traiter l'état actuel → Afficher si l'état change
        ↑                                         ↓
        └──────────── Attendre 100 ms ─────────────┘
```

## 2. Les trois `#include`

```c
#include <xc.h>
#include "mcc_generated_files/system/system.h"
#include "lcd.h"
```

- `xc.h` donne accès au PIC et aux fonctions propres à XC8.
- `system.h` fournit les déclarations des fonctions MCC utilisées par le projet.
- `lcd.h` annonce les fonctions pour l'écran.

Ils rendent des noms disponibles ; ils ne lancent pas l'initialisation.

## 3. Les commandes `MOTEUR_ON()` et `MOTEUR_OFF()`

```c
#define MOTEUR_ON() do { OPTO_TRIAC_SetHigh(); LED_SetHigh(); } while(0)
```

Ce nom court regroupe deux actions : mettre la commande de l'opto à 1 et mettre
la sortie LED à 1. `MOTEUR_OFF()` met ces deux sorties à 0.

`do { ... } while(0)` fait exécuter le groupe une seule fois. Cela permet
d'utiliser la macro comme une seule instruction, avec un `;` après son appel.
Ce n'est pas la boucle principale du programme.

Dans les fichiers MCC lus, `OPTO_TRIAC` correspond à RB2 et `LED` à RB0.
Une sortie à 1 est une commande électrique : elle ne prouve pas que le moteur
tourne réellement. La réaction dépend aussi du montage.

**Pour simplifier plus tard :** on pourra utiliser de petites fonctions avec
des noms clairs, si cette écriture est plus facile à expliquer.

## 4. Les seuils

```c
#define SEUIL_BAS 91
#define SEUIL_HAUT 173
```

Ils découpent la mesure en trois zones :

| Mesure lue | Branche utilisée dans l'état MARCHE |
|---|---|
| Moins de 91 | Compter un défaut de mesure faible |
| De 91 à 173, bornes comprises | Remettre les compteurs de défaut à zéro |
| Plus de 173 | Compter un défaut de mesure forte |

Ce sont des points numériques après la réduction de la lecture, pas des ampères.
Une mesure égale au seuil ne déclenche donc pas la branche « hors seuil ».

## 5. Les durées

```c
#define DUREE_ATTENTE 20
#define DUREE_DEMAR 30
#define DUREE_MAX_DEM 50
#define DUREE_10S 100
```

Ces nombres comptent des passages dans la boucle. Le programme prévoit environ
100 ms d'attente à chaque passage : 20 passages représentent donc au moins
environ 2 secondes, 30 environ 3 secondes, 50 environ 5 secondes et 100 environ
10 secondes. Le temps des conversions et du LCD s'ajoute.

**À retenir :** `DUREE_10S` ne vaut pas « 100 secondes ». Il vaut 100 passages.
Pour des durées régulières, on reprendra ensuite la gestion du temps avec un timer.

## 6. Les états et leurs numéros

```c
#define ETAT_ARRET 0
#define ETAT_DEMARRAGE 1
#define ETAT_MARCHE 2
#define ETAT_BLOQUE 3
#define ETAT_A_VIDE 4
```

Chaque numéro donne une signification à la variable `etat`. Par exemple,
`etat = ETAT_MARCHE;` signifie « à partir de maintenant, traiter le fonctionnement ».
Le nombre n'a pas de propriété spéciale : c'est son nom qui aide à lire.

## 7. Les variables

| Variable | Son rôle |
|---|---|
| `etat` | La situation que le programme doit traiter |
| `etat_precedent` | La situation affichée la dernière fois |
| `compteur` | Les passages à l'arrêt ou au démarrage |
| `compteur_haut` | Les passages consécutifs avec une mesure trop forte |
| `compteur_bas` | Les passages consécutifs avec une mesure trop faible |
| `courant` | Le résultat de `Lire_Courant()` ; le nom ne garantit pas une mesure en ampères |

`etat_precedent` commence à 255, qui n'est aucun des cinq états prévus. Cela
force un premier affichage, puisque l'état initial vaut 0.

## 8. Le début de `main`

```c
SYSTEM_Initialize();
LCD_Initialize();
MOTEUR_OFF();
```

On prépare les périphériques avec MCC, puis l'écran, puis on demande l'arrêt
sur les sorties de commande. Ces trois lignes s'exécutent avant la boucle.

**Test à faire quand on reprendra le code :** vérifier l'affichage et la LED
de commande en basse tension, sans moteur secteur connecté.

## 9. Lire puis choisir un `case`

```c
courant = Lire_Courant();
switch (etat)
{
    // un case par état
}
```

On prend une mesure. `switch` regarde ensuite la valeur de `etat` et choisit
le bloc `case` correspondant. `break` termine ce choix et quitte le `switch`.
Changer `etat` dans un `case` ne lance pas immédiatement le nouveau `case` :
il sera choisi au passage suivant.

## 10. Le bloc `ETAT_ARRET`

Il demande l'arrêt et ajoute 1 à `compteur`. À partir de 20 passages, il remet
le compteur à zéro et choisit `ETAT_DEMARRAGE`.

Ce départ après environ 2 secondes sert au test. Ce n'est pas un déclenchement
par présence d'eau ni un autotest tous les trois jours.

**Exercice :** si `compteur` vaut 19 à l'entrée de ce bloc, il devient 20,
puis il est remis à 0 et l'état devient DEMARRAGE.

## 11. Le bloc `ETAT_DEMARRAGE`

Il demande la marche et incrémente `compteur`.

1. Avant 30 passages, il laisse le démarrage continuer.
2. Dès 30 passages, si la mesure est au plus égale à 173, il passe à MARCHE
   et remet les compteurs à zéro.
3. Si la mesure reste supérieure à 173 jusqu'à 50 passages, il passe à BLOQUE.

Une mesure très faible permet aussi de passer à MARCHE : ce bloc teste seulement
le seuil haut. Le défaut bas sera ensuite traité dans MARCHE.

## 12. Le bloc `ETAT_MARCHE`

La commande est maintenue à 1. Il y a ensuite trois possibilités :

```c
if (courant > SEUIL_HAUT)
{
    compteur_haut = compteur_haut + 1;
    compteur_bas = 0;
}
```

Mesure forte : compter sa durée et oublier une éventuelle suite de mesures faibles.
Le `else if` fait l'inverse pour une mesure sous le seuil bas.
Le `else` remet les deux compteurs à zéro quand la mesure est dans la zone normale.

Les deux tests suivants déclarent BLOQUE ou A_VIDE quand le compteur concerné
atteint 100. Il faut donc une suite de mesures anormales, sans retour dans la
zone normale entre elles. On ne cumule pas toutes les anomalies de la journée.

**Exemple :** haut, haut, normal, haut → compteur haut : 1, 2, 0, 1.

## 13. Les blocs BLOQUE et A_VIDE

Ils demandent tous les deux l'arrêt. Ils ne changent plus `etat` : le programme
reste donc dans le défaut jusqu'au reset, dans cette version.

**Point à reprendre :** quand MARCHE choisit un défaut, sa commande a déjà été
mise à 1 au début du `case`. La commande d'arrêt du `case` de défaut n'arrive
qu'au tour suivant, après les traitements et l'attente. Il faudra revoir ce délai
de réaction, sans confondre « changer l'état » et « changer la sortie ».

## 14. Le bloc `default`

Si la variable contient une valeur qui n'est aucun des cinq états, le programme
demande l'arrêt et remet l'état à ARRET. Cela ne corrige pas la cause d'une valeur
inattendue ; cela décrit seulement le comportement de repli prévu.

## 15. Afficher seulement quand l'état change

```c
if (etat != etat_precedent)
{
    Afficher_Etat(etat);
    etat_precedent = etat;
}
```

Si l'état est différent de celui mémorisé, on affiche le nouveau et on le mémorise.
Cela évite d'effacer et de réécrire l'écran à chaque tour.

## 16. `__delay_ms(100)`

Le `main` attend 100 millisecondes avant de recommencer. Pendant cette attente,
il ne prend pas de nouvelle décision dans cette boucle. Ce délai facilite le
test, mais ne remplace pas une gestion du temps précise pour le projet final.

## 17. La fonction `Lire_Courant()`

```c
return (uint8_t)(ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8);
```

Elle effectue trois opérations :

1. Demander une conversion de la tension de `Capteur_COURANT` par rapport
   à la référence négative sélectionnée.
2. Décaler le mot retourné de 8 bits vers la droite, pour garder son octet haut.
3. Renvoyer cette valeur dans un `uint8_t`.

Le pilote génère ici un résultat 12 bits au format signe et valeur absolue,
aligné à gauche. Pour une entrée positive référencée à la masse, cet octet haut
conserve les huit bits de poids fort de la mesure. C'est lié à ces réglages précis.
Il ne faut pas recopier `>> 8` avec un autre format sans le vérifier.
[Format du résultat, figure 17-3 de Microchip](https://ww1.microchip.com/downloads/en/DeviceDoc/40001675C.pdf).

## 18. La fonction `Afficher_Etat()`

Elle efface l'écran, place le curseur en première ligne, écrit `Etat moteur :`,
puis place le curseur en deuxième ligne. Son `switch` choisit ensuite un message
selon le nombre reçu : `Arret`, `Demarrage`, `En marche`, ou le défaut.

Le premier `switch` du programme décide du comportement ; celui-ci choisit
seulement le texte. Les commandes internes du LCD sont expliquées dans
[[19 - Memo code - LCD et MCC]].

## Vérifier que j'ai compris

- [ ] Je peux expliquer pourquoi on garde deux compteurs de défaut.
- [ ] Je sais ce qui se passe exactement à une mesure égale à 91 ou 173.
- [ ] Je sais que l'attente de 2 secondes est un comportement de test.
- [ ] Je peux expliquer pourquoi le LCD n'est réécrit que lors d'un changement.
- [ ] Je distingue la commande d'une sortie et la preuve que le moteur tourne.

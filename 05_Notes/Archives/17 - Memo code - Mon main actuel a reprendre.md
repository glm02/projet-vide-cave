---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# Mon `main.c` actuel : comprendre avant de corriger

Retour : [[14 - Memo code - Commencer ici]]

## 1. Le fichier contient deux programmes collés

Au moment de la lecture, `main.c` contient un premier `main` à la ligne 27,
puis un second à la ligne 115. La seconde partie reprend le programme expliqué
dans [[16 - Memo code - Le programme simple]].

Un même programme ne peut pas définir deux fois sa fonction `main`.
Plusieurs variables, constantes et fonctions sont également redéfinies.
La première reprise sera donc de choisir et séparer les versions, avant
d'essayer de comprendre un message de compilation ligne par ligne.

La configuration MPLAB lue liste `main.c` et `lcd.c` parmi les sources.
La présence de `main_simple.c` ou de `main_test_efficace.c` dans le dossier
ne signifie pas que MPLAB utilise automatiquement ces deux fichiers.

## 2. Les noms des commandes et les seuils

```c
#define MOTEUR_ON() OPTO_TRIAC_SetHigh()
#define MOTEUR_OFF() OPTO_TRIAC_SetLow()
#define SEUIL_HAUT 150
#define SEUIL_BAS 40
```

Les deux premières lignes donnent des noms aux commandes de la sortie.
Les deux suivantes nomment des seuils. Mais, dans le premier bloc de programme,
ces deux noms de seuil ne sont pas utilisés dans les conditions de mesure.

**À retenir :** écrire un `#define` ne déclenche aucun comportement à lui seul.

## 3. Les états et les variables

Les cinq noms d'état préparent une organisation future. Dans le premier `main`,
il n'y a pourtant aucun `switch(etat)` : la variable d'état ne dirige pas encore
le comportement.

`valeur` reçoit une conversion brute ; `courant` reçoit le résultat de la fonction
`Mesure_Courant()`. Les déclarations `uint16_t` donnent une capacité de stockage,
pas une unité physique : ces nombres ne sont pas automatiquement des ampères.

Les prototypes annoncent `Mesure_Courant()` et `Afficher_Etat()` au compilateur.

## 4. La boucle du premier `main`

```c
valeur = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF);
printf("CAN=%u\r\n", valeur);
__delay_ms(200);
courant = Mesure_Courant();
```

On lit une conversion, on envoie sa valeur sous forme de texte, on attend 200 ms,
puis on appelle la fonction de mesure. `%u` est un emplacement pour un entier
non signé ; `\r\n` demande un retour au début de ligne puis une nouvelle ligne.
L'envoi passe par la liaison série du projet, pas par le LCD.

Les lignes suivantes mettent la sortie LED à 1 pendant 1 seconde, puis à 0
pendant 1 seconde. Ce clignotement ralentit fortement la boucle de surveillance.
Il peut aussi masquer les commandes de LED faites dans la fonction de mesure.

## 5. Les variables de `Mesure_Courant()`

```c
uint16_t maxi = 173;
uint16_t mini = 91;
```

`maxi` est destiné à garder la plus grande mesure trouvée, `mini` la plus petite.
Les valeurs 173 et 91 ne sont pas ici des seuils utilisés comme tels : elles
servent de points de départ à la recherche du maximum et du minimum.

**Pourquoi cela gêne :** si toutes les mesures valent 120, `maxi` restera 173
et `mini` restera 91. Le maximum et le minimum mémorisés ne décriront pas le signal.
Pour chercher les extrêmes, on pourra partir de la première vraie mesure ou
de bornes adaptées au format du résultat.

Autre point : cette version lit le mot brut de `ADC_GetConversion`, alors que
le programme simple conserve seulement son octet haut. Les mêmes nombres de
seuil ne peuvent pas être repris sans vérifier l'échelle de mesure.

## 6. La boucle de 100 lectures

```c
for (i = 0; i < 100; i++)
```

Elle fait 100 passages. À chacun, on prend une mesure et on attend ensuite
200 microsecondes. Une microseconde vaut un millionième de seconde.
100 × 200 µs = 20 ms d'attente, **auxquelles s'ajoutent** les conversions et
les instructions. Cela ne garantit donc pas une acquisition exactement sur 20 ms.

## 7. Le test du maximum commande actuellement la sortie

```c
if (mesure > maxi)
{
    maxi = mesure;
    MOTEUR_ON();
    LED_SetHigh();
}
```

Cette condition demande : « ai-je trouvé un nouveau record maximum ? ».
Si oui, elle mémorise ce record et demande la marche.
Elle ne demande pas : « le courant dépasse-t-il le seuil haut fixé ? ».
Au fil de la boucle, `maxi` change : ce n'est plus une limite fixe.

On reprendra la séparation des rôles : mesurer d'abord, comparer le résultat
à des seuils ensuite, puis appliquer la commande correspondante.

## 8. Le test du minimum et l'appel manquant

Le deuxième `if` met à jour le minimum et demande l'arrêt lors d'un nouveau minimum.
La ligne suivante est actuellement écrite :

```c
LED_D1_SetHigh;
```

Dans `pins.h`, le nom est défini comme une macro avec parenthèses. Pour
l'appeler, la forme attendue est :

```c
LED_D1_SetHigh();
```

Ces parenthèses font partie de l'appel. L'extrait montre la correction à prévoir ;
le fichier C n'a pas été modifié pendant la rédaction de cette fiche.

## 9. Pourquoi `courant` reste à zéro

```c
return 0;
```

Cette ligne renvoie toujours zéro. Le `main` range donc toujours zéro dans
`courant`, quelles que soient les mesures faites à l'intérieur.
Il faudra décider du résultat souhaité : un maximum, un écart max − min ou
une autre mesure. Une seule fonction ne doit pas être présentée comme faisant
un calcul qu'elle ne renvoie pas encore.

## 10. `Afficher_Etat()` n'affiche encore rien dans cette première version

Le corps contient seulement un commentaire. Il n'y a donc aucune instruction
d'affichage exécutée par cette définition. Le premier `main` ne l'appelle pas non plus.

## Notre ordre de reprise

- [ ] Garder une seule définition de `main` et des noms cohérents.
- [ ] Lire et afficher une mesure pour comprendre son échelle.
- [ ] Corriger la recherche du minimum et du maximum.
- [ ] Renvoyer le résultat qu'on veut utiliser.
- [ ] Déplacer la décision de commande après la mesure.
- [ ] Reprendre les temps d'attente, les états et l'affichage progressivement.

**Petit exercice :** si la fonction mesure dix valeurs différentes mais termine
par `return 0;`, que reçoit `courant` ? Réponse : 0. Le résultat renvoyé dépend
de la ligne `return`, pas du nom donné à la fonction.

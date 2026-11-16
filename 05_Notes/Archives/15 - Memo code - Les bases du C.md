---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# Les bases du C, sans prérequis

Retour : [[14 - Memo code - Commencer ici]] · Ensuite : [[16 - Memo code - Le programme simple]]

## 1. Les outils et les fichiers

| Mot | Explication simple |
|---|---|
| C | Le langage dans lequel on écrit les instructions |
| PIC | Le microcontrôleur qui exécutera ces instructions |
| MPLAB X | Le logiciel dans lequel on ouvre et prépare le projet |
| XC8 | Le compilateur : il traduit le C pour le PIC et signale des erreurs |
| MCC | L'outil qui génère du code pour configurer les fonctions du PIC |
| Fichier `.c` | Le détail des instructions et des fonctions |
| Fichier `.h` | Les noms et déclarations dont d'autres fichiers ont besoin |
| Compiler | Traduire le programme et vérifier qu'on peut construire le logiciel |
| Programmer le PIC | Envoyer le logiciel construit dans la mémoire du PIC |

Compiler sans erreur ne prouve pas que le comportement est correct.

## 2. Lire une ligne

```c
compteur = compteur + 1;
```

On prend l'ancienne valeur de `compteur`, on ajoute 1 et on range le résultat
dans `compteur`. S'il valait 4, il vaut maintenant 5.

Le point-virgule `;` termine cette instruction.
Les accolades `{ }` regroupent plusieurs instructions en un bloc.
`//` commence un commentaire jusqu'à la fin de la ligne.
`/* ... */` permet un commentaire sur plusieurs lignes.
Les commentaires aident à lire ; le PIC ne les exécute pas.

## 3. Une variable est une case avec un nom

```c
uint8_t compteur = 0;
```

- `uint8_t` : la catégorie de nombres qu'on peut ranger dans cette case.
- `compteur` : son nom.
- `= 0` : sa valeur de départ.

| Type rencontré | Nombres qu'il peut contenir | Exemple d'utilisation |
|---|---|---|
| `uint8_t` | 0 à 255 | Petit compteur ou état |
| `int8_t` | −128 à 127 | Valeurs positives et négatives de la table sinus |
| `uint16_t` | 0 à 65 535 | Somme de mesures |
| `int16_t` | −32 768 à 32 767 | Écart pouvant être négatif |
| `uint32_t` | 0 à 4 294 967 295 | Grande somme de carrés |
| `char` | Un élément de texte, par exemple `'A'` | Texte du LCD |

Le `u` signifie « non signé » : pas de nombres négatifs.
Un bit est un chiffre binaire, 0 ou 1. Un octet contient 8 bits.
Avec 8 bits, il y a 256 combinaisons : les nombres de 0 à 255.

**Piège :** si un `uint8_t` vaut 255 puis reçoit `compteur + 1`, la valeur
stockée revient à 0. Il faut choisir une taille adaptée.

Une variable déclarée hors des fonctions est globale : plusieurs fonctions du
fichier peuvent l'utiliser. Déclarée dans une fonction, elle est locale à ce bloc.
Une variable locale ordinaire sans valeur de départ ne doit pas être lue avant
d'avoir reçu une valeur. Les variables globales sans initialiseur partent à zéro.

## 4. Un nom fixe avec `#define`

```c
#define SEUIL_HAUT 173
```

Cela donne un nom au nombre 173. Avant la compilation, `SEUIL_HAUT` est remplacé
par cette valeur. Ce n'est pas une case qu'on incrémente pendant le programme.
On ne met pas de `;` à la fin de ce `#define`.

## 5. Choisir avec `if` et `else`

```c
if (courant > SEUIL_HAUT)
{
    compteur_haut = compteur_haut + 1;
}
else
{
    compteur_haut = 0;
}
```

Si la mesure dépasse le seuil, on incrémente le compteur. Sinon, on le remet à zéro.
`else if` permet d'essayer une autre condition quand la précédente est fausse.

| Symbole | Se lit |
|---|---|
| `=` | Ranger une valeur |
| `==` | Est égal à ? |
| `!=` | Est différent de ? |
| `>` / `<` | Est supérieur / inférieur à ? |
| `>=` / `<=` | Est supérieur ou égal / inférieur ou égal à ? |
| `||` | Au moins une des deux conditions est vraie |
| `&&` | Les deux conditions sont vraies |

**À retenir :** `etat = 2;` change l'état. `etat == 2` pose une question.

## 6. Répéter avec une boucle

```c
while (1)
{
    courant = Lire_Courant();
}
```

`while` veut dire « tant que ». Ici 1 est toujours vrai : on recommence sans fin.

```c
for (i = 0; i < 100; i++)
{
    // traitement
}
```

On démarre avec `i = 0`, on continue tant que `i < 100`, et `i++` ajoute 1 après
chaque passage. On utilise donc les valeurs 0 à 99 : cela fait bien 100 passages.

## 7. Une fonction est un travail auquel on donne un nom

```c
uint8_t Lire_Courant(void);
```

C'est un prototype : il annonce une fonction dont le détail est écrit ailleurs.
`uint8_t` à gauche annonce le type de résultat. `void` entre parenthèses dit
qu'on ne lui fournit aucun paramètre.

```c
courant = Lire_Courant();
```

On appelle la fonction, puis on range son résultat dans `courant`.
Dans la fonction, `return valeur;` renvoie un résultat à l'endroit de l'appel.

```c
void Afficher_Etat(uint8_t etat_a_afficher);
```

Cette fonction reçoit un nombre appelé `etat_a_afficher`. Elle ne renvoie pas
de résultat : le `void` à gauche le précise. Son travail est d'agir sur l'écran.
Déclarer une fonction ne l'exécute pas : il faut l'appeler avec ses parenthèses.

## 8. Un tableau est une suite de cases

```c
uint8_t mesures[3] = {10, 20, 30};
```

`mesures[0]` vaut 10, `mesures[1]` vaut 20 et `mesures[2]` vaut 30.
La première case porte le numéro 0. La case `mesures[3]` n'existe pas dans
ce tableau. L'indice désigne la case qu'on veut lire ou modifier.

## 9. Les calculs et les conversions de type

Avec des entiers, `7 / 2` donne 3 : la partie après la virgule est perdue.
`(uint16_t)valeur` demande de traiter `valeur` dans le type indiqué : c'est un
« cast », ou conversion de type.

Il faut aussi une taille suffisante **pendant** le calcul. Ranger un résultat
dans une grande variable ne répare pas un calcul qui a déjà débordé.

Pour des nombres positifs non signés : `x >> 1` revient à diviser par 2,
`x >> 3` à diviser par 8, et `x << 1` à multiplier par 2 si cela tient dans le type.
On perd les bits qui sortent. Pour des nombres négatifs, ne pas appliquer cette
recette sans vérifier les règles du compilateur.

## 10. Le tout petit exercice

Sans exécuter le programme, complète :

```c
uint8_t compteur = 3;
compteur = compteur + 1;
if (compteur >= 4)
{
    compteur = 0;
}
```

**Après l'addition :** … · **Condition vraie ou fausse :** … · **Valeur finale :** …

<details><summary>Voir la correction</summary>

Après l'addition : 4. La condition est vraie. La valeur finale est 0.

</details>

Référence pour les types utilisés par XC8 :
[documentation Microchip](https://onlinedocs.microchip.com/oxy/GUID-BB433107-FD4E-4D28-BB58-9D4A58955B1A-en-US-9/GUID-3A37A613-2364-4965-9519-E79FE101ECC3.html).

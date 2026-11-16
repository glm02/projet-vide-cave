---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# `main_test_efficace.c` : comprendre chaque bloc

Retour : [[14 - Memo code - Commencer ici]]

Cette fiche vient après [[15 - Memo code - Les bases du C]] et
[[16 - Memo code - Le programme simple]]. On peut la lire en plusieurs fois.

## 1. Ce test a un autre rôle que le programme moteur

Il cherche à acquérir un signal et à comparer deux façons de calculer sa valeur
efficace alternative. Il affiche des nombres sur la liaison série.
Il ne contient pas la machine à états et la commande moteur du programme simple.
Il ne faut pas présenter toutes les versions comme un programme unique déjà intégré.

## 2. Les constantes du début

| Nom | Valeur actuelle | Son rôle |
|---|---|---|
| `N_ECH` | 100 | Nombre de points mémorisés pour une acquisition |
| `N_MOYENNE` | 8 | Nombre de résultats gardés pour lisser l'affichage |
| `SIMULATION` | 1 | Utiliser la table de sinus plutôt que mesurer le signal à chaque point |
| `CENTRE` | 128 | Niveau central du signal artificiel |
| `AFFICHER_ECHANTILLONS` | 0 | Ne pas envoyer la liste des 100 points en plus du résumé |
| `FENETRE_MIN`, `FENETRE_MAX` | 0 et 255 | Limites du contrôle des points |

Ces valeurs configurent ce fichier de test. Le commentaire qui demande un
réglage MCC n'effectue pas lui-même ce réglage.

## 3. La table `sinus[]`

```c
const int8_t sinus[N_ECH] = { /* les 100 nombres du fichier */ };
```

Imagine une courbe dont on a relevé 100 hauteurs. Le tableau conserve ces hauteurs
dans l'ordre : elles montent, redescendent, deviennent négatives, puis reviennent.
`const` indique qu'on ne modifie pas ce tableau dans le programme.
`int8_t` permet les valeurs négatives nécessaires à cette courbe.

Cette table est une suite de nombres dans la mémoire du PIC : elle ne produit
pas à elle seule une tension sinusoïdale sur une broche.

## 4. Les deux tableaux de travail

`echantillons[100]` garde les 100 points de l'acquisition. `historique[8]` garde
les huit derniers résultats efficaces. Un échantillon est une valeur prise
à un instant ; l'historique contient des résultats calculés à partir de plusieurs valeurs.

`index_historique` indique quelle case de l'historique sera remplacée.
Ces tableaux sont globaux : au départ, leurs cases valent zéro.

## 5. Le déroulement de `main`

```text
Initialiser → Acquérir 100 points → Calculer → Envoyer le résumé
                   ↑                                  ↓
                   └──────── Attendre 300 ms ──────────┘
```

La moyenne, la crête, les deux résultats efficaces, le résultat lissé et le nombre
de points hors fenêtre sont des variables locales. Le programme remplit d'abord
le tableau, puis calcule ces valeurs et les affiche avec `printf`.

`%u` réserve une place à un entier non signé ; les arguments qui suivent le texte
remplissent ces places dans le même ordre. Le texte `pts` signifie « points
numériques », `mV` « millivolts ». 1 000 mV = 1 V.

Si `AFFICHER_ECHANTILLONS` vaut 1, une boucle envoie aussi les 100 points un par un.
Le délai de 300 ms s'ajoute aux acquisitions, aux calculs et aux envois : il ne
garantit pas exactement trois affichages par seconde.

## 6. `Lire_CAN_8bits()`

Cette fonction appelle le CAN, garde l'octet haut avec `>> 8`, puis renvoie un
`uint8_t`. Le mécanisme est expliqué dans la fiche du programme simple.
Une valeur numérique n'a pas automatiquement l'unité ampère.

## 7. `Echantillonner_Periode()` : préparer l'amplitude

```c
amplitude = Lire_CAN_8bits() >> 1;
```

On lit le potentiomètre puis on divise sa valeur positive par 2 avec un décalage.
Cela donne un nombre de 0 à 127 pour régler la hauteur du signal simulé.
Cette lecture est faite une fois avant les 100 points.

## 8. Attendre avec `TMR2IF`

```c
while (TMR2IF == 0)
{}
TMR2IF = 0;
```

Le timer est un compteur matériel. Son drapeau `TMR2IF` signale un événement
de fin de période. Le programme regarde le drapeau en boucle jusqu'à ce qu'il
vaille 1, puis le remet à zéro pour attendre le suivant. C'est de la scrutation,
pas une fonction d'interruption appelée automatiquement.

Le but annoncé est un point toutes les 200 µs. 100 × 200 µs = 20 ms,
qui est la durée d'un cycle à 50 Hz.

**Point important dans les fichiers lus :** `SYSTEM_Initialize()` n'appelle
aucune initialisation de Timer2 et aucun pilote Timer2 n'est présent dans les
fichiers MCC listés. Ce fichier ne configure pas non plus Timer2 directement.
Il faut donc ajouter et vérifier cette configuration avant de pouvoir utiliser
cette attente ; sinon le programme peut rester bloqué ici.

La première attente dépend aussi de la position du compteur matériel lorsqu'on
efface le drapeau. Les traitements entre deux événements doivent rester assez
courts pour ne pas manquer une période.

## 9. Fabriquer un point simulé ou lire un point réel

```c
echantillons[i] = (uint8_t)(CENTRE + (((int16_t)sinus[i] * amplitude) >> 7));
```

Cette ligne est utilisée quand `SIMULATION == 1`. Lis-la de l'intérieur vers
l'extérieur :

1. Prendre le point numéro `i` de la table.
2. Le multiplier par l'amplitude choisie au potentiomètre.
3. Réduire l'échelle par le décalage de 7 bits.
4. Ajouter le centre 128.
5. Ranger le résultat dans la case `i` des échantillons.

**Exemple avec un point positif :** point de table 64, amplitude 64 →
64 × 64 = 4 096 → décalage de 7 bits : 32 → ajouter 128 : résultat 160.
Un point zéro donne 128. Les points négatifs passent sous le centre.
Le cast en `int16_t` permet ici le produit signé dans une taille plus grande.
Le décalage signé dépend des règles du compilateur ; il ne faut pas le remplacer
aveuglément par une division quand on retravaillera les nombres négatifs.

Quand `SIMULATION == 0`, le `else` lit réellement le CAN pour chaque point.
Le mode simulation valide des calculs sur un signal fabriqué, pas la mesure
du courant de la vraie pompe.

## 10. `Calcul_Moyenne()`

La fonction additionne les 100 points puis divise la somme par 100.
Par exemple : 100, 120, 140 → somme 360 → moyenne 120.

Le tableau contient jusqu'à 255 par point. Une somme de 100 points peut atteindre
25 500 : elle ne tiendrait pas dans un `uint8_t`, mais tient dans un `uint16_t`.
La moyenne sert ici à estimer le centre du signal.

## 11. `Calcul_Crete()`

Elle part avec `maxi = 0` et `mini = 255`, puis regarde chaque point.
Un nouveau maximum remplace `maxi`, un nouveau minimum remplace `mini`.
Elle renvoie ensuite `(maxi - mini) / 2`.

**Exemple :** maximum 168, minimum 88 → écart total 80 → demi-écart 40.
L'écart total est la valeur crête à crête. La moitié représente l'amplitude
d'un signal symétrique. Pour un signal quelconque, ce n'est pas forcément
la plus grande distance entre la moyenne et un sommet.

## 12. La première méthode de valeur efficace

```c
eff_methode1 = (uint8_t)(((uint16_t)crete * 181) >> 8);
```

Pour une sinusoïde centrée, la valeur efficace alternative vaut la crête divisée
par √2, soit environ crête × 0,707. Le code approche ce facteur avec 181 / 256.
Le décalage de 8 bits divise le produit positif par 256.

Avec une crête de 40 points : 40 × 181 = 7 240 ; 7 240 / 256 donne 28 en entier.
Cette méthode dépend de la forme sinusoïdale : on ne doit pas l'appliquer
comme une formule universelle pour toutes les formes de courant.

## 13. `Calcul_Efficace()` : la deuxième méthode

Pour chaque point, on soustrait la moyenne pour enlever le centre constant.
On met cet écart au carré, on additionne les carrés, on divise par le nombre
de points, puis on prend la racine carrée.

```text
points → enlever la moyenne → carrés → moyenne des carrés → racine
```

**Exemple très court :** points 126 et 130, moyenne 128 → écarts −2 et +2 →
carrés 4 et 4 → moyenne des carrés 4 → racine 2 points.

C'est la valeur efficace de la **partie alternative après retrait de la moyenne**,
pas celle du signal complet contenant son niveau continu.

`ecart` est signé parce qu'il peut être négatif. `somme_carres` utilise 32 bits
parce que la somme peut dépasser 65 535.

**Calcul à reprendre :** le commentaire suppose des écarts limités à ±127.
Ce n'est pas garanti pour tous les signaux entre 0 et 255. Sur XC8 PIC,
le produit signé `ecart * ecart` peut dépasser la capacité de son calcul
intermédiaire. Une grande variable de destination ne suffit pas : il faut
prévoir un type assez grand avant la multiplication et vérifier les bornes.
Par exemple, 253² = 64 009, supérieur à 32 767.
[Types entiers XC8](https://onlinedocs.microchip.com/oxy/GUID-BB433107-FD4E-4D28-BB58-9D4A58955B1A-en-US-9/GUID-3A37A613-2364-4965-9519-E79FE101ECC3.html).

## 14. `Racine_Entiere()`

Elle cherche un entier `r` en partant de 0. Tant que le carré de `r + 1`
ne dépasse pas le nombre reçu, elle augmente `r`.
Pour 10, on peut atteindre 3 car 3² = 9, mais pas 4 car 4² = 16.
La fonction renvoie donc 3 : elle tronque la racine.

Cela permet un calcul entier simple à expliquer. Ce n'est pas une fonction
générale vérifiée pour toute la plage possible d'un `uint16_t` : sa boucle
et ses produits doivent rester dans les bornes attendues du test.

## 15. `Moyenne_Glissante()`

Elle place le nouveau résultat dans `historique[index_historique]`, avance
l'indice et revient à 0 après la case 7. Les huit cases sont réutilisées en boucle.
Elle additionne ensuite les huit cases et divise par 8 avec `>> 3`.

Le but est de calmer les variations d'affichage, en échange d'une réaction
plus progressive. Au démarrage, sept cases valent encore zéro : si la première
valeur reçue est 80, le premier résultat lissé vaut 10. Il faudra remplir
l'historique ou compter les valeurs disponibles pour éviter ce démarrage faussé.

## 16. `Points_vers_mV()`

```c
return (uint16_t)(((uint32_t)points * 5000) >> 8);
```

Dans l'échelle supposée de 0 à 255 pour 0 à presque 5 V, on multiplie les points
par 5 000 puis on divise par 256. 128 points donnent 2 500 mV, donc 2,5 V.
Le cast en 32 bits vient avant le produit pour accueillir sa taille.

Cela suppose une référence de 5 V et le format de résultat attendu. Ce calcul
convertit une tension en millivolts, pas un courant en ampères.

## 17. `Compter_Hors_Fenetre()`

Pour chaque point, la condition teste « inférieur au minimum **ou** supérieur
au maximum ». Le symbole `||` signifie « ou ». Chaque point hors limites
ajoute 1 au compteur renvoyé.

Avec les limites actuelles 0 et 255, aucun élément d'un tableau `uint8_t`
ne peut sortir de la fenêtre. Le contrôle renverra donc zéro. Il faut des
limites plus serrées pour que ce test fournisse une information utile.

## Les petits exercices

- [ ] Expliquer pourquoi une table sinus n'est pas une mesure sur la pompe.
- [ ] Retrouver ce qui se passe si `TMR2IF` ne devient jamais 1.
- [ ] Calculer la moyenne et le demi-écart de 90, 100 et 110.
- [ ] Expliquer pourquoi on soustrait la moyenne avant les carrés.
- [ ] Expliquer le premier résultat de la moyenne glissante avec des cases à zéro.

Correction du troisième exercice : moyenne 100, maximum 110, minimum 90,
demi-écart 10. Ce sont deux informations différentes sur le même signal.

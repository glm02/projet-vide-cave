---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# Comprendre mon code en partant de zéro

Retour : [[00 - Accueil]]

Le PIC est une petite puce qui exécute des instructions. Ton programme lui dit
quoi lire, quelle décision prendre et quelle sortie commander. On va apprendre
ces trois choses progressivement.

## Dans quel ordre lire les fiches ?

| Ordre | Fiche | Ce que je vais comprendre |
|---|---|---|
| 1 | [[15 - Memo code - Les bases du C]] | Les symboles, variables, conditions, boucles et fonctions |
| 2 | [[16 - Memo code - Le programme simple]] | Chaque bloc de `main_simple.c`, les états et les seuils |
| 3 | [[17 - Memo code - Mon main actuel a reprendre]] | Ce que fait `main.c` aujourd'hui et les erreurs à corriger |
| 4 | [[18 - Memo code - Le test de valeur efficace]] | Les échantillons, les calculs et le signal simulé |
| 5 | [[19 - Memo code - LCD et MCC]] | Les fonctions d'affichage et le code qui prépare le PIC |

Commence par les deux premières fiches. Le calcul de valeur efficace peut attendre
que tu comprennes la lecture d'une valeur et les conditions.

## Comment utiliser une fiche ?

Lis un seul bloc, puis explique-le sans regarder. Fais ensuite le petit exercice
sur papier. Quand on modifiera le programme, on testera une seule étape à la fois.
Les extraits sont des explications du code actuel, pas un nouveau programme complet.

## Ce qu'on va retravailler ensemble

1. Choisir un seul `main` : le fichier actuel en contient deux.
2. Lire une valeur du CAN et l'afficher.
3. Comprendre les nombres obtenus avant de choisir les seuils.
4. Tester les conditions avec une LED.
5. Reprendre les états et les durées.
6. Ajouter les calculs et les fonctions du projet seulement quand la base est comprise.

**L'objectif :** que tu puisses expliquer chaque ligne avec tes mots, y compris
les limites du test et ce qu'il reste à faire.

## D'où viennent ces explications ?

Les fichiers lus sont ceux de `Projet_vide_cave_1.X`, dans ta copie locale du
projet sous `Documents/Ecole` : `main.c`, `main_simple.c`, `main_test_efficace.c`,
`lcd.c`, `lcd.h` et les pilotes MCC utilisés. Les fonctions de `libsae22` sont
évoquées pour distinguer cette bibliothèque du projet MCC actuel ; leur catalogue
complet n'est pas le sujet de ces fiches.

Lecture le 4 novembre 2026. Aucun fichier C n'a été modifié, aucune compilation
ni programmation du PIC n'a été lancée pendant la création des fiches.
Les réglages demandés dans un commentaire ne sont pas forcément ceux générés.

Le questionnaire pour rédiger le dossier reste ici :
[[13 - Questions pour rediger le dossier de conception]].

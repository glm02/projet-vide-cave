---
tags: [vide-cave, oral, INFO, programme]
date: 2026-11-04
---
# Questions sur mon programme

Retour : [[00 - Accueil]]

**Pour préparer le dossier avec tes réponses, commence par
[[13 - Questions pour rediger le dossier de conception]].**
La présente fiche concerne seulement l'explication du code.

Pour apprendre avant de répondre : [[14 - Memo code - Commencer ici]].

Ces questions partent des fichiers `main.c`, `main_simple.c` et
`main_test_efficace.c`, trouvés dans le dossier local `Projet_vide_cave_1.X`.
Elles concernent le programme actuel. Les questions KiCad viendront plus tard.

## Comment travailler

Je réponds à voix haute, avec mes mots, en quelques phrases. Je peux ensuite
dicter ma réponse pour la corriger : d'abord les erreurs techniques, puis les
phrases difficiles à comprendre. Le but est de pouvoir expliquer mon code.

Les réponses restent vides pour commencer par ce que je comprends vraiment.
Si je bloque, je note la ligne ou le mot que je ne comprends pas.

## Pour commencer : expliquer ce que j'ai fait

### 1. Ton programme sert à faire quoi ?

**Ma réponse :**

**À corriger ou à revoir :**

### 2. Qu'est-ce que tu as déjà testé sur la carte ? Qu'est-ce qui reste à tester ?

Préciser ce que j'ai réellement observé : LED, affichage, potentiomètre, liaison série…

**Ma réponse :**

**À corriger ou à revoir :**

### 3. À quoi sert le potentiomètre dans ton test ?

Expliquer ce qu'il remplace et ce que ce test permet de vérifier.

**Ma réponse :**

**À corriger ou à revoir :**

### 4. Quelle différence fais-tu entre `main_simple.c` et `main_test_efficace.c` ?

**Ma réponse :**

**À corriger ou à revoir :**

## Lire et expliquer le programme simple

### 5. Que fait `SYSTEM_Initialize()` ? Pourquoi l'appeler au début ?

**Ma réponse :**

**À corriger ou à revoir :**

### 6. À quoi sert le `while (1)` ?

**Ma réponse :**

**À corriger ou à revoir :**

### 7. À quoi correspondent `SEUIL_BAS` et `SEUIL_HAUT` ?

Les valeurs 91 et 173 de `main_simple.c` correspondent-elles à des ampères,
à des volts ou à des points du CAN ? Comment ont-elles été choisies pour ce test ?

**Ma réponse :**

**À corriger ou à revoir :**

### 8. À quoi servent les cinq états du moteur ?

Expliquer avec mes mots : `ARRET`, `DEMARRAGE`, `MARCHE`, `BLOQUE`, `A_VIDE`.

**Ma réponse :**

**À corriger ou à revoir :**

### 9. Pourquoi utiliser `switch (etat)` ? À quoi sert `break` ?

**Ma réponse :**

**À corriger ou à revoir :**

### 10. Pourquoi laisser un temps au moteur pour démarrer avant de déclarer un défaut ?

Retrouver où ce délai intervient dans `ETAT_DEMARRAGE`.

**Ma réponse :**

**À corriger ou à revoir :**

### 11. À quoi servent `compteur_haut` et `compteur_bas` ?

Que se passe-t-il si la mesure revient entre les deux seuils ?

**Ma réponse :**

**À corriger ou à revoir :**

### 12. Dans ce programme, comment comptes-tu les dix secondes ?

Expliquer le lien entre `DUREE_10S`, le compteur et `__delay_ms(100)`.
Est-ce une durée exacte si la boucle fait aussi des conversions et de l'affichage ?

**Ma réponse :**

**À corriger ou à revoir :**

### 13. Une fois en défaut, comment le programme peut-il repartir ?

Lire les `case ETAT_BLOQUE` et `case ETAT_A_VIDE`, puis distinguer ce comportement
de celui qu'on souhaite pour la version finale.

**Ma réponse :**

**À corriger ou à revoir :**

### 14. Pourquoi garder la variable `etat_precedent` pour le LCD ?

**Ma réponse :**

**À corriger ou à revoir :**

## Comprendre la mesure

### 15. Que renvoie `ADC_GetConversion(...)` ?

Qu'est-ce qui est réellement mesuré sur l'entrée ? Qu'est-ce qu'il faudrait
connaître pour exprimer ensuite cette mesure en ampères ?

**Ma réponse :**

**À corriger ou à revoir :**

### 16. Pourquoi y a-t-il `>> 8` dans `Lire_Courant()` ?

Expliquer un décalage à droite avec un petit exemple, puis vérifier le format
du résultat du CAN dans les fichiers générés. Ne pas déduire le nombre de bits
utilisés uniquement du nombre de bits annoncé pour le CAN du PIC.

**Ma réponse :**

**À corriger ou à revoir :**

### 17. Pourquoi certaines variables sont en `uint8_t` et d'autres en `uint16_t` ?

Prendre un exemple de compteur et un exemple de mesure dans mes fichiers.

**Ma réponse :**

**À corriger ou à revoir :**

### 18. Pour un signal alternatif, pourquoi une seule lecture ne suffit-elle pas à connaître son amplitude ?

**Ma réponse :**

**À corriger ou à revoir :**

### 19. Dans `main_test_efficace.c`, que fait le programme quand `SIMULATION` vaut 1 ?

Expliquer le rôle du tableau `sinus[]` et celui du potentiomètre. Est-ce alors
une mesure du courant de la pompe réelle ?

**Ma réponse :**

**À corriger ou à revoir :**

### 20. Pourquoi prévoir 100 échantillons espacés de 200 µs ?

Retrouver la durée visée et expliquer le lien avec un signal à 50 Hz.
Où le Timer2 est-il configuré dans la version qu'on souhaite compiler ?

**Ma réponse :**

**À corriger ou à revoir :**

### 21. Dans ce test, utilises-tu une interruption ou la scrutation de `TMR2IF` ?

Expliquer ce que fait `while (TMR2IF == 0)` et pourquoi le drapeau est remis à zéro.

**Ma réponse :**

**À corriger ou à revoir :**

### 22. Quelle différence y a-t-il entre la moyenne, la crête et la valeur efficace ?

Montrer quelles fonctions calculent ces valeurs. Pourquoi soustraire la moyenne
avant de calculer la valeur efficace de la partie alternative ?

**Ma réponse :**

**À corriger ou à revoir :**

### 23. À quoi sert la moyenne sur les huit dernières valeurs ?

Est-ce que cela change la vitesse à laquelle l'affichage réagit ?

**Ma réponse :**

**À corriger ou à revoir :**

## Reprendre le code simplement

### 24. Dans `main.c`, pourquoi faut-il choisir un seul programme principal ?

Le fichier lu contient deux définitions de `main`, à la ligne 27 et à la ligne 115,
ainsi que des constantes et variables redéfinies. Quelle partie souhaite-t-on garder ?

**Ma réponse :**

**À corriger ou à revoir :**

### 25. Que devrait renvoyer `Mesure_Courant()` ?

Dans la première version de `main.c`, elle termine actuellement par `return 0`.
À quoi servent alors `maxi` et `mini` ? Où faudrait-il prendre la décision de commander le moteur ?

**Ma réponse :**

**À corriger ou à revoir :**

### 26. Quelle partie du code as-tu du mal à expliquer ? Comment la réécrire plus simplement ?

Choisir un seul bloc : les macros `MOTEUR_ON()` / `MOTEUR_OFF()`, un calcul,
une fonction ou une condition. Dire ce que ce bloc doit faire avant de le réécrire.

**Ma réponse :**

**À corriger ou à revoir :**

### 27. Quel petit test ferais-tu pour vérifier chaque étape ?

Par exemple : lire le potentiomètre, afficher la valeur, comparer aux seuils,
puis vérifier les changements d'état avec la LED.

**Ma réponse :**

**À corriger ou à revoir :**

### 28. Qu'est-ce qui manque encore pour passer du test au programme du projet ?

Ne citer que ce qui manque réellement : acquisition retenue, seuils mesurés,
autotest, temporisations, affichage, alarmes…

**Ma réponse :**

**À corriger ou à revoir :**

## Mon bilan après l'entraînement

- [ ] Je peux expliquer le programme simple sans le lire en entier.
- [ ] Je sais distinguer un test au potentiomètre, un signal simulé et une mesure réelle.
- [ ] Je sais expliquer mes seuils, mes compteurs et mes types de variables.
- [ ] J'ai repéré les blocs à simplifier et les essais à refaire.

**Les trois points à reprendre en premier :**

1.
2.
3.

**Une réponse que je veux reformuler :**

**Ma version corrigée, avec mes mots :**

## État des fichiers utilisés

Lecture des sources le 4 novembre 2026, sans compilation ni modification des
fichiers C. Le dossier `VIDE CAVE PARTIE 2` contient des fichiers KiCad ;
son analyse est reportée. Les sources C utilisées pour cette fiche sont dans
`Projet_vide_cave_1.X`, dans la copie locale du projet sous `Documents/Ecole`.
Elles ne sont pas encore présentes dans la copie de travail Orca.

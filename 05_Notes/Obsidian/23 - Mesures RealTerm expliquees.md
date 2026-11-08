---
tags: [vide-cave, oral, mesures]
date: 2026-11-08
---
# Mesures RealTerm expliquées

Retour : [[00 - Accueil]] · Oral : [[22 - Soutenance - Consignes et texte]]

Le tableau à mettre dans l'exposé : `Mesures_RealTerm_simple.xlsx`, dans le dossier
`Soutenance vide-cave` du Bureau.

## Ce qu'on a fait

On a tourné le potentiomètre de la carte SAE22, branché sur RA0. Le PIC lisait la tension
et envoyait le résultat au PC par la liaison série. RealTerm affichait ce résultat, et on a
noté les valeurs dans un tableau.

Le morceau de code qui envoyait la valeur :

```c
valeur = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF);
printf("CAN=%u\r\n", valeur);
```

## Le tableau

| Mesure | Valeur lue sur RealTerm | Valeur sur 8 bits | Tension |
|---|---|---|---|
| 1 | 65520 | 255 | 4,98 V |
| 2 | 65520 | 255 | 4,98 V |
| 3 | 56368 | 220 | 4,30 V |
| 4 | 44592 | 174 | 3,40 V |
| 5 | 23792 | 92 | 1,80 V |
| 6 | 3312 | 12 | 0,23 V |
| 7 | 2544 | 9 | 0,18 V |
| 8 | 2560 | 10 | 0,20 V |

## Ce que ça veut dire

**Pourquoi RealTerm affiche un si grand nombre, comme 65520 ?**
Le résultat du CAN est rangé à gauche dans une case de 16 bits. C'est donc un grand nombre,
même si le vrai résultat est plus petit.

**La valeur sur 8 bits :** on divise par 256. Dans le code, c'est le `>> 8`.

Exemple : 44592 / 256 = 174 (on enlève la virgule).

**La tension :** on multiplie par 5 V, puis on divise par 256.

Exemple : 174 × 5 / 256 = 3,40 V.

**Ce qu'on voit :** plus on tourne le potentiomètre, plus la valeur monte. Elle va de 9 à 255,
donc de 0,18 V à 4,98 V. Elle ne descend pas tout à fait à 0.

**D'où viennent mes seuils :** on a coupé cette plage en 3 zones égales.

(255 − 9) / 3 = 82, donc 9 + 82 = **91** et 91 + 82 = **173**.

- En dessous de 91 : courant faible, pompe à vide.
- Entre 91 et 173 : courant normal.
- Au-dessus de 173 : courant fort, moteur bloqué.

## À dire à l'oral

> On a d'abord testé la lecture analogique avec le potentiomètre de la carte SAE22. On lisait
> les valeurs sur RealTerm et on les a mises dans un tableau. En 8 bits, on va de 9 à 255. On
> a coupé cette plage en trois zones, ce qui nous donne nos deux seuils de test : 91 et 173.
> Ces seuils seront à refaire avec le vrai capteur.

## Si le jury demande

**Pourquoi 8 bits ?**
> C'était suffisant pour tester avec le potentiomètre, et c'est plus simple à traiter.

**Et avec le vrai capteur ?**
> Il donne 20 mV par ampère. En 8 bits, un point vaut 19,5 mV, donc environ 1 A : c'est trop juste
> pour notre pompe. Il faudra passer en 12 bits ou amplifier le signal, on doit en parler avec le prof.

**Pourquoi 2 mesures à 9 et 10 ?**
> Le potentiomètre est au minimum et la mesure bouge un peu : un point, c'est seulement 19,5 mV.

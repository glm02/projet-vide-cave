---
tags: [vide-cave, oral, soutenance, consignes]
date: 2026-11-08
---
# Soutenance : consignes et texte

Retour : [[00 - Accueil]] · Pour le code : [[21 - Fiche oral - Expliquer mon code]]

## 1. Les consignes de l'IUT

### Le format

- **Durée de l'exposé :** 10 minutes pour un binôme, soit environ 5 minutes par personne.
- **Questions du jury :** 10 à 20 minutes. La qualité des réponses compte aussi.
- **Temps de parole :** il doit être équilibré entre les deux.
- **Support :** diaporama, et si possible schéma, photo, démonstration ou objet à montrer.

### Ce qui est évalué

| Critère | Ce que le jury regarde |
|---|---|
| Aspect général | Prestance, respect du temps, bonne utilisation des supports |
| Fond | Sujet, contexte, principe, vision d'ensemble, points intéressants, conclusions |
| Forme | Exposé clair et structuré, plan, transitions, français correct, vocabulaire technique, conviction |
| Compréhension | Un étudiant de notre niveau doit tout comprendre et voir l'ampleur du travail |
| Réponses | Qualité des réponses aux questions |
| Part personnelle | Ce que chacun a fait et ce qu'il a appris |

### L'ordre demandé

1. Se présenter.
2. **Introduction :** synoptique simple du sujet dans son contexte, synthèse du cahier des charges.
3. **Développement :** partie technique et résultats, avec les choix argumentés. Montrer un synoptique détaillé, les schémas et les organigrammes.
4. **Conclusion technique :** ce qui est fait, ce qui reste à faire, **planning actualisé**, comment rattraper le retard ou exploiter une avance.
5. **Conclusion personnelle :** acquis, liens avec les autres matières, méthodes de travail.

Pour chaque idée, se demander : « quel message je veux faire passer ? », puis choisir le meilleur moyen de le montrer (image, schéma, chronogramme, objet, démonstration).

## 2. À ne pas oublier dans le diaporama

### Obligatoire

- [x] Noms, groupe 311, nom de l'enseignant, IUT Lyon 1 GEII, 2026-2027
- [x] Un plan de l'exposé
- [x] Synoptique simple (introduction)
- [ ] Synthèse du cahier des charges
- [ ] Synoptique détaillé avec les noms des signaux (AN0, RB2)
- [x] Les composants : dire qu'ils nous ont été donnés par les profs, et à quoi sert chacun
- [x] Schéma structurel (KiCad)
- [ ] Organigramme ou machine à états du programme
- [ ] Résultats et essais déjà réalisés
- [ ] Ce qui est fait et ce qui reste à faire
- [ ] **Planning actualisé**
- [ ] Retards et moyens de les rattraper
- [ ] Part de travail de chacun
- [ ] Conclusion personnelle : acquis et méthodes de travail

### Ce que tu voulais ajouter

- [ ] Nomenclature : capteur, triac, optotriac, optocoupleur, PIC, LCD, avec leurs images
- [ ] Un petit bout de code expliqué
- [ ] Ce qu'on avait mis dans le rapport, pour la conclusion

### À faire avant demain

- [x] **Planning :** relire `Gantt_vide-cave_a_jour.xlsx` à deux (dossier `Soutenance vide-cave` du Bureau)
- [ ] Photos réelles si vous en avez : carte SAE22, composants
- [ ] Se répartir les parties avec ton binôme
- [ ] Une répétition chronométrée, à voix haute
- [ ] Vérifier que le fichier s'ouvre sur le PC de la salle, et garder une copie PDF

### Les erreurs à éviter

- Ne pas lire les diapos.
- Ne pas présenter comme fait ce qui n'est pas testé : le vrai capteur et le 230 V n'ont pas encore été essayés.
- Ne pas dire « MOC3041 » ni « 130 V ».
- Ne pas mettre trop de texte : une idée par diapo.

## 3. Le plan et le texte

La répartition entre vous deux est une proposition, à adapter. Les durées font 10 minutes au total.

### Partie 1 : se présenter (20 s) · Rafaël

> Bonjour, nous sommes [prénom] et [prénom], du groupe 311. Notre projet porte sur la surveillance d'une pompe de vide-cave. On va présenter le besoin, les composants choisis, le programme, puis ce qu'il nous reste à faire.

**À montrer :** titre, noms, plan.

### Partie 2 : le problème et l'objectif (1 min) · Rafaël

> Une pompe de vide-cave peut rester longtemps sans fonctionner, et elle se trouve dans une cave, où on ne va pas tous les jours. Si elle est bloquée, on risque de ne le découvrir que le jour où il y a de l'eau. Notre objectif est de connaître l'état du moteur sans avoir à descendre, juste en mesurant son courant. Le système est prévu pour un usage domestique.

**À montrer :** synoptique simple ou image de l'installation (préciser que c'est une maquette 3D).

### Partie 3 : le cahier des charges et le principe (1 min) · Rafaël

> Le courant dépend de ce que le moteur doit entraîner. S'il est trop fort, le moteur force : il est bloqué, on le coupe et on alerte. S'il est normal, la pompe vide l'eau et on la laisse tourner. S'il est trop faible, elle tourne à vide et on la coupe. Attention au démarrage : le moteur appelle beaucoup de courant, c'est normal, donc on tolère ce pic quelques secondes. La pompe se met aussi en marche toute seule tous les trois jours.

**À montrer :** les trois cas de courant.

### Partie 4 : l'architecture et ce qui a changé (1 min) · Jules

> À gauche, le 230 V avec la pompe et le triac. À droite, le 5 V avec le PIC. Trois composants font le lien sans contact électrique : le capteur de courant, l'optotriac et l'optocoupleur. Depuis la pré-étude, on est passés du relais au triac. On a ajouté un optocoupleur pour repérer le passage par zéro de la tension. Et on repart de la carte de la SAE22, qu'on modifie : on garde le PIC, la liaison série, des LED et le LCD, on enlève la liaison avec le hacheur.

**À montrer :** synoptique détaillé.

### Partie 5 : les composants (1 min 30) · Jules

| Composant | Référence | Rôle | À retenir |
|---|---|---|---|
| Capteur de courant | ACS756 (Allegro) | Transforme le courant en tension par effet Hall | 2,5 V à 0 A, puis 20 mV/A |
| Triac | BTA12 (ST) | Laisse passer ou non le 230 V vers la pompe | 12 A efficaces au maximum |
| Optotriac | MOC3023 | Déclenche le triac, isolé du PIC | Amorçage à 5 mA max, 400 V crête |
| Optocoupleur | 6N136 | Repère le passage par zéro de la tension | Période de 20 ms, deux zéros par période |
| Microcontrôleur | PIC16F1788 | Lit le courant, décide, commande | CAN 12 bits, RA0 capteur, RB2 commande |
| Afficheur | LCD 162B, 2 × 16 | Affiche l'état du moteur | Mode 4 bits |

> On n'a pas vraiment eu à chercher : ces composants nous ont été donnés par les profs. Nous, on a regardé à quoi ils servent et comment les câbler. Il reste quelques résistances et condensateurs à fixer avant d'imprimer la carte.

**À montrer :** le tableau avec les images, dans le dossier `Soutenance vide-cave/Photos composants` du Bureau.

### Partie 6 : le schéma (45 s) · Jules

> Le PIC commande l'optotriac par RB2, à travers R7. Pour R7 : 5 V moins 1,5 V pour la LED, divisé par 10 mA, ça donne 350 Ω, on prend 330 Ω, à valider. L'optotriac déclenche la gâchette du triac. Ce schéma est une version de travail : il reste à faire passer le courant du moteur dans le capteur, à ajouter le bornier secteur et à revoir R4, qui est en série avec le moteur.

**À montrer :** partie puissance du schéma KiCad.

### Partie 7 : la lecture analogique (1 min 15) · Rafaël

> Le capteur donne une tension, mais le PIC ne comprend que des nombres. C'est le CAN qui transforme la tension en nombre. Pour tester, on a utilisé le potentiomètre de la carte SAE22. Le PIC envoyait le nombre au PC, on le lisait sur RealTerm et on notait les valeurs en tournant le potentiomètre. On a vu la valeur aller de 9 à 255, soit de 0,18 V à 4,98 V. On a coupé cette plage en trois zones, ce qui nous donne deux seuils de test : 91 et 173.

**À montrer :** le tableau RealTerm : voir [[23 - Mesures RealTerm expliquees]].

### Partie 8 : le code (1 min 15) · Rafaël

```c
Courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;

if(Courant >= SEUIL_BAS && Courant <= SEUIL_HAUT)    // courant normal
{
    Compteur = 0;
}
else if(Compteur >= 100)    // anormal depuis 10 s
{
    OPTO_TRIAC_SetLow();    // on coupe le moteur
}
```

> Je ne montre que les morceaux importants. On lit le courant, on garde une valeur de 0 à 255, et on la compare à nos deux seuils. Si le courant est normal, le compteur repart de zéro, donc un pic isolé ne coupe pas la pompe. S'il reste anormal pendant 10 secondes, on coupe le moteur. Pour le temps, le Timer2 réglé dans MCC appelle notre fonction toutes les 100 ms. Le programme suit trois états : démarrage, marche et défaut. Pour l'instant, c'est le potentiomètre qui simule le courant, et le lancement tous les trois jours n'est pas encore codé.

**À montrer :** seulement les morceaux de la note [[25 - Texte des diapos]], pas tout le code.

### Partie 9 : ce qu'on a, ce qu'il reste (1 min) · Rafaël

| Bloc | Ce qu'on a | Reste à faire | Ce qu'on va obtenir |
|---|---|---|---|
| Mesure | Lecture analogique testée au potentiomètre, valeurs relevées sous RealTerm | Brancher l'ACS756, passer à 12 bits, mesurer sur une période | Le vrai courant du moteur |
| Commande | BTA12 et MOC3023 choisis, schéma KiCad | Fixer les résistances, tester | Le PIC coupe la pompe |
| Détection du zéro | 6N136 au schéma | Fixer ses valeurs, le relier au programme | Des mesures au bon moment |
| Programme | Deux seuils testés sur LED, machine à états écrite | Vrais seuils, interruptions, lancement tous les 3 jours | Bloqué, à vide ou en marche reconnu seul |
| Carte | Carte SAE22 pour les essais, typons en cours | Finir le schéma, imprimer, souder | La carte finale |
| Évolutions | Code d'affichage LCD écrit | LCD sur la carte, boutons, ESP32 | Une alerte par mail |

> Ce qui marche aujourd'hui, c'est la lecture analogique et les seuils. On n'a pas encore de résultat avec le vrai capteur et le 230 V, parce que la carte n'est pas imprimée.

### Partie 10 : le planning (30 s) · Jules

> On a un peu de retard, surtout sur le code et la carte. On a perdu du temps à chercher des composants sur internet alors qu'ils étaient à l'IUT. Pour rattraper, on avance bloc par bloc : capteur et triac, puis optocoupleur, puis LED et LCD. La date de fin ne change pas.

**À montrer :** le Gantt à jour.

### Partie 11 : conclusion (30 s) · chacun une phrase

> **Rafaël :** Je me suis occupé de la recherche des composants, du choix des broches du PIC et du code. J'ai surtout appris la lecture analogique, qu'on n'avait pas encore vue en cours, et sa configuration avec MCC.
>
> **Jules :** Je me suis occupé de KiCad, du schéma et des typons. [À compléter avec ses mots.]
>
> **Pour finir :** Le projet mélange électronique, programmation et gestion de projet. On a utilisé GitHub et un Kanban pour s'organiser. Il nous reste à imprimer la carte et à tester avec le vrai capteur. Merci, nous sommes prêts pour vos questions.

## 4. Ce que tu dois savoir expliquer

Le code ligne par ligne : [[24 - Mon code simple ligne par ligne]]. Le reste est dans [[21 - Fiche oral - Expliquer mon code]] :

- `uint8_t`, 8 bits, 12 bits et le quantum
- la lecture analogique et `>> 8`
- la machine à états et le comptage des 10 secondes
- les questions pièges du jury

## 5. Les chiffres à retenir

| Chiffre | Ce que c'est |
|---|---|
| 230 V efficaces, 325 V crête | Le secteur |
| 20 ms | Une période à 50 Hz |
| 2,5 V + 20 mV/A | La sortie du capteur |
| 19,5 mV | Le quantum en 8 bits |
| 1,22 mV | Le quantum en 12 bits |
| 91 et 173 | Les seuils de test au potentiomètre |
| 100 ms | L'appel de notre fonction par le Timer2, puis la mesure dans le `main` |
| 1:16, 1:10, 10 ms, 10 fois | Le réglage du Timer2 dans MCC : 0,25 µs × 16 × 250 × 10 = 10 ms, et 10 × 10 ms = 100 ms |
| 5 s, 10 s | Démarrage toléré, défaut confirmé en marche |
| 5 mA, 330 Ω | Amorçage du MOC3023, valeur prévue pour R7 |

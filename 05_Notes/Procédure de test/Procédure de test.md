---
tags: [vide-cave, tests, carte]
date: 2026-11-16
---
# Procédure de test de la carte

Retour : [[00 - Accueil]] · Suivi : [[Kanban vide-cave.base|Kanban]] · Code : [[24 - Mon code simple ligne par ligne]]

On suit les étapes dans l'ordre. **On ne passe à l'étape suivante que si tout est OK.** Chaque
tableau se remplit pendant le test : valeur mesurée, puis OK ou pas OK. En cas de problème, on
note ce qu'on a vu dans la colonne « Remarque » avant de chercher la cause.

> [!danger] 230 V
> Les étapes 6 et 7 se font **uniquement avec le prof**, carte posée sur un support isolant,
> pompe ou lampe branchée avant la mise sous tension. Jamais la masse de l'oscilloscope côté
> 230 V : on ne mesure que côté 5 V.

## Matériel

- Multimètre, alimentation de labo réglable avec limitation de courant.
- Oscilloscope 2 voies.
- MPLAB SNAP et le projet `08_Firmware/Projet_vide_cave_1.X`.
- Potentiomètre 10 kΩ pour simuler le capteur.
- Pour l'étape 6 : une lampe 230 V (charge simple), puis la pompe.
- Pour l'étape 7 : pince ampèremétrique.

## 1. Carte nue, avant de souder

| Test | Attendu | Mesuré | OK | Remarque |
|---|---|---|---|---|
| Aspect des pistes (loupe) | Pas de piste coupée ni de pont de cuivre | | | |
| Perçages | Tous les trous présents, au bon diamètre | | | |
| Continuité +5 V → broche VDD du PIC (20) | < 1 Ω | | | |
| Continuité GND → broches VSS du PIC (8 et 19) | < 1 Ω | | | |
| +5 V ↔ GND | Circuit ouvert | | | |
| Partie 230 V ↔ partie 5 V | Circuit ouvert, aucune piste qui traverse | | | |

## 2. Soudure

Ordre : du plus bas au plus haut. Résistances, supports de circuits intégrés, condensateurs, LED,
bouton, connecteurs, puis les gros composants (triac, borniers, capteur). **Le PIC va sur un
support**, on ne le met qu'à l'étape 4.

À vérifier en soudant : sens du condensateur C1 (47 µF), de la LED D4, des supports (encoche),
de l'optotriac et du capteur de courant.

## 3. Carte soudée, hors tension

| Test | Attendu | Mesuré | OK | Remarque |
|---|---|---|---|---|
| Inspection des soudures | Brillantes, pas de pont entre pastilles | | | |
| Sens de C1, D4, CI | Conforme au schéma | | | |
| +5 V ↔ GND à l'ohmmètre | Pas 0 Ω (la valeur monte pendant que C1 se charge) | | | |
| Partie 230 V ↔ partie 5 V | Circuit ouvert | | | |

## 4. Mise sous tension 5 V, sans le PIC

Alimentation de labo réglée sur **5 V, limitée à 100 mA**, sur le bornier J1.

| Test | Attendu | Mesuré | OK | Remarque |
|---|---|---|---|---|
| Courant consommé | Quelques mA, pas de limitation | | | |
| Tension entre broches 20 et 19 du support | 5 V | | | |
| Tension sur MCLR (broche 1) | 5 V (rappel au +5 V) | | | |
| Sortie du capteur, sans courant | Environ 2,5 V | | | |
| Rétroéclairage du LCD | Allumé | | | |

## 5. Avec le PIC, sans 230 V

Couper l'alimentation, placer le PIC sur son support (encoche du bon côté), réalimenter.

| Test | Comment | Attendu | Mesuré | OK |
|---|---|---|---|---|
| Programmation | SNAP sur J4, « Make and Program Device » | Programmation réussie | | |
| LCD | Au démarrage | « Demarrage » affiché | | |
| Passage en marche | Attendre 5 s | « En marche » affiché | | |
| Base de temps | Oscilloscope sur une broche basculée dans l'interruption | 10 ms par interruption | | |
| Commande optotriac | Tension sur RB2 (broche 23) | 5 V en marche | | |
| Lecture du courant à vide | Valeur du CAN sur 8 bits (liaison série J2 ou débogueur) | Environ 128 (2,5 V) | | |
| Défaut « A VIDE » | Potentiomètre à la place du capteur, sous 91 pendant 10 s | RB2 à 0 V, « A VIDE » | | |
| Défaut « BLOQUE » | Potentiomètre au-dessus de 173 pendant 10 s | RB2 à 0 V, « BLOQUE » | | |
| Retour à la normale | Valeur entre 91 et 173 | Le compteur repart à zéro, pas de coupure | | |
| Bouton reset SW4 | Appui | Le programme redémarre | | |

## 6. Avec le 230 V, charge simple (avec le prof)

D'abord une lampe à la place de la pompe : si le triac ne commande pas bien, rien ne force.

| Test | Attendu | Mesuré | OK | Remarque |
|---|---|---|---|---|
| Lampe au démarrage | S'allume | | | |
| Coupure sur défaut (potentiomètre) | La lampe s'éteint au bout de 10 s | | | |
| Signal du 6N136 sur RB4, côté 5 V | Créneau de 50 Hz, un front toutes les 20 ms | | | |
| Échauffement du triac après 5 min | Tiède, pas brûlant | | | |

## 7. Avec la pompe (avec le prof)

Ces mesures donnent les **vrais seuils** du code, à la place de 91 et 173.

| Cas | Courant (pince) | Tension sortie capteur | Valeur CAN | LCD et réaction |
|---|---|---|---|---|
| Moteur arrêté | 0 A | | | |
| Pic de démarrage (valeur max, durée) | | | | |
| Pompe dans l'eau, marche normale | | | | |
| Pompe à vide (hors de l'eau) | | | | |
| Rotor bloqué (court, avec le prof) | | | | |

Après ce tableau : choisir les nouveaux seuils au milieu des écarts entre les cas, les mettre dans
`SEUIL_BAS` et `SEUIL_HAUT`, reprogrammer, puis refaire l'étape 7.

## Bilan

| Date | Version du code | Étapes réussies | Problèmes restants |
|---|---|---|---|
| | | | |

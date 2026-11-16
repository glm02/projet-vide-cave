---
tags: [vide-cave, suivi]
date: 2026-10-12
---
# Tâches

Retour : [[00 - Accueil]] · Constats : [[04 - Anomalies et verifications]]

## Organisation et nettoyage

- [x] Identifier la carte G206 de travail et distinguer les autres versions.
- [x] Sauvegarder le PCB avant nettoyage.
- [x] Retirer du PCB les empreintes héritées absentes du schéma.
- [x] Conserver les composants du schéma et la géométrie des empreintes gardées.
- [x] Documenter la fabrication B.Cu bleu / straps F.Cu rouges.
- [x] Préparer un coffre Obsidian portable et les exclusions Git.
- [ ] Consulter le bilan final de nettoyage et les erreurs restantes dans le rapport d'analyse.
- [ ] Vérifier le dépôt distant et tester une récupération sur un autre poste.

## Données et décisions avant la puissance

- [ ] Relever la plaque de la pompe : tension, puissance, courant nominal, démarrage et technologie du moteur.
- [ ] Choisir les seuils Imin/Imax et justifier le mode de détection d'anomalie.
- [ ] Décider si la mesure de déphasage est maintenue ; définir la voie de tension / zéro nécessaire.
- [ ] Choisir les MPN réels du capteur, de l'optotriac et du triac de puissance.
- [ ] Définir les protections et le raccordement phase/neutre/moteur ; faire revoir l'isolement et la partie secteur.

## Schéma puis PCB

- [ ] Corriger la liaison mesure U3 → PIC et les nets de commande PIC → optotriac.
- [ ] Câbler la voie IP+/IP− dans la chaîne moteur.
- [ ] Renseigner R3, R4 et R7 : valeurs, empreintes et réseaux corrects.
- [ ] Préciser la polarité, la référence et l'empreinte BZ1.
- [ ] Renseigner les composants de puissance et leurs empreintes.
- [ ] Revoir le découplage PIC/capteur.
- [ ] Vérifier les changements schéma → PCB avant synchronisation.
- [ ] Implanter et router les blocs terminés suivant [[02 - Regles PCB]].
- [ ] Lancer ERC, DRC, comparaison schéma/PCB et revue de l'isolement après ces modifications.
- [ ] Régénérer les exports utiles uniquement à partir de la version validée.

## Logiciel et dossier scolaire

- [ ] Ajouter le firmware et ses instructions de compilation/programmation.
- [ ] Vérifier oscillateur interne, VCAPEN et ANSEL pour les broches LCD.
- [ ] Définir le démarrage, la mesure de courant, l'autotest, les alarmes et les cas de panne.
- [ ] Actualiser la pré-étude : PIC/triac, place éventuelle de l'ESP32, courant et déphasage.
- [ ] Vérifier la cohérence du synoptique, de la présentation, de la maquette et du schéma final.
- [ ] Consigner les essais et résultats dans [[08 - Journal de travail]].

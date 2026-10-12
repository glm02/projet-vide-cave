---
tags: [vide-cave, pcb, contraintes]
date: 2026-10-12
---
# Règles PCB

Retour : [[00 - Accueil]] · Contexte : [[03 - Decisions et historique]]

## Fabrication décidée

**B.Cu (bleu) porte le routage courant. F.Cu (rouge) est réservé à peu de straps filaires.** L'objectif est une carte principalement simple face, avec des ponts ponctuels. Ne pas transformer le projet en routage double face généralisé.

La couleur dépend de l'affichage KiCad ; les noms de couches font référence. Il n'existe pas de fichier `.kicad_dru` qui interdise automatiquement les pistes F.Cu dans G206. Un DRC sans erreur ne suffirait donc pas à prouver le respect de cette consigne.

## Paramètres existants à préserver

Source : paramètres `.kicad_pro` et zone GND du PCB, relevés par l'analyse du 12 octobre 2026. Ces valeurs décrivent l'état du projet ; elles ne démontrent pas à elles seules une conformité d'isolement secteur.

| Paramètre | Valeur |
| --- | ---: |
| Largeur minimale de piste | 0,6 mm |
| Classe Default : largeur | 0,6 mm |
| Classe Power : largeur | 0,7 mm |
| Réseaux associés à Power | +5V, GND |
| Dégagement général | 0,5 mm |
| Distance cuivre/bord | 1 mm |
| Perçage minimal | 0,7 mm |
| Anneau annulaire minimal | 0,4 mm |
| Espacement entre trous | 1 mm |
| Largeur minimale de connexion cuivre | 0,6 mm |
| Zone GND : couche | B.Cu |
| Zone GND : dégagement | 1 mm |
| Zone GND : ouverture thermique | 0,61 mm |
| Zone GND : largeur des branches thermiques | 0,61 mm |
| Zone GND : cuivre minimal | 0,6 mm |

## Straps identifiés

| Réseau | Longueur | Départ → arrivée, coordonnées KiCad en mm |
| --- | ---: | --- |
| GND | 14 mm | (113,5 ; 135) → (127,5 ; 135) |
| MCLR | 10 mm | (173 ; 110) → (183 ; 110) |

Les vias associés sont de diamètre 2 mm avec perçage 0,8 mm. Leur interprétation comme passages de straps doit être confirmée dans le dossier de fabrication. Le comptage final des pistes et le résultat DRC sont dans le [rapport technique](../Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md).

## Revue à chaque modification

- Vérifier que les nouveaux chemins restent en B.Cu.
- Justifier chaque strap F.Cu et vérifier sa possibilité de montage.
- Conserver les paramètres ci-dessus tant qu'aucune nouvelle décision n'est prise.
- Remplir de nouveau les zones après modification du cuivre.
- Lancer le DRC et distinguer les défauts de cuivre des écarts schéma/PCB.
- Ne pas toucher aux composants conservés pour simplement nettoyer les éléments de l'ancienne carte.

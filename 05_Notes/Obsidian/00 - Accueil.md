---
tags: [vide-cave, accueil]
date: 2026-10-12
---
# Projet vide-cave

Ce coffre rassemble le suivi du projet GEII. La carte de travail est **G206_CAMEL_TP_CAO**, issue d'une carte ancienne adaptée. Les sources de la variante PIC/Allegro/TRIAC restent indépendantes.

## Navigation

- [[01 - Fonctionnement et architecture]] : objectifs, blocs et réalisation actuelle.
- [[02 - Regles PCB]] : bleu B.Cu, straps rouges F.Cu, paramètres conservés.
- [[03 - Decisions et historique]] : choix validés et traçabilité du nettoyage.
- [[04 - Anomalies et verifications]] : points à corriger ou confirmer.
- [[05 - Taches]] : ordre de travail et cases à cocher.
- [[06 - Documents et coherence]] : liens vers les sources et écarts avec la pré-étude.
- [[07 - Guide Git et portabilite]] : récupérer le projet et synchroniser ses modifications.
- [[08 - Journal de travail]] : consigner les prochaines séances.
- [[09 - Travail a deux et MOC3041]] : accès GitHub, proposition de commande et simulations.

## État à retenir

Le nettoyage vise les éléments hérités uniquement sur le PCB. Les composants présents au schéma doivent rester ; les éléments conservés ne doivent pas être déplacés ou remplacés pendant ce nettoyage. Le schéma garde des problèmes électriques et des informations manquantes qui doivent être traités séparément.

La référence technique détaillée est le [rapport d'analyse du 12 octobre 2026](../Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md). Les notes ci-dessous synthétisent ses constats ; elles ne constituent pas un second contrôle électrique indépendant.

## Ouvrir les sources

- [Projet KiCad G206](../../03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_pro)
- [Composants G206, une ligne par référence](../../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv) et [nomenclature groupée](../../03_Electronique/G206A_CAMEL_TP_CAO/Nomenclature_G206_groupee.csv)
- [Rapport Word](../../01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx)
- [Synoptique V3](../../02_Synoptiques/Synoptique%20V3.drawio)
- [Maquette Blender](../../04_Maquette_3D/Vide_cave_GEII.blend)
- [Visualisation Anatomy à ouvrir dans un navigateur](../../07_Visualisation_Anatomy/index.html), [aperçu](../../07_Visualisation_Anatomy/preview.png) et [notice](../../07_Visualisation_Anatomy/README.md)
- [README du dépôt](../../README.md)
- [Dépôt GitHub](https://github.com/glm02/projet-vide-cave)
- [Ancienne note INFO 3 conservée](../PROJET%20VIDE%20CAVE/INFO%203.md)

Les liens vers les fichiers techniques sortent du coffre mais restent relatifs à sa position dans le dépôt. Selon le système, ouvrir les `.kicad_pro`, `.docx` ou `.blend` dans leur application associée ou via l'explorateur de fichiers.

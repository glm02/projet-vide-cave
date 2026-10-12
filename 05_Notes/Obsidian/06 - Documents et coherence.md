---
tags: [vide-cave, documents]
date: 2026-10-12
---
# Documents et cohérence

Retour : [[00 - Accueil]] · Décisions : [[03 - Decisions et historique]]

## Documents sources

| Source | Utilisation |
| --- | --- |
| [Rapport Word de pré-étude](../../01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx) | Besoin, objectifs et premières solutions ; à actualiser |
| [Présentation](../../01_Documents/Presentation/projet_vide_cave_geii.pptx) | Support de soutenance |
| [Planning](../../01_Documents/Planning/GANT%20P2506736.ods) | Calendrier du travail |
| [Synoptique V3](../../02_Synoptiques/Synoptique%20V3.drawio) | Version modifiable du diagramme fonctionnel |
| [Synoptique version 2](../../02_Synoptiques/Synoptique%20projet%20version%202.drawio) | Version précédente à garder comme référence |
| [Consignes de pré-étude](<../../01_Documents/Consignes/Consignes rapport pré-étude.pdf>) | Exigences scolaires |
| [Consignes de compte rendu](../../01_Documents/Consignes/Le%20compte%20rendu%20de%20conception.pdf) | Contenu du compte rendu |
| [Dossier de fabrication](../../01_Documents/Consignes/le_dossier_de_fabrication_23d.pdf) | Documents attendus pour la réalisation |
| [Préparer la soutenance](../../01_Documents/Consignes/Préparer%20la%20soutenance%20orale.pdf) | Préparation de l'oral |
| [Connexion PICkit](../../03_Electronique/Documentation/pickit_connexion.pdf) | Programmation du PIC |
| [Analyse du projet](../Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md) | Preuves des écarts et revue technique |
| [Composants G206](../../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv) | 24 références du schéma, valeurs, empreintes, MPN, datasheets et écarts avec le PCB |
| [Nomenclature groupée](../../03_Electronique/G206A_CAMEL_TP_CAO/Nomenclature_G206_groupee.csv) | 19 groupes de composants ; somme des quantités : 24 |

## Écarts à décider, puis à rédiger

| Sujet | Pré-étude / synoptique | Sources G206 | Mise à jour attendue |
| --- | --- | --- | --- |
| Unité de commande | ESP32 pressenti dans la pré-étude | PIC16F1788 | Expliquer le choix retenu et ses fonctions |
| Commutation | Relais envisagé | Optotriac et triac | Justifier le choix, références et protections |
| Mesures | Courant et déphasage | Capteur de courant incomplet, pas de voie de tension | Ajouter la référence de phase ou revoir la spécification |
| Autotest | Tous les trois jours, durée ≤ 1 minute | Firmware absent | Décrire la temporisation et les conditions de fonctionnement |
| Alarmes | LCD, LED, buzzer | Partie alarme à préciser | Définir les sorties et les comportements attendus |
| Paramètres | Seuils laissés à compléter | Données moteur inconnues | Relever la plaque et renseigner les critères |

Un choix ancien dans le rapport n'est pas automatiquement une erreur électronique. Il devient un problème documentaire si le dossier final présente ce choix comme la réalisation actuelle.

La maquette Blender et la visualisation Anatomy sont des supports d'explication. Leur aspect visuel ne prouve ni la continuité électrique ni la validation de la fabrication. Les anciens exports Gerber ne constituent pas la source de référence du PCB actuel.

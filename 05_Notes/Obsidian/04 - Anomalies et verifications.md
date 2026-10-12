---
tags: [vide-cave, analyse, a-verifier]
date: 2026-10-12
---
# Anomalies et vérifications

Retour : [[00 - Accueil]] · Actions : [[05 - Taches]]

Ces points reprennent le [rapport d'analyse](../Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md), qui contient les preuves et les limites des contrôles. Le schéma n'est pas corrigé dans le cadre du nettoyage du PCB. Les chiffres DRC finaux doivent être lus dans ce rapport.

## Connexions à corriger ou compléter

| Point | Constat transmis par l'analyse | Conséquence |
| --- | --- | --- |
| Mesure vers le PIC | U3.3 sur `AN0`, U1.2 sur `capteur de courant` | Les noms distincts ne relient pas ces broches |
| Commande de l'optotriac | U1.23 sur `commande octo coupleur `, R7.1 sur `RB2` | La commande PIC et la branche R7 ne forment pas le même net |
| Courant moteur | U3.IP+ (4) et IP− (5) non câblés | Aucun courant moteur ne traverse la voie de mesure représentée |
| Déphasage | Pas de référence de tension / zéro secteur | Fonction annoncée dans la pré-étude non réalisable avec cette seule acquisition |
| Raccordement secteur | Symbole AC à une broche, absence de bornier phase/neutre complet | La chaîne secteur/puissance doit être explicitée |
| Synchronisation R3/R4/R7 | Valeurs `R`, empreintes vides au schéma ; anciennes valeurs 10 kΩ / 10 kΩ / 270 Ω sur PCB, écarts de nets | Une mise à jour automatique sans préparation serait ambiguë |
| Buzzer BZ1 | `+` relié à GND, `−` vers R9/PIC, empreinte absente | Référence, polarité et mode de commande à préciser |
| Implantation puissance | U3 et TRIAC1 hors PCB, triac sans MPN ni empreinte | Partie commande/puissance à concevoir après correction des sources |

## Références et vérifications conditionnelles

| Point | Niveau de confiance / limite |
| --- | --- |
| Symbole FODM3011 | Le nominal de 250 V associé au symbole est inférieur au pic d'un secteur 230 V (~325 V). La valeur « optotriac » ne suffit pas à identifier le composant réel : le risque est conditionnel au MPN retenu |
| Symbole ACS756 | Le symbole visé correspond à une variante ±100 A, 20 mV/A, décalage de sortie de 2,5 V à courant nul. Confirmer la référence commandée et l'adaptation à la pompe |
| Découplage du capteur | Condensateur local de 100 nF absent du montage représenté ; à définir avec la référence réelle |
| Découplage PIC | C2 est à environ 33,5 mm de VDD (broche 20), d'après la revue géométrique |
| Firmware PIC | RA6/RA7 utilisées pour le LCD : oscillateur interne, VCAPEN désactivé et configuration ANSEL numérique à vérifier ; firmware non fourni |
| Alimentation/reset/ICSP PIC | Connexions nominales jugées cohérentes dans le rapport ; cela ne valide pas le fonctionnement de l'ensemble |

## Ne pas corriger un faux positif

LCD1 utilise le brochage **162B particulier** : 1 LEDA, 2 LEDK, 3 VSS, 4 VDD, 5 VO, 6 RS, 7 RW, 8 E, 9 à 16 DB0 à DB7. Le symbole issu de HY1602E a été modifié volontairement. Un avertissement de comparaison à la bibliothèque générique n'est pas une preuve de mauvais brochage du LCD. Conserver le contrôle contre la documentation fabricant décrit dans le rapport.

Les avertissements de différence avec une ancienne bibliothèque et les écarts volontaires liés à l'adaptation doivent être triés. Aucun résultat d'analyse automatique ne doit justifier seul la suppression d'un composant du schéma.

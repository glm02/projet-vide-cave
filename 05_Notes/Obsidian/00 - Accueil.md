---
tags: [vide-cave, accueil]
date: 2026-11-16
---
# Projet vide-cave

## En ce moment

**Le suivi : [[Kanban vide-cave.base|Kanban vide-cave]].** Une carte = une tâche, à glisser d'une colonne à l'autre.

Ordre : procédure de test → Gerbers → soudure → code → tests → carte V2.

- **La procédure : [[Procédure de test]]**, à remplir pendant les essais.
- **Rafaël :** les mesures de test et la finition du code (`08_Firmware/Projet_vide_cave_1.X/main.c`).
- **Jules :** le KiCad, puis la carte V2 avec l'optotriac (`03_Electronique/G206A_CAMEL_TP_CAO V2`).

## Planning (on travaille le lundi)

| Lundi | Rafaël | Jules |
|---|---|---|
| 16/11 | Procédure de test, choix 12 bits, zéro du capteur, test des 3 états sur SAE22 (3 h 30) | Gerbers, base de temps 100 ms (2 h 30) |
| 23/11 | Peaufiner le code (2 h) | Souder la carte (3 h) |
| 30/11 | Tester la carte soudée (2 h 30) | Tester la carte soudée, interruption du 6N136 (4 h) |
| 07/12 | Nouveaux seuils, version finale du code (2 h) | Mesures sur la pompe avec le prof (3 h) |
| 14/12 | 6N136 et broches sur le schéma V2 (3 h) | Optotriac sur le schéma V2 (2 h) |
| 04/01 | Marge | Partie puissance de la V2 (3 h) |
| 11/01 | Routage et isolement de la V2 (4 h) | Routage et isolement de la V2 (4 h) |

**Carte actuelle testée et code fini : 07/12/2026. Carte V2 prête à envoyer : 11/01/2027.**
Total : Rafaël 17 h, Jules 21 h 30. Hypothèses : la carte revient de fabrication en une semaine,
pas de séance les 21/12 et 28/12 (vacances de Noël).

## Comprendre le projet

1. [[24 - Mon code simple ligne par ligne]] : le code expliqué.
2. [[23 - Mesures RealTerm expliquees]] : les premières mesures et les seuils 91 et 173.
3. [[26 - Optocoupleur 6N136 explique]] : la détection du zéro pour la V2.
4. [[21 - Fiche oral - Expliquer mon code]] : bits, `uint8_t`, CAN et questions pièges.
5. [[20 - Idees d'ameliorations]] : les améliorations possibles.

## Soutenance et dossier

- [[27 - Fiche memo oral]], [[25 - Texte des diapos]], [[22 - Soutenance - Consignes et texte]].
- [[13 - Questions pour rediger le dossier de conception]].

## Le reste

Les anciennes notes sont dans `05_Notes/Archives`. L'étude de l'optotriac MOC3041 est dans
`05_Notes/Etude_MOC3041` : elle resservira pour la V2.

---
tags: [vide-cave, decisions, historique]
date: 2026-10-12
---
# Décisions et historique

Retour : [[00 - Accueil]] · Suivi : [[08 - Journal de travail]]

| Date | Décision | Conséquence pratique |
| --- | --- | --- |
| 2026-10-12 | G206_CAMEL_TP_CAO est la carte à travailler | Ne pas la remplacer par la variante indépendante PIC/Allegro/TRIAC |
| 2026-10-12 | Des composants de l'ancienne carte ont été supprimés volontairement du schéma | Nettoyer leurs restes sur le PCB, pas les réintroduire dans le schéma |
| 2026-10-12 | Garder les composants présents au schéma et les autres composants conservés | Pas de déplacement ni remplacement pendant le nettoyage |
| 2026-10-12 | Routage B.Cu bleu, peu de straps F.Cu rouges | Éviter une refonte double face ; conserver les règles existantes |
| 2026-10-12 | Anatomy est un support visuel | Ne pas l'utiliser comme générateur de PCB |
| 2026-10-12 | Utiliser un dépôt GitHub commun et un coffre Obsidian | Sources récupérables sur d'autres postes, décisions et tâches documentées |

## Nettoyage de l'ancienne implantation

Le relevé initial comptait 24 composants au schéma et 42 empreintes sur le PCB. 21 empreintes héritées ont été retirées du PCB : **C3, C4, D1, D2, D3, J3, R1, R2, R5, R6, R8, RV1, SW1, SW2, SW3 et TP1 à TP6**.

Le nettoyage conserve 21 empreintes, dont le contenu a été comparé à la sauvegarde. Les composants du schéma qui n'avaient pas d'empreinte implantée ne sont pas supprimés du schéma. Au total, 98 segments, 2 vias et 42 raccordements en goutte d'eau hérités ont été retirés. Le contrôle final annonce 99 violations DRC, 3 connexions manquantes et 15 écarts de parité, avec 0 marqueur de court-circuit et 0 piste pendante. Consulter le [rapport final](../Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md) pour leur qualification ; ne pas déduire la validité électrique du seul nettoyage.

## Sauvegarde avant changement

Une sauvegarde locale a été créée dans :

```text
03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO-backups/
G206_CAMEL_TP_CAO-2026-10-12_avant_nettoyage_Codex.zip
```

Les sauvegardes automatiques KiCad sont ignorées par Git. Cette archive reste sur le PC d'origine. L'historique Git commence avec le dépôt publié et ne recrée pas les versions antérieures à son premier commit. Le [tableau comparatif initial](../Analyse_G206_2026-10-12/comparaison_tous_composants.csv) sert de trace du constat avant nettoyage, pas d'inventaire du PCB final.

## Prochaine décision à enregistrer

Après validation de l'analyse : définir les références réelles de puissance, les raccordements secteur/moteur, la mesure du courant, la stratégie de déphasage et les changements nécessaires du schéma. Une correction de ces points doit faire l'objet d'une modification dédiée, distincte du nettoyage déjà demandé.

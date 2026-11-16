---
statut: À faire
qui: Les deux
partie: Carte V2
etape: 6 - Carte V2
duree: 2
date: 2026-12-14
---
Sur le schéma V2, la patte 3 de U1 (cathode de la LED du 6N136) est reliée au GND du 5 V : le 230 V rejoint la masse basse tension par R2, R1 et la LED, l'optocoupleur n'isole plus rien.

1. Relier la patte 3 à AC_N.
2. Ajouter une diode tête-bêche sur la LED (5 V maximum en inverse).
3. Refaire le routage autour de U1 (R1-Pad2 passe côté 230 V), puis DRC et ERC.

Ne pas envoyer les Gerbers de la V2 avant. Voir [[Rapport routage V2 - 16-11-2026]].

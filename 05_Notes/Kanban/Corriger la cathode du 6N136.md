---
statut: Fait
qui: Les deux
partie: Carte V2
etape: 6 - Carte V2
duree: 2
date: 2026-12-14
---
Sur le schéma V2, la patte 3 de U1 (cathode de la LED du 6N136) était reliée au GND du 5 V : le 230 V rejoignait la masse basse tension par R2, R1 et la LED, l'optocoupleur n'isolait plus rien.

**Corrigé le 16/11/2026 :** patte 3 sur AC_N, diode D1 (1N4148) tête-bêche sur la LED, R1 et R2 rapprochées de J3, zone de U1 re-routée, moteur et neutre inversés sur J4. DRC 0, ERC 0, 3 mm minimum entre 230 V et 5 V. Détails dans [[Rapport routage V2 - 16-11-2026]].

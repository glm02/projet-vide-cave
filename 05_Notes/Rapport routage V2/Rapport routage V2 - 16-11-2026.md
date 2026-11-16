---
date: 2026-11-16
sujet: Carte V2 — rangement, routage et isolement
---
# Rapport : routage de la carte V2 (16/11/2026)

> [!abstract] En bref
> La carte V2 (triac, MOC3041, 6N136, ACS756) est entièrement re-routée **en simple face, couche B.Cu (bleu)**. Pistes 230 V en **2 mm**, pistes 5 V en **1 mm**, au moins **3 mm** entre 230 V et 5 V, aucun angle droit. KiCad ne trouve plus **aucune erreur ni aucun avertissement** (DRC 0, ERC 0, carte et schéma cohérents).
>
> **À régler avant d'envoyer les Gerbers :** la cathode du 6N136 (U1 patte 3) est sur GND au lieu de AC_N, voir [[#Problème de schéma à corriger]].

![[Routage V2 avant apres.png]]
*À gauche le routage de départ, à droite le routage final.*

## Où sont les fichiers

| Quoi | Où |
|---|---|
| Copie propre du projet KiCad (à utiliser) | `03_Electronique/G206A_CAMEL_TP_CAO V2 routée 2026-11-16` |
| Gerbers et perçage (B.Cu, Edge.Cuts, .drl) | même dossier, sous-dossier `Gerber` |
| Dossier de travail d'origine | `Documents/bordel camel a trier/G206A_CAMEL_TP_CAO V2` |
| Sauvegardes de chaque étape | dossier d'origine, `G206_CAMEL_TP_CAO-backups` |

Le dossier `03_Electronique/G206A_CAMEL_TP_CAO V2` (version d'octobre) n'a pas été modifié.

## 1. Rangement du dossier

Dans le dossier d'origine :

- **Supprimé** (KiCad les recrée) : les 3 fichiers `~*.lck` restés après un arrêt brutal de KiCad, et `fp-info-cache`.
- **Mis de côté dans `_a_supprimer`** (5,5 Mo, rien n'est effacé) : les Gerbers de février (ancienne carte SAE22), deux zips de février, l'ancien schéma `avant_detection_zero`, une copie de restauration, deux nomenclatures CSV périmées (elles listaient un buzzer et une LED qui ne sont plus sur le schéma) et 22 sauvegardes zip en double.

## 2. Problèmes trouvés au départ

| Problème | Gravité |
|---|---|
| Piste **AC_L (230 V) passée sous l'optotriac, entre ses deux rangées**, à 1 mm d'une piste GND : l'isolement du MOC3041 était annulé | Grave |
| Moteur L à 1,0–1,5 mm du +5V, AC_L à 1,4 mm de la LED du MOC, AN0 à 1,7 mm de IP- : 36 endroits sous 3 mm entre 230 V et 5 V | Grave |
| Anneaux de cuivre trop fins sur U2 (empreinte MOC3041 téléchargée) et U3 | Erreur DRC |
| Isolement de 0,46 mm entre +5V et GND sur U3 (pas de 1,91 mm de l'ACS756) | Erreur DRC |
| Bibliothèque MOC3041M absente du projet, 10 empreintes différentes de leur bibliothèque, 33 traits de sérigraphie sur les pastilles | Avertissements |

## 3. Ce qui a été fait sur la carte

### Routage
- **Tout le routage refait sur B.Cu uniquement** (simple face, rien en F.Cu).
- **Pistes 230 V en 2 mm** : Phase L, Neutre N, Moteur L, gâchette, commande gâchette (R3), R1/R2. Rétrécies à 1 mm sur quelques millimètres seulement à la sortie des pattes du triac (pas de 2,54 mm). IP+ et IP- gardent 4 et 5 mm.
- **Pistes 5 V en 1 mm** : +5V, GND, AN0, RB2, LED du MOC, détection du zéro, sortie du 6N136.
- **Aucun angle droit** : chanfreins à 45° de 1,5 mm, et plus aucun angle aigu (piège à acide à la gravure).
- **Pas de passage sous l'optotriac** : AC_L arrive sur U2 par l'extérieur, le GND passe seul entre les deux rangées côté 5 V.
- **Ponts sous les résistances** : sur une carte simple face, une piste peut passer entre les deux pattes d'une résistance axiale. Le routage s'en sert sous R3 (AC_L) et sous R5 (GND de C1).

### Isolement
- **Classe de pistes « Secteur »** (2 mm) pour tous les nets 230 V.
- **Règle KiCad** (fichier `G206_CAMEL_TP_CAO.kicad_dru`) : au moins 3 mm entre le secteur et la basse tension. KiCad refuse désormais toute piste trop proche. Les broches non connectées sont exclues de la règle.
- **Résultat :** minimum 3,00 mm entre 230 V et 5 V, au moins 2 mm entre deux pistes 230 V (sauf au pied du triac et entre IP+ et IP-, qui sont au même potentiel).

### Composants
- **U2 (MOC3041)** : empreinte remplacée par `DIP-6_W7.62mm` standard (anneaux corrects), en carte et au schéma.
- **R7** remontée de 5 mm (trop près du triac) et **J4** décalée de 1,35 mm vers la gauche (pour laisser passer les pistes 5 V le long du bord droit).
- **U3 (ACS756)** : règle d'exception pour ses broches fines (pas de 1,91 mm imposé par le boîtier).
- **Sérigraphie** : traits coupés là où ils passaient sur les pastilles (8 composants), référence J2 déplacée.
- **Bibliothèques du projet** : les empreintes de la carte (pastilles agrandies pour l'IUT) sont enregistrées dans `206A_CAMEL_empreintes` ; le symbole MOC3041M dans `MOC3041M.kicad_sym` ; les symboles ACS756 et 6N136 figés dans `G206_CAMEL_composants.kicad_sym` (pour ne pas décaler les broches avec la bibliothèque de KiCad 10).

### Textes
- **Gravés dans le cuivre** : seulement les repères utiles (AC_L, AC_N, ML, 5V, GND, AN0, RB2, noms des composants) et le titre **SAE31 VIDE-CAVE**. Taille 1,5 mm, trait 0,3 mm, en miroir pour se lire côté cuivre.
- **Annotations** (Phase L, Neutre N, Moteur L, Gâchette, Détection zéro, AN0, +5V, GND) sur le calque **Cmts.User** : visibles dans KiCad, **non exportées dans les Gerbers**.
- Le texte cuivre « Triac de puissance » a été retiré (plus de place), il est devenu une annotation.

## 4. Vérifications

| Contrôle | Résultat |
|---|---|
| DRC (règles du projet + 3 mm secteur) | 0 erreur, 0 avertissement |
| Pistes non connectées | 0 |
| Parité schéma / carte | 0 écart |
| ERC | 0 erreur, 0 avertissement |
| Isolement 230 V / 5 V | 3,00 mm minimum |
| Couches | tout en B.Cu |

Non vérifié : un contrôle visuel des Gerbers dans la visionneuse de KiCad, et la largeur des pistes moteur face au vrai courant de démarrage de la pompe (voir [[Mesurer le courant de démarrage]]).

## Problème de schéma à corriger

> [!warning] Cathode du 6N136 sur la mauvaise masse
> La patte 3 de U1 (cathode de la LED du 6N136) est reliée au **GND du 5 V**. Le secteur passe donc par R2, R1 et la LED jusqu'à la masse basse tension : l'optocoupleur **n'isole plus rien**, et la carte relie le 230 V au 5 V à travers les résistances.
>
> **Correction :** relier la patte 3 à **AC_N** et ajouter une **diode tête-bêche** sur la LED (elle ne supporte que 5 V en inverse). Il faudra alors refaire le routage autour de U1, car R1-Pad2 passera côté 230 V.
>
> **Ne pas envoyer les Gerbers avant cette correction.**

## Reste à faire

- [ ] Corriger la cathode du 6N136 et ajouter la diode (à décider avec Jules)
- [ ] Reprendre le routage autour de U1 après cette correction
- [ ] Contrôler les Gerbers dans la visionneuse de KiCad
- [ ] Vérifier la largeur des pistes moteur avec le courant de démarrage mesuré

![[Routage V2 final.png]]
*Routage final, côté cuivre, avec les annotations du calque Cmts.User.*

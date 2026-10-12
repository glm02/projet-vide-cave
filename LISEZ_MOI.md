# Projet vide-cave

Le dossier est organisé par type de travail. Les fichiers d'origine, bibliothèques,
sauvegardes et versions ont été conservés.

```text
Projet vide cave/
├── 01_Documents/
│   ├── Consignes/        PDF de consignes et supports de cours
│   ├── Rapport/          Rapport de pré-étude Word
│   ├── Presentation/     Diaporama PowerPoint
│   └── Planning/         Planning Gantt
├── 02_Synoptiques/       Draw.io, images, PDF et sauvegardes associées
├── 03_Electronique/
│   ├── Carte_vide_cave_PIC_Allegro_TRIAC/   Nouvelle carte, schéma et typons
│   ├── G206A_CAMEL_TP_CAO/                 Projet CAO fourni
│   ├── Projets_KiCad_initiaux/             Autres fichiers KiCad existants
│   └── Documentation/                     Connexion PICkit
├── 04_Maquette_3D/       Fichier Blender, rendus, scripts et notice
├── 05_Notes/             Coffre Obsidian PROJET VIDE CAVE
└── 06_Archives_et_exports/
    ├── Carte_vide_cave_PIC_Allegro_TRIAC.zip
    ├── Claude outputs/  Anciens schémas et images exportés
    └── Rangement/       Journal des déplacements et vérification
```

## Accès rapide

- [Nouvelle carte : ouvrir le projet KiCad](<C:/Users/rafae/Documents/Ecole/Projet vide cave/03_Electronique/Carte_vide_cave_PIC_Allegro_TRIAC/Vide_cave_5A.kicad_pro>).
- [Notice de la carte et raccordements](<C:/Users/rafae/Documents/Ecole/Projet vide cave/03_Electronique/Carte_vide_cave_PIC_Allegro_TRIAC/LIRE_MOI.md>).
- [Maquette Blender](<C:/Users/rafae/Documents/Ecole/Projet vide cave/04_Maquette_3D/Vide_cave_GEII.blend>) et [notice de la maquette](<C:/Users/rafae/Documents/Ecole/Projet vide cave/04_Maquette_3D/LISEZ_MOI.md>).
- [Rapport Word](<C:/Users/rafae/Documents/Ecole/Projet vide cave/01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx>).
- [Présentation PowerPoint](<C:/Users/rafae/Documents/Ecole/Projet vide cave/01_Documents/Presentation/projet_vide_cave_geii.pptx>).
- [Planning](<C:/Users/rafae/Documents/Ecole/Projet vide cave/01_Documents/Planning/GANT P2506736.ods>).
- [Synoptique V3 modifiable](<C:/Users/rafae/Documents/Ecole/Projet vide cave/02_Synoptiques/Synoptique V3.drawio>).

Les typons et Gerbers se trouvent dans `03_Electronique/Carte_vide_cave_PIC_Allegro_TRIAC/exports/`.
La carte est un prototype pour une pompe 230 V / environ 300 W ; consulter sa
notice et faire valider le montage avant fabrication et mise sous tension.
La scène électronique de la maquette Blender représente le projet CAO fourni,
pas la nouvelle carte avec triac.

## Conservation des fichiers

Le rangement a déplacé 24 éléments et vérifié par SHA-256 que les 181 fichiers
présents au départ avaient conservé leur contenu. Les chemins du script de lecture
KiCad et de la notice de la maquette ont ensuite été adaptés à cette organisation.
Une copie des versions précédentes de ces fichiers est conservée dans
`06_Archives_et_exports/Rangement/Versions_avant_adaptation/`.

Blender était encore ouvert pendant le rangement. Un alias de dossier **masqué**
`maquette_3d` pointe vers `04_Maquette_3D` pour que les anciens chemins continuent
de fonctionner. Il ne contient pas une seconde copie de la maquette. Utiliser
désormais `04_Maquette_3D` pour ouvrir et retrouver le modèle.

Les noms internes des projets KiCad, les bibliothèques locales, les sauvegardes
et le coffre Obsidian ont été conservés ensemble. Le journal des anciens et
nouveaux chemins est dans `06_Archives_et_exports/Rangement/deplacements.json`.

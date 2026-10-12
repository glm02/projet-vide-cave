# Maquette du vide-cave GEII

Ouvrir `Vide_cave_GEII.blend` dans Blender. Le fichier contient quatre scènes,
sélectionnables dans la liste « Scene » en haut de l'interface :

1. **01 Installation du vide-cave** : installation dans une cave en briques, avec
   un bassin fermé sur quatre côtés sous le niveau du sol, une grille amovible,
   conduite de refoulement et coffret de surveillance fixé au mur.
2. **02 Coffret en vue éclatée** : coque creuse, face avant séparée, écran LCD,
   voyants, boutons, grille du buzzer et volumes des composants électroniques.
3. **03 Implantation KiCad fournie** : positions des empreintes et pastilles
   extraites du PCB fourni, contour de la carte et pistes de la face arrière.
   Les corps des composants sont des volumes approchés.
4. **04 Intérieur moteur et pompe** : vue en coupe dédiée, avec stator et bobinages,
   rotor à cage, arbre, roulements, garniture mécanique et roue centrifuge à six
   aubes. Les pièces et légendes sont modifiables. Le flotteur et son câble ont
   été retirés de toutes les scènes à la demande de l'utilisateur.

La coupe intérieure est pédagogique et générique : aucun plan interne du modèle
réel n'est fourni. La disposition et la forme des pièces ne constituent pas une
restitution fabricant. L'intérieur est également visible dans la scène d'installation.

Les objets sont séparés, nommés en français et regroupés dans des collections.
Les matériaux, textes, caméras et éclairages restent modifiables. Les unités sont
en mètres. Pavé numérique 0 : vue caméra ; bouton central : tourner autour du modèle.
F12 : refaire le rendu de la scène sélectionnée.

## Documents consultés

- `01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx` : fonctions de surveillance, auto-test tous
  les trois jours pendant une minute maximum, LCD, LED, buzzer, pompe 230 V.
- `01_Documents/Presentation/projet_vide_cave_geii.pptx` et les trois synoptiques `.drawio` de `02_Synoptiques` : chaîne de
  mesure, décision, protection et affichage.
- `03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_sch` : PIC16F1788, ACS756,
  FODM3011, triac et LCD 16 × 2. Ces choix plus précis ont été retenus pour la
  représentation de l'électronique. L'ESP32/Wi-Fi figure comme une évolution
  dans les documents de présentation et n'est pas ajouté comme un composant
  déjà installé.
- `03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_pcb` : contour rectangulaire de
  90 × 80 mm, positions des composants et pastilles, pistes arrière.
- Documents de consignes PDF : attentes de présentation, conception et
  fabrication. Ils ne donnent aucune cote mécanique du vide-cave réel.

## Dimensions proposées et limites

Les dimensions mécaniques ne sont pas indiquées dans les documents. La scène
d'installation est donc une **maquette de présentation**, avec une pompe générique
de diamètre maximal 190 mm et hauteur approximative 330 mm, une cuve de
780 × 690 mm et un coffret de **190 × 245 × 75 mm** (largeur × hauteur × profondeur).
La hauteur et la conduite sont proposées pour rendre l'installation lisible.
Le bassin possède quatre parois et une grille amovible au-dessus. Il est placé
sous le niveau du sol d'une cave proposée de 2,50 × 2,20 m au sol et environ
2,40 m de hauteur, avec murs en briques et plafond. La caméra est dans la cave.
La vue intérieure détaillée du moteur et de la pompe reste dans la scène 04.

La face avant, ses boutons, la couleur des voyants, les presse-étoupes, le support,
la forme de la pompe et l'implantation intérieure du coffret sont des propositions.
Le module d'alimentation basse tension et le dissipateur sont des volumes indicatifs.
Les positions et les dimensions exactes des pièces doivent être adaptées aux
références réellement utilisées. Les textes du LCD illustrent un état de fonctionnement,
sans afficher une fausse valeur mesurée de courant.

La scène KiCad conserve les origines fournies. **U3 (capteur de courant) et TRIAC1
(optotriac) sont placés hors du contour de la carte dans le fichier source.** Ce
point doit être clarifié avant un modèle mécanique destiné à la fabrication.
La scène KiCad n'est pas l'implantation intérieure proposée dans la scène coffret.
La forme des boîtiers n'est pas une importation des modèles 3D fabricants.
Les pistes sont visibles sous la carte et les pastilles au-dessus ; aucun schéma
de raccordement électrique complet n'est déduit de cette visualisation.

Cette maquette ne valide pas l'indice IP, l'isolation électrique, la dissipation,
la compatibilité mécanique ni l'aptitude à l'impression 3D du coffret.

## Recréer ou adapter le modèle

`lire_kicad.py` extrait les données sources en lecture seule vers
`implantation_kicad.json`. `creer_maquette.py` génère les quatre scènes, les rendus
et le fichier Blender. Les dimensions du coffret sont définies dans `housing()`.
Pour régénérer le fichier, exécuter dans un terminal :

```powershell
& 'C:\Program Files\Blender Foundation\Blender 5.2\blender.exe' --background --python '.\04_Maquette_3D\creer_maquette.py'
```

Le générateur remplace les fichiers générés dans `04_Maquette_3D`. Conserver une
copie séparée du `.blend` avant d'y faire des modifications manuelles.

# Carte électronique du vide-cave - révision A

Projet KiCad indépendant de la maquette Blender et de la carte d'origine.
Ouvrir **Vide_cave_5A.kicad_pro** avec KiCad 8 ou une version compatible. Le nom
5A désigne l'hypothèse conservatrice de dimensionnement des pistes, pas le
courant mesuré de la pompe. Dernière indication : pompe **230 V, environ 300 W**.

## Fichiers livrés

- Schéma natif `.kicad_sch` et carte routée `.kicad_pcb`.
- Bibliothèque de symboles autonome, empreintes locales du capteur et de l'optotriac.
- `exports/Schema.pdf` : schéma électrique avec les connexions par étiquettes.
- `exports/Implantation.pdf` : vue de dessus de la carte.
- `exports/typons/` : deux typons PDF et SVG à l'échelle 1:1.
- `exports/gerbers/` et `exports/Gerbers_prototype.zip` : cuivre, masques,
  sérigraphie et contour ; perçages PTH/NPTH séparés, trous ovales routés.
- `Nomenclature.csv`, `connexions.json`, rapports ERC/DRC et scripts de génération.

La carte fait **190 x 130 mm**, sur deux faces, FR4 1,6 mm, cuivre 35 µm comme
hypothèse de calcul. Les pistes principales de phase et de neutre font 4 mm.
Les sorties du TO-220 comportent des étranglements courts de 1,1 mm, à vérifier
thermiquement. Pistes logiques 0,35 mm ; vias 0,8 mm, perçage 0,4 mm. Une fente
de 0,8 x 9 mm passe sous l'optotriac. Le routage maintient au moins **8 mm de
distance entre les cuivres de logique et les cuivres secteur** selon la règle DRC.
Cela ne constitue pas à lui seul une qualification d'isolement du produit.

## Composants et fonctionnement

**U1 : PIC16F1788-I/SP**, DIP28, alimenté en 5 V. Les deux VSS sont reliés à GND.
Le circuit prévoit le découplage, le rappel MCLR, un connecteur PICkit, un LCD
16x2 à interface parallèle 4 bits, deux boutons externes et une liaison UART TTL.
Utiliser l'oscillateur interne pour libérer RA6, et configurer les broches LCD en
numérique. Le programme du PIC reste à créer ou à adapter ; aucun automatisme
de niveau ou de protection contre la marche à sec n'est fourni dans ce dossier.

**U2 : ACS756SCB-100B-PFF-T**, repris des références de ton projet. Brochage :
1=VCC, 2=GND, 3=VIOUT, 4=IP+, 5=IP-. Le courant de la pompe passe dans le
conducteur isolé du capteur, jamais dans GND. L'offset nominal est VCC/2 ; la
sensibilité nominale du modèle 100B est 20 mV/A. L'entrée AN0 reçoit la sortie
via 1 kΩ et 10 nF (fréquence de coupure environ 15,9 kHz). Pour le courant
alternatif, le logiciel doit retirer l'offset et calculer une valeur efficace
sur des périodes entières de 50 Hz. À quelques ampères, ce capteur 100 A donne
un faible signal : acquisition, filtrage et étalonnage réels restent nécessaires.

L'ACS756 est signalé par Allegro comme déconseillé pour les nouvelles conceptions.
Il est conservé pour reprendre le matériel existant. Pour un achat neuf, revoir
le choix du capteur et son empreinte avec un modèle actuel adapté à quelques
ampères ; ne pas remplacer automatiquement U2 sur cette carte.

**U3 : MOC3083TVM**, optotriac 800 V à passage par zéro, IFT maximal 5 mA.
Commander exactement la variante **TVM à entre-rangées 10,16 mm**. Le MOC3083M
ordinaire à 7,62 mm ne correspond pas à l'empreinte livrée. Les broches 3 et 5
restent non connectées ; la broche 5 ne doit pas être raccordée. Q2 commande
sa LED par R5=330 Ω. À 5 V, avec VF=1,5 V et VCEsat=0,2 V, IF≈10 mA ; même
à 4,75 V et R5 à +5 %, IF reste proche de 8,8 mA. Le rappel de base de Q2
force l'arrêt lorsque la sortie PIC est à haute impédance.

**Q1 : BTA16-800CRG**, triac **standard à quatre quadrants**, 800 V / 16 A,
IGT maximal 25 mA en QI et QIII, TO-220 à languette isolée. C'est la référence
finalement retenue avec le MOC3083TVM. Elle remplace la proposition initiale SW.
R6 et R7 valent chacune 91 Ω (182 Ω au total), et R8=330 Ω relie la gâchette
à A1. Cela donne une marge de déclenchement pour le triac standard : avec une
tension de 12 V, 3 V dans l'opto et 1,3 V à la gâchette, le courant disponible
est d'environ (12-3-1,3)/182 - 1,3/330 = **38 mA**, avant tolérances. Il s'agit
d'un pré-dimensionnement ; vérifier les déclenchements et la commutation avec
la pompe réelle, à froid et à chaud.

La phase suit : **J1-L → F1 → ACS756 IP+ / IP- → triac A2 / A1 → J2-L**.
Le neutre J1-N rejoint directement J2-N. L'optotriac transmet la commande et
ne porte pas le courant du moteur. La commande est tout ou rien, sans variation
de vitesse. RB2=1 demande la marche ; RB2=0 demande l'arrêt. Le triac s'éteint
au passage du courant par zéro. Son arrêt ne réalise pas un sectionnement.

R9=39 Ω / 1 W et C5=10 nF **classe X2, 305 VAC** constituent le réseau RC entre
A1 et A2. Utiliser une résistance adaptée aux impulsions, peu inductive et
incombustible. L'empreinte C5 prévoit un pas de 15 mm et un corps maximal de
19 x 5 mm : sélectionner une référence X2 dont le dessin mécanique correspond.
Une capacité MKS ordinaire ne convient pas, malgré le nom générique de l'empreinte.
Ces valeurs sont des points de départ issus des circuits d'application onsemi.
Leur adéquation dépend du moteur ; vérifier le facteur de puissance, le dv/dt,
les redéclenchements et les surtensions. Aucune validation CEM ou de tenue aux
transitoires secteur n'a été effectuée.

**F1 : Littelfuse 02153.15MXP**, candidat temporisé céramique HBC 5x20,
3,15 A / 250 VAC. Le calibre est provisoire pour la pompe ~300 W ; il ne doit
pas être décidé à partir de 300/230 uniquement. Confirmer le courant de plaque,
le courant et la durée du démarrage, le câble et la courbe temps-courant. Les
5 A annoncés plus tôt ont servi de marge pour les pistes. S'ils représentent
réellement un courant continu de plaque, revoir F1 et l'ensemble thermique.
Le fusible ne remplace pas la protection thermique du moteur.

**J1/J2 : RND 205-00298**, pas 10 mm, deux pôles, constructeur 10 A / 300 V.
L'empreinte locale prévoit des trous de 1,4 mm selon la recommandation constructeur.
Faire néanmoins un essai d'ajustement avec les composants réels avant fabrication.

## Connexions extérieures

| Connecteur | Broches |
|---|---|
| J1 secteur | 1 = phase L ; 2 = neutre N |
| J2 pompe | 1 = phase commutée L ; 2 = neutre N |
| J3 alimentation | 1 = +5 V ; 2 = GND, alimentation externe isolée |
| J4 PICkit | 1 = VPP/MCLR ; 2 = VDD ; 3 = GND ; 4 = ICSPDAT ; 5 = ICSPCLK ; 6 non utilisée |
| J5 LCD | 1 GND ; 2 +5V ; 3 VO ; 4 RS ; 5 GND/RW ; 6 E ; 7-10 libres ; 11 D4 ; 12 D5 ; 13 D6 ; 14 D7 ; 15 anode via 220 Ω ; 16 GND |
| J6 boutons | 1 GND ; 2 START/RB1 ; 3 STOP/RB3 ; 4 +5V |
| J7 UART | 1 GND ; 2 TX/RC6 ; 3 RX/RC7 ; 4 +5V |
| J8 hors carte | Terre secteur, terre pompe et coffret conducteur si applicable, sur un bornier PE externe |

**La terre de protection ne passe pas sur cette carte.** Réaliser la continuité
PE secteur-pompe par une liaison externe adaptée au courant de défaut et aux
exigences de l'installation. J8 est représenté uniquement sur le schéma ; il
n'est pas implanté sur le PCB. Ne jamais confondre PE, N et GND.

Les boutons se ferment vers GND ; activer les rappels internes du PIC ou prévoir
des rappels externes si le câblage de l'IHM le nécessite. J7 est une interface
TTL 5 V, pas une interface RS-232 ni une prise USB. Pendant l'ICSP, choisir une
seule source d'alimentation 5 V (PICkit ou J3) ; la carte ne possède pas de
sélection automatique entre les deux sources.

## Dissipation et intégration

Le calibre 16 A du triac n'est pas le courant certifié de la carte. Avec le
modèle thermique ST VTO≈0,85 V et Rd≈25 mΩ, une sinusoïde de 5 A donne une
estimation de pertes de 0,85 x 0,900 x 5 + 0,025 x 5² ≈ **4,45 W**.
Prévoir un dissipateur, cible provisoire **≤8 °C/W**, un bon contact thermique
et une fixation appropriée. Contrôler la température de jonction estimée avec
la température ambiante réelle, le boîtier et le courant effectif. L'intégration
mécanique du dissipateur n'est pas qualifiée dans ce projet ; il faut aussi
vérifier ses distances aux bornes, broches et pistes secteur et aux connecteurs
accessibles. Aucun dissipateur précis n'est commandé ou modélisé.

La carte doit être montée dans un coffret adapté, hors de l'eau et de la zone
humide du bassin, avec retenue des câbles. Le projet ne certifie ni l'installation
230 V ni son enveloppe. Revue par l'encadrant compétent avant mise sous tension.

## Impression et fabrication

Les PDF sont des vecteurs à l'échelle 1:1. Imprimer en **taille réelle / 100 %**,
sans ajustement automatique ; mesurer la barre de contrôle de 20 mm. La face
F.Cu est en miroir. La face B.Cu utilise les coordonnées vues depuis le dessus.
Les deux fichiers sont prévus pour poser la face imprimée contre le cuivre
lors d'une insolation par contact ; confirmer le sens avec le procédé de
l'atelier, notamment pour un transfert toner. Faire un essai papier sur les
composants réels et vérifier l'alignement des deux faces.

Cette carte nécessite des trous et vias métallisés, dont les trous ovales du
capteur, et une fente fraisée. Les typons seuls ne définissent pas ces opérations.
Pour un prototype industriel, utiliser les Gerbers et les fichiers de perçage
ensemble ; faire relire les empreintes personnalisées et les tolérances par le
fabricant. Pour une réalisation d'atelier, définir avec l'encadrant comment les
connexions entre faces seront réalisées. L'empreinte ACS756 est dérivée du
dessin de référence Allegro et doit faire l'objet d'un essai mécanique réel.

## Validation fournie et travail restant

Les contrôles KiCad 8.0.9 sur la version livrée donnent : **ERC 0, DRC 0,
connexions manquantes 0, écarts schéma-carte 0**, sans exclure de violations.
Les rapports sont dans `exports/controle/`. Les PDF ont été rendus et contrôlés
visuellement. Ce sont des vérifications de cohérence CAO, pas des essais du
montage, de la pompe, de l'isolement, de la thermique ou de la CEM. La carte reste
un **prototype à faire valider avant fabrication et branchement au secteur**.

Pour regénérer : adapter le chemin KiCad dans `construire_carte.py`, exécuter
ce script avec le Python de KiCad, puis `router_carte.py`. Le premier script
écrase le schéma et le PCB générés ; conserver toute modification manuelle avant
de le relancer. `exporter_dossier.py` nécessite reportlab, pypdf et Poppler.

## Sources techniques

- [Microchip PIC16F1788](https://www.microchip.com/en-us/product/PIC16F1788).
- [Allegro ACS756, dessin PFF et caractéristiques](https://www.allegromicro.com/-/media/files/datasheets/acs756-datasheet.pdf), copie dans `sources/ACS756.pdf`.
- [Statut de l'ACS756](https://www.allegromicro.com/en/products/sense/current-sensor-ics/integrated-current-sensors/acs756).
- [onsemi MOC3083M, variantes TVM et circuit d'application](https://www.onsemi.com/download/data-sheet/pdf/moc3083m-d.pdf).
- [ST, famille BTA16 et T16](https://www.st.com/resource/en/datasheet/t1610.pdf).
- [Littelfuse série 215](https://www.littelfuse.com/assetdocs/littelfuse_fuse_215_datasheet.pdf?assetguid=990f7193-d9a2-48e4-b760-2b43514249cf).
- [RND, bornier série 205-00298](https://media.distrelec.com/Web/Downloads/_t/ds/rnd_terminal_blocks_00298_eng_tds.pdf).

La bibliothèque du PIC provient du schéma scolaire fourni ; les autres symboles
et les empreintes standard proviennent des bibliothèques KiCad installées.

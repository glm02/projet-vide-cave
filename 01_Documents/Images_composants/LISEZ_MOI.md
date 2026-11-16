# Images des composants pour le rapport de conception

Pour chaque composant : une image de la **datasheet du fabricant** (boîtier et brochage) et un **extrait de notre schéma KiCad** (comment on l'a câblé). Mettre les deux côte à côte dans le rapport, à la question 16, avec la légende proposée.

| Composant | Image datasheet | Extrait de notre schéma | Légende proposée |
|---|---|---|---|
| PIC16F1788 | `01_PIC16F1788_brochage_datasheet.png` | `01_PIC16F1788_schema_kicad.png` | Brochage du PIC16F1788 en boîtier DIP-28 (datasheet Microchip) et son câblage sur notre carte |
| Capteur ACS756 | `02_ACS756_boitier_datasheet.png` (photo du boîtier) et `02_ACS756_brochage_datasheet.png` | `02_ACS756_schema_kicad.png` | Capteur de courant Allegro ACS756 : boîtier, brochage, et son câblage vers l'entrée AN0 du PIC |
| (option) ACS756 | `02_ACS756_schema_application_datasheet.png` ou `02_ACS756_schema_interne_datasheet.png` | — | Montage d'application recommandé par Allegro / schéma interne du capteur à effet Hall |
| Triac BTA12 | `03_BTA12_boitier_brochage_datasheet.png` | `03_BTA12_schema_kicad.png` | Triac BTA12 en boîtier TO-220 isolé (datasheet ST) et la partie puissance : triac, optotriac et moteur |
| Optotriac MOC3023 | `04_MOC3023_boitier_brochage_datasheet.png` | `04_MOC3023_schema_kicad.png` | Optotriac MOC3023 : schéma interne et boîtier DIP-6, et sa commande par la sortie RB2 du PIC |
| Optocoupleur 6N136 | — (datasheet non récupérée) | `05_6N136_schema_kicad.png` | Optocoupleur 6N136 utilisé pour détecter le passage par zéro de la tension |
| Afficheur LCD 16×2 | `06_LCD_dessin_brochage_datasheet.png` (à pivoter de 90°) | `06_LCD_schema_kicad.png` | Afficheur LCD 16×2 (Displaytech 162B) : dessin mécanique, brochage et câblage en mode 4 bits |
| Schéma complet | — | `00_schema_complet_kicad.png` | Schéma électrique complet de la carte (KiCad) |

## Attention

- Dans `04_MOC3023_schema_kicad.png`, le symbole s'appelle encore « TRIAC1 octo-triac » : c'est bien le MOC3023, mais le nom du symbole est à renommer dans KiCad.
- Le schéma complet (version du 22/10) a encore U2 « MOC3023M » à la place de la détection du zéro : sur la version du 2/11, c'est le 6N136. Exporter le schéma final avant de mettre le schéma complet dans le rapport.
- Les photos réelles de vos composants valent mieux que les dessins des datasheets pour la partie « réalisation » : photographier la carte, le triac, le capteur et le LCD.

## Sources (à citer dans le rapport)

- Microchip, *PIC16(L)F1788/9 Data Sheet*, DS40001675C, p. 4.
- Allegro MicroSystems, *ACS756xCB Datasheet*, rev. 11, p. 2 et 4.
- STMicroelectronics, *BTA12, BTB12, T12xx Datasheet*, rev. 9, p. 1.
- Fairchild / onsemi, *MOC3010M … MOC3023M Datasheet*, rev. 1.5, p. 1.
- Displaytech, *LCD Module 162B Series Product Specification*, v2.1, p. 7.

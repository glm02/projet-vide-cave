# Règles PCB du projet G206

## Consignes de l'équipe

- Fabrication essentiellement simple face.
- **B.Cu, bleu : face de routage courante.**
- **F.Cu, rouge : quelques straps de fil seulement.** Réduire leur nombre ; ne pas transformer la carte en routage double face généralisé.
- Conserver les composants du schéma. Les composants supprimés de l'ancienne carte ne sont pas à réintroduire ; leurs anciennes empreintes et branches inutiles doivent être retirées du PCB.
- Ne pas déplacer, remplacer ou modifier les composants conservés sans nouvelle demande.
- Les composants de puissance et les nouveaux circuits se dimensionnent après clarification des modèles et caractéristiques du moteur.

La limitation bleu/straps est une consigne de fabrication. Elle n'est pas enregistrée comme une interdiction automatique de routage F.Cu : aucun fichier `.kicad_dru` n'est présent. Deux couches de cuivre activées dans KiCad ne signifient donc pas ici une intention de fabriquer deux faces cuivrées.

## Paramètres enregistrés dans KiCad

Toutes les valeurs ci-dessous sont en mm, extraites du `.kicad_pro` et du PCB, sans modification.

| Paramètre | Valeur |
|---|---:|
| Largeur minimale d'une piste | 0,6 |
| Classe Default : largeur proposée | 0,6 |
| Classe Power : largeur proposée | 0,7 |
| Nets de la classe Power | +5V et GND |
| Isolation minimale entre cuivres | 0,5 |
| Cuivre au bord du PCB | 1,0 |
| Largeur minimale de connexion cuivre | 0,6 |
| Diamètre minimal de via | 2,0 |
| Via proposé : diamètre / trou | 2,0 / 0,8 |
| Anneau minimal | 0,4 |
| Trou traversant minimal | 0,7 |
| Isolation trou/cuivre | 0,4 |
| Distance entre trous | 1,0 |
| Hauteur minimale du texte | 0,8 |
| Épaisseur minimale du texte | 0,08 |
| Taille de pastille par défaut | 3 × 3 |
| Perçage de pastille par défaut | 1,4 |

Les pistes effectivement conservées utilisent 0,6 et 0,7 mm. Il n'existe pas encore de classe distincte pour la puissance secteur. Ces dimensions de pistes ne prouvent pas une capacité à transporter le courant moteur ; celle-ci dépend de l'épaisseur de cuivre, du courant, de l'échauffement acceptable et de la géométrie.

## Plan de masse et gouttes de cuivre

Un véritable plan de masse GND est défini sur B.Cu. Les autres zones sont principalement des gouttes de cuivre aux jonctions ; elles ne constituent pas autant de plans de masse.

| Paramètre du plan GND | Valeur |
|---|---:|
| Isolation de la zone aux autres nets | 1,0 |
| Épaisseur minimale de cuivre | 0,6 |
| Espacement des freins thermiques | 0,61 |
| Largeur des branches thermiques | 0,61 |
| Seuil d'aire d'îlot enregistré | 10 mm² |
| Priorité | 0 |

Le plan a été recalculé après nettoyage. Des îlots isolés demeurent signalés par le DRC : examiner leur utilité et la continuité de GND lors de la reprise du routage.

## Straps après nettoyage

| Net | Départ X,Y | Arrivée X,Y | Longueur | Largeur |
|---|---|---|---:|---:|
| GND | 113,5 ; 135 | 127,5 ; 135 | 14 | 0,7 |
| MCLR | 173 ; 110 | 183 ; 110 | 10 | 0,6 |

Le strap CODEUR de 47,5 mm faisait partie des anciennes liaisons sans destination et a été supprimé. Les deux straps conservés utilisent au total quatre vias Ø2 mm/perçage 0,8 mm. En fabrication non métallisée, assurer la continuité avec le fil et les soudures appropriées ; le nom « via » dans KiCad n'effectue pas la métallisation physique.

## Contrôles et exclusions

- Les violations d'isolation, courts-circuits, contour, pistes non connectées et largeur de piste sont réglées en erreur.
- Les exclusions DRC et ERC du projet sont vides.
- L'absence de courtyard et certaines vérifications de type de montage sont ignorées dans les paramètres enregistrés.
- Refaire le remplissage des zones et le DRC après tout changement de placement ou de routage.
- Régénérer les anciens Gerbers avant fabrication ; ils ne décrivent pas le PCB nettoyé.
- La zone secteur doit recevoir des règles de séparation appropriées lors de sa conception. Le minimum général de 0,5 mm ne valide pas l'isolement secteur/logique.

Voir [l'analyse complète](RAPPORT_ANALYSE.md) et [les composants du schéma](../../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv).

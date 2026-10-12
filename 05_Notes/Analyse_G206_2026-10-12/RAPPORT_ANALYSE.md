# Analyse et nettoyage de la carte vide-cave G206

Date : 12 octobre 2026. Projet : `03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_pro`.

**Conclusion : le PCB a été nettoyé conformément au schéma conservé, mais la carte reste en cours de conception. Elle ne permet pas encore de mesurer le courant du moteur, de commander correctement l'optotriac ou de mesurer le déphasage. Elle n'est pas prête à fabriquer ni à raccorder au secteur.**

La suppression des anciens composants du schéma était volontaire : vous adaptez une carte de commande de robot. Les empreintes restantes de ces anciens composants ont été retirées du PCB. Elles ne sont donc pas des fonctions à réintroduire automatiquement.

## Ce qui a été modifié et préservé

- 21 anciennes empreintes absentes du schéma retirées du PCB : C3, C4, D1, D2, D3, J3, R1, R2, R5, R6, R8, RV1, SW1, SW2, SW3, TP1 à TP6.
- 98 segments de pistes, 2 vias et 42 zones de gouttes de cuivre associés au cuivre devenu sans destination retirés.
- Plan de masse recalculé après nettoyage.
- **Les blocs des 21 empreintes gardées sont identiques octet par octet à l'état initial.** Positions, angles, valeurs, pastilles, numérotation et modèles n'ont pas été remplacés.
- **Schéma et paramètres `.kicad_pro` conservés sans modification.** Les erreurs relevées dans ces fichiers sont décrites ci-dessous, sans correction automatique.
- Sauvegarde avant intervention : `G206_CAMEL_TP_CAO-backups/G206_CAMEL_TP_CAO-2026-10-12_avant_nettoyage_Codex.zip`.
- Les trois éléments absents du PCB mais présents au schéma sont BZ1, M1 et Triac_de_puissance1. M1 peut rester un moteur externe ; son raccordement doit alors être représenté par des borniers. Aucun boîtier ou modèle non défini n'a été inventé pour les deux autres.

## Erreurs à traiter en priorité

| Priorité | Constat | Conséquence et correction nécessaire | Base du constat |
|---|---|---|---|
| Bloquant | U3.3 est sur `AN0`, U1.2 sur `capteur de courant` | Deux étiquettes différentes : le signal de courant ne rejoint pas l'entrée AN0/RA0 du PIC. Unifier le net ou câbler la liaison. | Netlist native, fichiers et ERC |
| Bloquant | U1.23 est sur `commande octo coupleur `, R7.1 sur `RB2` | La sortie du PIC ne rejoint pas la résistance de la LED de TRIAC1. Même numéro de port dans une étiquette descriptive ne crée pas de connexion. | Netlist native, fichiers et ERC |
| Bloquant | U3.4 IP+ et U3.5 IP− sont laissés ouverts | Aucun courant moteur ne traverse le capteur : il ne mesure pas la pompe. Mettre son conducteur primaire dans la chaîne de puissance. | Schéma et ERC ; fiche ACS756 |
| Bloquant | Pas de voie tension ni de référence de passage par zéro | Le déphasage courant/tension demandé dans le rapport n'est pas mesurable avec cette carte telle qu'elle est dessinée. Prévoir une référence de phase isolée. | Inventaire complet et netlist |
| Bloquant | Symbole d'alimentation `AC` à une seule broche, sans entrée phase/neutre | Ce symbole nomme un net ; il ne constitue pas une source alternative à deux bornes. Le raccordement du secteur et du moteur est incomplet. | Schéma exporté et fichier brut |
| Bloquant | R3, R4 et R7 gardent des données de l'ancienne carte sur le PCB | Six pastilles ont des nets différents du schéma. Les valeurs du PCB sont encore 10 kΩ, 10 kΩ et 270 Ω, alors que le schéma dit seulement `R`. Définir les résistances et leurs empreintes avant synchronisation/routage. | Comparaison exhaustive des composants |
| Bloquant | U3 et TRIAC1 sont hors du contour du PCB | Ces composants ne seront pas inclus dans la carte découpée à 90 × 80 mm. Leur implantation est à réaliser ; elle a été conservée à votre demande. | Coordonnées et vue native |
| Bloquant si FODM3011 retenu | TRIAC1 utilise le symbole FODM3011, prévu pour 250 V en blocage | 230 V efficaces donnent environ 325 V crête, avant surtensions. Le composant nominal du symbole est inadapté ; préciser un véritable modèle compatible secteur et son empreinte. | Documentation onsemi et calcul |
| À corriger | BZ1 est polarisé, avec son `+` à GND et son `−` relié à R9/PIC | Polarité incohérente pour un buzzer polarisé alimenté depuis une sortie positive. Choisir le buzzer réel et vérifier son courant ; un pilote peut être nécessaire. | Schéma et netlist ; modèle réel inconnu |
| À corriger | U3 n'a pas de condensateur de découplage local | Ajouter le découplage recommandé à VCC/GND du capteur avant implantation finale. | Fiche Allegro, application typique |
| À améliorer | C2 est à environ 33,49 mm à vol d'oiseau de U1.20 VDD | Le condensateur de 100 nF existe, mais son implantation ne donne pas une boucle locale courte au PIC. À rapprocher dans une prochaine correction autorisée. | Coordonnées de pastilles et calcul |

L'espace final dans `commande octo coupleur ` est réellement enregistré. Utiliser des noms de nets courts et identiques aux deux extrémités évite ce type de rupture.

## Compréhension du système

Le projet doit surveiller une pompe de cave, détecter un blocage ou une marche à vide, arrêter le moteur et informer l'utilisateur. La chaîne prévue est : courant moteur → capteur à effet Hall → ADC du PIC → décision → commande optique → triac de puissance → moteur. Une seconde référence issue de la tension secteur est nécessaire pour calculer le déphasage.

L'interface locale comprend un LCD 16 × 2, une LED et un buzzer. Le connecteur PICkit sert à programmer le PIC, la liaison UART TTL à échanger des informations ou effectuer le debug. Le bouton SW4 est câblé comme reset MCLR, pas comme un bouton indépendant d'acquittement de défaut.

Dans l'état actuel, cette chaîne est dessinée partiellement : les deux liaisons logiques citées plus haut sont ouvertes, le primaire du capteur n'est pas raccordé et l'étage secteur est incomplet. Le logiciel ne peut pas compenser ces ruptures matérielles.

### Alimentation

J1 fournit directement le +5 V et GND. C1 = 47 µF constitue la capacité de réserve ; C2 et C5 = 100 nF découplent respectivement la logique et le LCD. Aucun convertisseur secteur/5 V ou régulateur n'est présent sur cette carte : la source 5 V externe et son isolation doivent être définies.

L'alimentation nominale du PIC16F1788 est cohérente avec 5 V. Ses VSS 8 et 19 sont reliés à GND, VDD 20 au +5 V. R12 = 10 kΩ rappelle MCLR au +5 V ; SW4 le ramène à la masse. Cette partie est cohérente avec le brochage nominal fabricant.

### Mesure du courant

Le symbole U3 désigne un ACS756xCB-100B-PFF : 1 = VCC, 2 = GND, 3 = VIOUT, 4 = IP+, 5 = IP−. Ce brochage est cohérent avec la fiche Allegro. La référence fabricant exacte n'est toutefois pas renseignée dans les propriétés de commande.

Pour cette variante nominale, la sensibilité est de 20 mV/A et la sortie est centrée autour de VCC/2. À 5 V, zéro courant correspond donc à environ 2,5 V, et non à 0 V. Le programme devra soustraire l'offset calibré avant de calculer la valeur efficace. Le choix d'une plage ±100 A doit être justifié par le courant réel de la pompe et la précision recherchée ; les seuils ne sont pas encore définis.

### Commande et puissance

TRIAC1 est un optotriac de commande : sa LED appartient à la logique, sa sortie déclenche la gâchette du triac de puissance. Il ne doit pas porter directement le courant de la pompe. R7 limite le courant de sa LED ; R3 appartient à la commande de gâchette. R4 est actuellement en série dans la branche moteur, sans valeur définie : son rôle et son dimensionnement doivent être explicités.

Le triac de puissance est encore un symbole générique, sans référence, empreinte, caractéristiques de déclenchement ni calcul thermique. Aucun fusible, réseau antiparasite dimensionné ou raccordement secteur complet n'est dessiné dans le projet G206. Le choix de ces éléments dépend du moteur réel, de son démarrage et de sa charge inductive.

La règle d'isolation de 0,5 mm du projet est une règle générale de CAO basse tension ; elle ne qualifie pas une séparation secteur/logique. Une séparation spécifique et des distances de sécurité adaptées doivent être définies lors de la conception de la puissance. Ne pas mélanger la masse logique au circuit secteur.

### LCD et communication

DS1 utilise un symbole HY1602E modifié pour l'afficheur Displaytech 162B. Sa numérotation particulière est bien celle du dessin fabricant 162B avec rétroéclairage : 1 LEDA, 2 LEDK, 3 VSS, 4 VDD, 5 VO, 6 RS, 7 R/W, 8 E, 9 à 16 DB0 à DB7. **Ne pas remplacer automatiquement ce symbole par un LCD générique à numérotation différente.**

DB0 à DB3 sont volontairement non connectés : il s'agit d'une interface 4 bits. R11 = 56 kΩ et R13 = 1,5 kΩ donnent VO ≈ 0,1304 V à vide ; ce calcul a également été retrouvé par une simulation LTspice isolée. Cela ne prouve pas le contraste réel à toutes températures. Avec un rétroéclairage nominal à 3,2 V et R14 = 270 Ω, le courant théorique est d'environ 6,7 mA : inférieur au courant typique de la variante basse consommation, donc luminosité à vérifier, sans conclure à une erreur bloquante.

| Connecteur | Broche | Signal |
|---|---|---|
| J4 PICkit | 1 | MCLR/VPP |
| J4 PICkit | 2 | +5 V |
| J4 PICkit | 3 | GND |
| J4 PICkit | 4 | ICSPDAT, U1.28 |
| J4 PICkit | 5 | ICSPCLK, U1.27 |
| J4 PICkit | 6 | PGM/LVP non connecté |
| J2 série TTL | 1 | GND |
| J2 série TTL | 4 | RX du PIC, à relier au TX de l'adaptateur |
| J2 série TTL | 5 | TX du PIC, à relier au RX de l'adaptateur |
| J2 série TTL | 2, 3, 6 | Non connectées |

RA6 et RA7 sont utilisées par le LCD : prévoir l'oscillateur interne, la libération de ces broches et VCAPEN désactivé sur RA6. Les entrées multiplexées du LCD doivent être configurées en numérique. Aucun firmware de la carte G206 n'a été trouvé dans le dossier étudié : ces configurations restent à contrôler dans le futur programme.

## Comparaison avec les documents

Référence principale : [rapport de pré-étude](../../01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx). Les synoptiques et les consignes de conception/fabrication ont également été lus.

| Fonction ou choix du rapport | État du projet G206 | Suite à donner |
|---|---|---|
| Mesure du courant moteur | Capteur dessiné, primaire et sortie non reliés correctement | Compléter la chaîne de mesure |
| Déphasage courant/tension | Aucune référence tension ou passage par zéro | Ajouter la voie isolée, ou réviser explicitement le périmètre |
| ESP32 pressenti | PIC16F1788 réellement utilisé | Actualiser le choix dans le rapport ; un PIC n'est pas une erreur en soi |
| Relais/contacteur de coupure | Optotriac + triac générique | Justifier la solution retenue et son comportement à l'arrêt |
| LCD, LED, buzzer | LCD et LED présents ; buzzer incomplet et polarisé à l'envers | Corriger/dimensionner le buzzer |
| Auto-test tous les trois jours, maximum une minute | Fonction logicielle, sans code livré | Prévoir temporisation, états et coupure en cas de défaut |
| Acquittement utilisateur | SW4 est un reset du PIC | Distinguer acquittement et reset, définir le comportement souhaité |
| Flotteur / présence d'eau | Pas d'entrée dédiée identifiable au schéma | Clarifier raccordement et responsabilité dans le système |
| Wi-Fi / serveur distant | UART TTL disponible, pas de module Wi-Fi | Fonction distante non réalisée ; conserver comme évolution si prévu |
| Seuils, précision, courant nominal et démarrage | Valeurs `[x]`, Imin/Imax et données de plaque non fixées | Mesurer la pompe avant dimensionnement final |

Le synoptique général montre ESP32 et relais. Les versions 2 et 3 se concentrent sur la comparaison de courant ; elles n'expliquent pas la référence de phase. Le rapport décrit le déphasage, mais les dessins et le matériel ne couvrent pas encore cette fonction. Le cartouche du schéma porte encore le titre de la carte robot d'origine : à actualiser dans une future modification du schéma.

Le projet voisin `Carte_vide_cave_PIC_Allegro_TRIAC` est une autre proposition, de 190 × 130 mm sur deux faces. Ses composants et son routage n'ont pas été substitués à votre G206. Il ne respecte pas automatiquement votre contrainte de routage simple face.

## PCB et règles de fabrication

Voir [les règles détaillées](REGLES_PCB.md). Le contour est un rectangle de 90 × 80 mm, de x = 100 à 190 mm et y = 70 à 150 mm. Le nettoyage conserve le contour et l'implantation des composants gardés.

Le routage courant est sur **B.Cu, bleu**. F.Cu, rouge, est réservé à quelques straps de fil : un Gerber rouge n'est pas à considérer comme un cuivre supérieur généralisé. Les vias des straps doivent avoir une liaison physique réelle si la fabrication de l'atelier n'est pas métallisée.

Après nettoyage, deux straps restent : GND de 14 mm et MCLR de 10 mm. Le strap CODEUR de 47,5 mm a été retiré avec les liaisons sans destination de l'ancienne carte. Aucun routage double face de substitution n'a été ajouté.

TRIAC1 est actuellement CMS sur F.Cu : ce choix demande une solution de montage compatible avec une carte fabriquée essentiellement sur la face bleue. Cette empreinte est hors carte ; il faut traiter l'implantation et le boîtier réel dans la prochaine conception de puissance.

Les erreurs de perçage et d'anneau se concentrent sur l'empreinte Allegro, dont les deux grosses connexions primaires sont représentées par plusieurs trous. Ne pas les compter comme autant de défauts électroniques indépendants : examiner le dessin mécanique et le procédé d'atelier. Les trous de 0,5 mm, anneaux de 0,35 mm et intervalles entre trous ne respectent toutefois pas les minimums enregistrés dans votre projet.

Le LCD dépasse volontairement largement le contour par son corps : ses broches restent sur le PCB. Ce dépassement mécanique ne doit pas être confondu avec U3 totalement hors carte. Le débordement de l'enveloppe PICkit doit aussi être évalué selon le coffret et l'accès du câble, pas supprimé automatiquement.

## Contrôles réalisés et évolution

| Contrôle KiCad natif | Avant nettoyage | Après nettoyage |
|---|---:|---:|
| Empreintes PCB | 42 | 21 |
| Violations DRC hors connexions et parité | 138 | 99 |
| Signalements de court-circuit | 7 | 0 |
| Pistes pendantes | 1 | 0 |
| Connexions manquantes | 3 | 3 |
| Écarts de parité schéma/PCB | 36 | 15 |
| ERC du schéma | 19, dont 4 erreurs et 15 avertissements | Inchangé |

Les trois connexions manquantes concernent GND de TRIAC1, GND de U3 et +5 V de U3. Leur maintien est attendu tant que ces composants ne sont pas implantés et raccordés. Les 15 écarts de parité restants concernent les trois empreintes manquantes et les valeurs, empreintes et six nets de R3/R4/R7.

Le CSV [Composants_G206.csv](../../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv) contient les 24 références du schéma. La [nomenclature groupée](../../03_Electronique/G206A_CAMEL_TP_CAO/Nomenclature_G206_groupee.csv) comporte 19 lignes, pour une quantité totale de 24. Une comparaison exhaustive a été faite sur les 21 composants communs : les différences de nets se concentrent sur R3, R4 et R7. Les passifs à deux broches ont été contrôlés pour valeur et connexion ; leurs modèles, tensions de service, puissances et tolérances restent à renseigner lorsque absents.

### Analyses exécutées

- Analyse du schéma `analyze_schematic.py`, PCB `analyze_pcb.py --full`, croisement `cross_analysis.py`, Gerbers `analyze_gerbers.py`, thermique `analyze_thermal.py` et risque CEM `analyze_emc.py`.
- ERC, DRC avec parité et remplissage des zones, export de netlist et vues PDF par KiCad 10.0.6.
- Contrôle manuel des fichiers bruts, des connexions critiques et des fiches PIC16F1788, ACS756 et Displaytech 162B.
- Sept constats manuels structurés passés par le contrôle des ancrages et sources du skill, sans entrée mise en quarantaine.
- Simulation automatique SPICE tentée avec LTspice : son test de découplage a été ignoré, faute de mesure exploitable par l'adaptateur. Aucun succès global de simulation n'en est déduit.
- Simulation LTspice distincte, isolée et à vide de R11/R13 : VO = 0,130434781 V pour 5 V, 56 kΩ et 1,5 kΩ. Elle contrôle seulement ce pont de contraste.

### Faux positifs et limites des analyses

Le diagnostic CEM initial produit 47 alertes, principalement liées à l'absence de plan de référence sur la face opposée. Ce modèle est peu adapté à votre objectif de fabrication essentiellement simple face : il ne justifie pas d'ajouter un grand plan rouge ou des vias de couture. Les retours de masse, le découplage et la séparation des courants de puissance restent pertinents. Le score heuristique 58/100 n'est pas une mesure de conformité CEM.

Les alertes d'écart aux bibliothèques KiCad 10 peuvent venir de composants personnalisés ou de la migration depuis KiCad 8. Le brochage du LCD modifié a été contrôlé contre la fiche 162B et ne doit pas être « corrigé » vers le standard générique. Les différences de valeurs et de nets de R3/R4/R7 sont, elles, des écarts réels entre les deux fichiers.

La thermique a été lancée, mais aucun composant n'a pu être chiffré faute de dissipations et de modèle réel : l'outil signale une analyse sans résultat, pas une carte froide. Le triac nécessite son propre calcul avec le courant de la pompe et le radiateur éventuel.

Le cycle de vie n'a pas été audité via distributeur, faute de MPN renseignés et de clés API. Les fabricants indiquent actuellement ACS756 « Not for New Design » et 162B « Not Recommended for New Design » ; ce n'est pas une interdiction d'utiliser le matériel déjà disponible pour le TP, mais il faut éviter de supposer un réapprovisionnement pérenne.

Les Gerbers G206 existants contiennent seulement B.Cu, Edge.Cuts et le perçage : ce choix est compatible avec un typon simple face et des straps manuels. Les couches F.Cu/masque manquantes ne sont donc pas automatiquement une erreur de fabrication. **Ces exports précèdent le nettoyage et doivent être régénérés avant utilisation.**

Pas de revue précédente documentée trouvée pour ce projet ; la comparaison avant/après de cette intervention sert de point de départ. Pas de mesure sur pompe réelle, de test électrique, de firmware validé ou de qualification de sécurité/CEM. Les fiches concernent les références nominales des symboles ; elles ne prouvent pas que les pièces effectivement achetées seront ces modèles.

## Ordre de travail proposé

1. Préciser moteur, courant nominal/démarrage, capteur et boîtiers des composants de puissance.
2. Corriger les deux noms de nets et raccorder le conducteur primaire de U3 dans le schéma.
3. Définir la référence de phase isolée et les raccordements secteur/moteur.
4. Dimensionner optotriac, triac, résistances, protection, antiparasitage et thermique.
5. Corriger le buzzer et définir l'acquittement utilisateur.
6. Synchroniser les empreintes/nets avec le PCB puis implanter uniquement les nouveaux éléments, en conservant les autres composants selon votre consigne.
7. Router sur bleu avec un minimum de straps rouges, refaire ERC/DRC, puis régénérer les documents de fabrication.

## Sources techniques

- [Microchip PIC16F1788/9, DS40001675C](https://ww1.microchip.com/downloads/en/DeviceDoc/40001675C.pdf), brochage DIP28 p. 4, allocation p. 9, VCAP p. 111.
- [Allegro ACS756](https://www.allegromicro.com/-/media/files/datasheets/acs756-datasheet.ashx), application typique et brochage p. PDF 2, variantes p. PDF 3, caractéristiques de la variante 100B dans le tableau dédié.
- [Displaytech 162B série v2.1](https://www.seacomp.com/sites/default/files/datasheets/162B-Series-Displaytech-Spec.pdf), dessin mécanique et table de broches p. imprimée 7, rétroéclairage basse consommation p. imprimée 12.
- [onsemi FODM3011/3012/3022/3023/3052/3053](https://www.onsemi.com/pdf/datasheet/fodm3053_nf098-d.pdf), valeurs maximales p. 2. Lecture via la documentation web fabricant ; téléchargement local bloqué par le serveur.
- [Microchip PIC16F1788, état produit](https://www.microchip.com/en-us/product/pic16f1788), [Displaytech 162B, état produit](https://www.seacomp.com/product/character-lcd-display-162b).

Le présent rapport est une revue des fichiers et une aide à la conception, pas une validation de mise sous tension du montage secteur.

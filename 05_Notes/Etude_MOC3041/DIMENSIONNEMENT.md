# Proposition MOC3041, puissance et mesure de courant

12 octobre 2026. Étude ciblée, sans modification du schéma ou du PCB. Référence de pompe et triac de puissance non communiquées : les valeurs secteur restent une base de travail, pas un dimensionnement de fabrication.

[Ouvrir la simulation Anatomy](../../07_Visualisation_Anatomy/simulation.html). Réglages : résistance LED, courant supposé, déphasage, gain et IGT hypothétique. Les positions illustrées proviennent du PCB actuel ; elles ne représentent pas une implantation modifiée.

## Décision et différences avec le projet actuel

Le choix annoncé est **MOC3041**. La référence fabricant/suffixe réellement achetée reste à préciser. Cette étude utilise la fiche **onsemi MOC3041M** ; un autre fabricant doit être vérifié séparément.

Le schéma enregistré utilise encore `Relay_SolidState:FODM3011`, 4 broches. **Ne pas simplement renommer sa valeur MOC3041.** Le MOC a 6 broches : LED 1 anode / 2 cathode ; sortie 4 et 6 ; 3 et 5 non raccordées. Il faut remplacer symbole et empreinte puis vérifier les nets. La commande marche/arrêt au voisinage du zéro de tension correspond au principe du projet ; cet optotriac n'est pas un variateur à angle de phase libre, ni un retour de mesure du zéro secteur vers le PIC. [Fiche onsemi, pages 1–3 et 6–7](https://www.onsemi.com/pdf/datasheet/moc3043m-d.pdf).

La netlist actuelle montre encore : R7 sur `RB2` distinct de la sortie PIC ; U3 sur `AN0` distinct de l'entrée PIC ; primaire IP+/IP− ouvert. R4 est **en série avec M1**, tandis que M1 et A1 reviennent tous deux au même net `AC`. Il manque une vraie entrée phase/neutre à deux bornes. Aucun choix de résistance ne rend cette boucle fonctionnelle. Confiance élevée : analyse du fichier actuellement enregistré, `schematic_actuel.json` local.

## LED de l'optotriac : proposition calculée

Je recommande un transistor côté logique : `+5 V → RLED → MOC.1`, `MOC.2 → collecteur/drain`, émetteur/source à GND. Le PIC pilote la base/grille avec rappel d'arrêt. Cela évite de faire fournir directement le courant LED au PIC et permet un état arrêté au reset.

Hypothèses de calcul : alimentation 5 V ±5 %, résistance ±5 %, chute LED de 1,5 V pour le coin bas ; chute du transistor 0,2 V. Pour le coin haut illustratif : chute LED 1,1 V et transistor 0,1 V. **1,1 V n'est pas une borne minimale garantie par le fabricant.** La fiche indique IFT maximal 15 mA à 25 °C ; les essais ci-dessous ne garantissent pas toutes les températures, la durée de vie ni toutes les références de transistor.

`Imin = (4,75 − 1,50 − 0,20)/(1,05 × RLED)`.

| RLED | Imin simulé | Nominal simulé | Coin haut simulé | Conclusion à 25 °C selon ces hypothèses |
|---|---:|---:|---:|---|
| 150 Ω | 19,37 mA | 23,67 mA | 28,42 mA | Proposition avec marge |
| 180 Ω | 16,14 mA | 19,72 mA | 23,68 mA | Marge plus faible |
| 220 Ω | 13,20 mA | 16,14 mA | 19,38 mA | Coin bas sous 15 mA |
| 270 Ω | 10,76 mA | 13,15 mA | 15,79 mA | À revoir pour MOC3041 |

**RLED proposée : 150 Ω / ¼ W**, tolérance 5 % ou mieux. Dissipation maximale dans les coins simulés : 0,115 W. Même une borne conservatrice de chute LED/transistor nulle donne `5,25²/(150×0,95)=0,193 W` ; vérifier le déclassement thermique dans le boîtier et retenir ½ W si l'ambiance réduit trop la marge. Ne pas déduire de cette borne un courant réel de LED.

Pour un NPN choisi pour la saturation à environ 30 mA, une base **1 kΩ** et un rappel base-émetteur **100 kΩ** sont un point de départ : à VOH=4 V, VBE=0,9 V et Rbase=1,05 kΩ, IB≈2,95 mA. Il reste à vérifier VOH garanti du PIC à cette charge, VCE(sat), courant de base nécessaire, brochage et température du transistor réel. Une solution MOSFET doit vérifier RDS(on) à la tension de grille disponible, et pas seulement sa tension de seuil. Aucun transistor réel n'a été validé par le modèle à chute fixe.

## Résistances côté secteur : exemple, puis vérifications indispensables

La figure 10 de la fiche onsemi présente, pour 240 V, **360 Ω** dans la commande du phototriac et **330 Ω** entre gâchette et A1. Son réseau RC est 39 Ω + 10 nF, avec adaptation indiquée pour certaines charges très inductives. **Ce réseau n'est pas une valeur universelle pour notre pompe.** [Exemple onsemi, page 6](https://www.onsemi.com/pdf/datasheet/moc3043m-d.pdf).

Proposition de topologie : `phase → fusible → IP+ → IP− → moteur → A2 ; A1 → neutre`. Puis `A2 → Rg → MOC.6`, `MOC.4 → G`, `Rgk entre G et A1`. Les masses du PIC et du capteur ne sont pas reliées au neutre ou à A1. Le conducteur primaire du capteur est distinct de sa partie analogique.

| Fonction | Base de travail | Ce qui manque pour la fixer |
|---|---|---|
| Résistance de commande Rg | 360 Ω, composant résistant aux impulsions | IGT dans les quadrants utilisés, VGT, température, seuil d'inhibition MOC, courant d'accrochage et surcharge de sortie |
| Rappel gâchette-A1 Rgk | 330 Ω dans l'exemple constructeur | Immunité dv/dt, courant dérivé et IGT du triac réel |
| Résistance du snubber | 39 Ω / 10 nF dans l'exemple ; autre valeur possible | Inductance, courant de commutation, dv/dt, courant de décharge, répétition et condensateur adapté secteur |
| R4 actuellement en série avec la pompe | Fonction à supprimer/revoir dans la topologie | Une résistance de commande ne doit pas limiter le courant du moteur en série |

Exemple d'équation, sans valeur de triac garantie : `IG ≈ (|VA2−A1| − VTM_opto − VGT)/Rg − VGT/Rgk`.

Avec VTM=3 V, VGT supposé=1,3 V, Rg=360 Ω, Rgk=330 Ω :

| IGT hypothétique | Tension nécessaire calculée |
|---|---:|
| 10 mA | 9,32 V |
| 25 mA | 14,72 V |
| 35 mA | 18,32 V |
| 50 mA | 23,72 V |

La simulation utilise une fenêtre illustrative de 20 V. C'est un plafond de caractéristique, **pas une fenêtre garantie à 20 V pour chaque pièce**. Il ne suffit pas de constater 18,32 < 20 pour garantir un triac IGT=35 mA. Un triac demandant 50 mA échoue déjà dans cette hypothèse. Le réamorçage après le zéro du courant sur charge inductive exige une vérification spécifique. [Principes de dimensionnement et validation expérimentale, ST AN5114](https://www.st.com/resource/en/application_note/an5114-controlling-a-triac-with-a-phototriac-stmicroelectronics.pdf).

Il faut vérifier puissance **impulsionnelle**, énergie, tension admissible et déclassement des résistances, pas seulement leur wattage continu. Exemple défavorable : 253 V efficaces donnent 358 V crête ; une résistance de 360 Ω limiterait à environ 0,98 A si presque toute cette tension lui était appliquée après un allumage intempestif. Cela dépasse le régime répétitif du phototriac : la protection et la durée d'impulsion sont essentielles. Le calcul n'est pas un régime normal admissible.

Le MOC3041 a une tenue de blocage de 400 V, distincte de sa tenue d'isolation logique/secteur. 230 V donnent 325 V crête et 253 V donnent 358 V avant surtensions. La marge est limitée : examiner la coordination de protection, ou une famille zéro-cross à tenue supérieure si le composant n'est pas imposé. Fusible, limitation des surtensions, triac et dissipateur doivent être définis avec la pompe. Le 0,5 mm global de clearance du projet ne constitue pas une validation d'isolement secteur/logique.

## Faut-il amplifier le capteur ?

**Pas obligatoirement.** L'ACS756 ±100 A nominal du schéma dispose déjà d'une amplification interne. Sa sortie vaut environ `VCC/2 + 20 mV/A × i(t)` à 5 V. Avec le PIC en ADC **12 bits**, référence 5 V, le pas théorique est 1,22 mV, soit **61 mA/code**. Cela ne signifie pas 61 mA de précision garantie. [Fiche ACS756, pages 4 et 6](https://www.allegromicro.com/-/media/files/datasheets/acs756-datasheet.ashx), [ADC du PIC, section 17](https://ww1.microchip.com/downloads/en/DeviceDoc/40001675C.pdf).

Exemple à 2 A efficaces sinusoïdaux : signal de 40 mV efficaces et 56,6 mV crête autour de 2,5 V. La fiche donne 6 mV de bruit typique **6σ** avec 10 nF à la sortie, et une dérive d'offset pouvant atteindre ±30 mV dans ses conditions de température. **6 mV n'est pas un bruit RMS de 6 mV.** Amplifier augmente aussi bruit et dérive. Commencer par alimentation propre, 100 nF local, acquisition ADC correcte, offset mesuré pompe arrêtée et calcul RMS sur plusieurs périodes.

Point de départ signal : 1 kΩ en série et 10 nF à l'entrée ADC ; simulation idéale fc=15,915 kHz, attenuation à 50 Hz≈0,999995 et retard de phase≈0,18°. Ce RC reste léger et **n'est pas un filtre anti-repliement suffisant à lui seul**. Définir cadence d'échantillonnage et bande utile ; le moyennage numérique ne corrige pas un repliement déjà produit.

Pour une mesure plus fine à faible courant, envisager un gain recentré : `VADC=2,5+G×(Vcapteur−2,5)`, avec référence 2,5 V filtrée, ampli compatible 5 V, entrées/sortie et charge vérifiées. Une configuration non inverseuse référencée utilise `G=1+Rf/Rg` : par exemple 10 kΩ / 90 kΩ donne ×10, mais cela reste un exemple nécessitant choix d'ampli et référence correctement pilotée. **Ne pas amplifier la tension complète de 2,5 V par 10.**

| Gain | Pas idéal en courant | Limite sinusoïdale avec sortie 0,2–4,8 V |
|---|---:|---:|
| ×1 | 61,0 mA/code | 81,3 A efficaces |
| ×5 | 12,2 mA/code | 16,3 A efficaces |
| ×10 | 6,1 mA/code | 8,13 A efficaces |

À ×10, un démarrage à 10 A efficaces écrête déjà. Il faudrait une plage adaptée, une seconde voie directe ou un gain commutable si l'alarme doit mesurer aussi le démarrage/surcourant. Le PIC possède des amplis internes, mais cela ne prouve pas que leurs broches/configurations sont disponibles avec l'affectation LCD actuelle. Une voie tension/phase **isolée** reste nécessaire pour le déphasage : le MOC de commande ne fournit aucun signal de retour au PIC.

## Simulations réellement exécutées et limites

LTspice 26.0.1 a exécuté les trois fichiers `.cir` joints. [Lancer les essais](lancer_spice.py), avec `LTSPICE_PATH` si nécessaire. Les journaux sont joints en texte.

- LED : 12 combinaisons de résistance et coins ; valeurs du tableau ci-dessus. Modèle à chutes fixes, pas modèle optique/thermique fabricant.
- RC : fc mesuré 15 915,49 Hz. Le journal AC affiche certaines mesures scalaires sous forme complexe/dB ; ne pas lire « −14,89 dB » de la mesure `ph()` comme un gain du filtre. C'est l'encodage du scalaire de phase d'environ −0,18°.
- Capteur/gain : 9 combinaisons, gain idéal et écrêtage comportemental. À 2 A, RMS AC mesuré 0,040 V (×1), 0,200 V (×5), 0,400 V (×10). Écrêtage à ×10 pour 10/15 A. Compression des données désactivée pour fiabiliser les mesures.

**Anatomy représente l'étude et ses résultats ; le calcul électrique SPICE est réalisé par LTspice.** Le modèle navigateur simule une charge R-L équivalente à 50 Hz. Il attend une première occasion d'amorçage, puis suppose une commande de gâchette idéale maintenue. **Il ne valide pas le réamorçage réel MOC/triac d'un moteur inductif.** Le scénario d'exemple commande ON à 5 ms et OFF à 35 ms ; le courant peut continuer jusqu'à son zéro après l'arrêt de la LED. Aucun modèle fabricant de triac réel, snubber, mécanique moteur, bruit, thermique ou surtension n'est inclus. Ces essais ciblés ne sont pas une validation globale de la carte.

## Informations encore requises

- Plaque pompe : tension, courant nominal, puissance absorbée, type de moteur et courant/durée de démarrage.
- Référence complète du triac : fabricant, suffixe, quadrants et IGT, commutation, boîtier et dissipation.
- Fabricant/suffixe MOC3041 acheté ; référence précise du capteur ; précision et seuils d'alarme attendus.

Les broches et caractéristiques citées sont sourcées ; calculs arithmétiques déterministes sous hypothèses explicites ; behavior moteur et ampli de confiance limitée. Les propositions n'ont pas été appliquées dans KiCad.

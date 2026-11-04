// Brouillon du rapport de conception — projet vide-cave (BUT GEII 2, SAé 31/42, groupe 311)
const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, HeadingLevel, AlignmentType, Table, TableRow, TableCell,
  WidthType, ShadingType, BorderStyle, ImageRun, PageBreak, TableOfContents, Footer, Header,
  PageNumber, LevelFormat, PageOrientation,
} = require("docx");

const REPO = "C:/Users/rafae/orca/workspaces/Projet vide cave/spiderfish";
const HERE = __dirname;
const OUT = process.argv[2] || "Rapport_conception_vide-cave.docx";

const BLEU = "0E3B4A", TEAL = "0E7C86", ROUGE = "C8473D", GRIS = "6B7F8C", FOND = "EEF3F5", ORANGE_FOND = "FDF1DD";
const FONT = "Calibri";
const W = 9638; // largeur utile A4 avec marges de 2 cm (en DXA)

// ---------- Aides ----------
const r = (text, o = {}) => new TextRun(Object.assign({ text, font: FONT }, o));
const P = (content, o = {}) => new Paragraph(Object.assign({ children: Array.isArray(content) ? content : [r(content)], spacing: { after: 120, line: 276 } }, o));
const H1 = (t) => new Paragraph({ heading: HeadingLevel.HEADING_1, children: [r(t)], pageBreakBefore: true });
const H2 = (t) => new Paragraph({ heading: HeadingLevel.HEADING_2, children: [r(t)] });
const H3 = (t) => new Paragraph({ heading: HeadingLevel.HEADING_3, children: [r(t)] });
const puce = (content, level = 0) => new Paragraph({ numbering: { reference: "puces", level }, children: Array.isArray(content) ? content : [r(content)], spacing: { after: 60 } });
const num = (content, ref = "num") => new Paragraph({ numbering: { reference: ref, level: 0 }, children: Array.isArray(content) ? content : [r(content)], spacing: { after: 60 } });
const B = (t) => r(t, { bold: true });
const AFAIRE = (t) => r("[À compléter : " + t + "]", { color: ROUGE, bold: true });
const legende = (t) => new Paragraph({ alignment: AlignmentType.CENTER, spacing: { before: 60, after: 240 }, children: [r(t, { italics: true, size: 19, color: GRIS })] });
const image = (file, wcm, ratio) => {
  const wpx = Math.round(wcm * 37.8);
  return new Paragraph({ alignment: AlignmentType.CENTER, spacing: { before: 120 }, children: [new ImageRun({ type: "png", data: fs.readFileSync(file), transformation: { width: wpx, height: Math.round(wpx * ratio) }, altText: { title: path.basename(file), description: path.basename(file), name: path.basename(file) } })] });
};
const encadre = (children, fill = FOND) => new Table({
  width: { size: W, type: WidthType.DXA }, columnWidths: [W],
  borders: { top: { style: BorderStyle.NONE }, bottom: { style: BorderStyle.NONE }, left: { style: BorderStyle.NONE }, right: { style: BorderStyle.NONE }, insideHorizontal: { style: BorderStyle.NONE }, insideVertical: { style: BorderStyle.NONE } },
  rows: [new TableRow({ children: [new TableCell({ width: { size: W, type: WidthType.DXA }, shading: { type: ShadingType.CLEAR, fill, color: "auto" }, margins: { top: 120, bottom: 120, left: 180, right: 180 }, children })] })],
});
const bord = { style: BorderStyle.SINGLE, size: 4, color: "C9D6DB" };
const tableau = (entetes, lignes, largeurs) => {
  const tot = largeurs.reduce((a, b) => a + b, 0);
  const cols = largeurs.map((l) => Math.round((l * W) / tot));
  cols[cols.length - 1] += W - cols.reduce((a, b) => a + b, 0);
  const cell = (txt, i, entete) => new TableCell({
    width: { size: cols[i], type: WidthType.DXA },
    shading: entete ? { type: ShadingType.CLEAR, fill: BLEU, color: "auto" } : undefined,
    margins: { top: 50, bottom: 50, left: 90, right: 90 },
    children: [new Paragraph({ children: Array.isArray(txt) ? txt : [r(String(txt), entete ? { bold: true, color: "FFFFFF", size: 19 } : { size: 19 })] })],
  });
  return new Table({
    width: { size: W, type: WidthType.DXA }, columnWidths: cols,
    borders: { top: bord, bottom: bord, left: bord, right: bord, insideHorizontal: bord, insideVertical: bord },
    rows: [new TableRow({ tableHeader: true, children: entetes.map((e, i) => cell(e, i, true)) }), ...lignes.map((l) => new TableRow({ cantSplit: true, children: l.map((c, i) => cell(c, i, false)) }))],
  });
};
const vide = () => new Paragraph({ children: [], spacing: { after: 60 } });

// ---------- Couverture ----------
const couverture = [
  P([r("UNIVERSITÉ CLAUDE BERNARD LYON 1", { bold: true, size: 22, color: BLEU })], { alignment: AlignmentType.CENTER, spacing: { after: 40 } }),
  P([r("IUT Lyon 1 — Département Génie Électrique et Informatique Industrielle", { size: 22, color: BLEU })], { alignment: AlignmentType.CENTER, spacing: { after: 40 } }),
  P([r("BUT GEII 2ᵉ année — SAé 31/42 — Groupe 311", { size: 22, color: BLEU })], { alignment: AlignmentType.CENTER, spacing: { after: 1200 } }),
  P([r("COMPTE RENDU DE CONCEPTION", { bold: true, size: 28, color: TEAL })], { alignment: AlignmentType.CENTER, spacing: { after: 240 } }),
  P([r("Surveillance et protection d'une pompe de vide-cave par mesure du courant", { bold: true, size: 44, color: BLEU })], { alignment: AlignmentType.CENTER, spacing: { after: 200 } }),
  P([r("Carte de commande à PIC16F1788 : capteur à effet Hall, optotriac et triac, LCD et alarmes", { size: 26, color: GRIS })], { alignment: AlignmentType.CENTER, spacing: { after: 600 } }),
  image(path.join(REPO, "04_Maquette_3D/01_installation.png"), 11, 0.75),
  P([r("Maquette 3D de l'installation (Blender)", { italics: true, size: 18, color: GRIS })], { alignment: AlignmentType.CENTER, spacing: { after: 600 } }),
  P([B("Réalisé par : "), AFAIRE("Prénom NOM, Prénom NOM")], { alignment: AlignmentType.CENTER }),
  P([B("Enseignant encadrant : "), AFAIRE("nom de l'enseignant")], { alignment: AlignmentType.CENTER }),
  P([B("Lieu : "), r("IUT Lyon 1 — département GEII")], { alignment: AlignmentType.CENTER }),
  P([B("Année universitaire : "), r("2026-2027 · "), B("Date de remise : "), AFAIRE("date")], { alignment: AlignmentType.CENTER }),
];

// ---------- Sommaire ----------
const sommaire = [
  new Paragraph({ heading: HeadingLevel.HEADING_1, children: [r("Sommaire")], pageBreakBefore: true }),
  new TableOfContents("Sommaire", { hyperlink: true, headingStyleRange: "1-2" }),
  P([r("Dans Word : clic droit sur le sommaire → Mettre à jour les champs → Mettre à jour toute la table.", { italics: true, color: GRIS, size: 18 })]),
  H2("Liste des annexes"),
  tableau(["N°", "Titre de l'annexe", "Page"], [
    ["A1", "Schéma structurel complet (KiCad, format A3)", "…"],
    ["A2", "Typon / implantation du PCB (face B.Cu, straps F.Cu)", "…"],
    ["A3", "Extraits de fiches techniques : PIC16F1788, ACS756, MOC3041, triac, LCD 162B", "…"],
    ["A4", "Simulations LTspice : courant de la LED du MOC3041, filtre RC, gain du capteur", "…"],
    ["A5", "Planning (diagramme de Gantt) actualisé", "…"],
    ["A6", "Demande de composants (document séparé)", "…"],
  ], [1, 8, 1]),
];

// ---------- 1. Introduction ----------
const intro = [
  H1("1. Introduction"),
  P("Un vide-cave est une pompe qui évacue automatiquement l'eau d'infiltration d'une cave ou d'un sous-sol. Elle reste à l'arrêt la plupart du temps, parfois pendant plusieurs mois. Une panne (moteur bloqué ou grippé, pompe à sec, défaut électrique) peut donc rester invisible jusqu'au jour où la pompe est réellement nécessaire, c'est-à-dire au moment où sa défaillance a le plus de conséquences."),
  P("Le projet consiste à concevoir une carte électronique, associée à une pompe existante (moteur asynchrone monophasé 230 V), qui surveille automatiquement son bon fonctionnement. Le principe retenu est la mesure du courant absorbé par le moteur, image de sa charge mécanique :"),
  puce([B("courant trop fort"), r(" : moteur bloqué ou grippé → arrêt du moteur et alarme ;")]),
  puce([B("courant normal"), r(" : la pompe évacue l'eau → fonctionnement autorisé ;")]),
  puce([B("courant trop faible"), r(" : pompe à vide, sans eau → arrêt et information de l'utilisateur.")]),
  P("Le cahier des charges impose en outre un autotest automatique tous les 3 jours, d'une durée maximale d'une minute, pour vérifier la pompe même en l'absence d'eau, ainsi qu'une information locale par écran LCD, LED et buzzer. La pompe, le flotteur de niveau et la supervision à distance par Wi-Fi sont hors du périmètre de la carte."),
  P("Le synoptique simple ci-dessous présente le système dans son environnement. Le courant du moteur est mesuré par un capteur à effet Hall ; le microcontrôleur PIC16F1788 décide et commande le moteur à travers un optotriac et un triac de puissance ; l'utilisateur est informé par le LCD, la LED et le buzzer."),
  encadre([
    P([r("Secteur 230 V  →  Triac  →  Moteur de la pompe", { font: "Consolas", size: 20 })], { alignment: AlignmentType.CENTER, spacing: { after: 40 } }),
    P([r("↑ optotriac            ↓ courant (capteur à effet Hall)", { font: "Consolas", size: 20 })], { alignment: AlignmentType.CENTER, spacing: { after: 40 } }),
    P([r("PIC16F1788  →  LCD · LED · buzzer", { font: "Consolas", size: 20 })], { alignment: AlignmentType.CENTER, spacing: { after: 0 } }),
  ]),
  legende("Figure 1 — Synoptique simple du système (à remplacer par un dessin draw.io)"),
];

// ---------- 2. Présentation approfondie ----------
const presentation = [
  H1("2. Présentation approfondie du projet"),
  H2("2.1 Évolution des choix depuis la pré-étude"),
  P("La pré-étude laissait plusieurs choix ouverts. Le tableau suivant indique les solutions retenues pour la conception et leur justification."),
  tableau(["Fonction", "Pré-étude", "Conception", "Justification"], [
    ["Traitement", "ESP32 pressenti", "PIC16F1788", "Microcontrôleur étudié en cours (MPLAB X, MCC, XC8) ; ADC 12 bits, timers et UART suffisants ; le Wi-Fi devient une évolution"],
    ["Coupure du moteur", "Relais électromécanique", "Optotriac MOC3041 + triac", "Pas d'usure mécanique, commutation silencieuse au passage par zéro de la tension, isolement assuré par l'optotriac"],
    ["Mesure du courant", "Hall, transformateur ou shunt", "ACS756 (effet Hall)", "Isolement galvanique intrinsèque ; sortie en tension directement lisible par l'ADC"],
    ["Mesure du déphasage", "Prévue", [AFAIRE("décision")], "Nécessite une voie de tension isolée (détecteur de passage par zéro), absente de la carte actuelle"],
    ["Carte", "À concevoir", "Carte G206 adaptée", "Réutilisation d'une carte existante : alimentation, PIC, LCD et ICSP déjà routés"],
  ], [2, 2.2, 2.3, 5]),
  vide(),
  encadre([P([B("Décision à rédiger — déphasage. "), r("Soit le déphasage est conservé : ajout d'une voie de passage par zéro de la tension isolée (type H11AA1) sur une entrée de capture du PIC, avec φ = 360° × Δt / T. Soit il est retiré du périmètre, parce que l'amplitude du courant suffit à distinguer le blocage et le fonctionnement à vide. Indiquer ici le choix et sa raison.")], { spacing: { after: 0 } })], ORANGE_FOND),

  H2("2.2 Notions nouvelles utilisées"),
  H3("Capteur de courant à effet Hall"),
  P("Le courant à mesurer traverse un conducteur placé près d'une cellule à effet Hall. Le champ magnétique créé par ce courant produit une tension proportionnelle, sans contact électrique entre le circuit de puissance et la sortie du capteur. L'ACS756 utilisé fournit une sortie centrée sur VCC/2 (2,5 V sous 5 V) à laquelle s'ajoute 20 mV par ampère."),
  H3("Optotriac à commutation au zéro de tension"),
  P("Un optotriac associe une LED et un phototriac dans un même boîtier isolé. Quand la LED est alimentée, le phototriac déclenche la gâchette du triac de puissance. Le MOC3041 intègre un détecteur de passage par zéro : il ne s'amorce qu'au voisinage du zéro de la tension secteur, ce qui limite les parasites. Il ne permet pas la variation de puissance par angle de phase."),
  H3("Triac sur charge inductive"),
  P("Le triac conduit dans les deux sens une fois amorcé et se bloque au passage par zéro du courant. Avec un moteur, charge inductive, le courant est en retard sur la tension : un réseau RC (snubber) limite la vitesse de remontée de la tension à ses bornes et évite les réamorçages intempestifs."),
  H3("Valeur efficace numérique"),
  P("Le PIC échantillonne la tension du capteur sur un nombre entier de périodes du secteur (20 ms à 50 Hz), retranche l'offset mesuré moteur arrêté, puis calcule la racine de la moyenne des carrés : I_eff = √( (1/N) Σ i²(k) )."),

  H2("2.3 Synoptique détaillé"),
  P("Le synoptique détaillé découpe la carte en six fonctions. Une barrière d'isolement sépare le côté secteur 230 V du côté basse tension 5 V : seuls le capteur à effet Hall et l'optotriac la traversent, sans liaison électrique. Les noms des signaux sont identiques dans le texte, le synoptique et le schéma structurel."),
  image(path.join(HERE, "fig_synoptique.png"), 16.5, 1125 / 2507),
  legende("Figure 2 — Synoptique détaillé (à reprendre au format A4 avec cartouche : titre, auteurs, date)"),
  tableau(["Fonction", "Composants", "Entrées", "Sorties"], [
    ["FP1 Alimentation", "Bornier J1, C1 47 µF, C2 et C5 100 nF", "5V_EXT", "+5V, GND"],
    ["FP2 Mesure du courant", "ACS756 (U3), filtre 1 kΩ / 10 nF", "I_MOTEUR", "V_CAPTEUR → AN0"],
    ["FP3 Traitement", "PIC16F1788 (U1), R12, SW4", "V_CAPTEUR", "CMD_OPTO, bus LCD, LED, BUZ, TX"],
    ["FP4 Commande de puissance", "Transistor, MOC3041, triac, Rg, Rgk, snubber, fusible", "CMD_OPTO", "PHASE_MOTEUR"],
    ["FP5 Interface utilisateur", "LCD 162B (DS1), LED D4, buzzer BZ1", "Bus LCD, LED, BUZ", "Affichage, signal lumineux et sonore"],
    ["FP6 Programmation et débogage", "J4 ICSP (MPLAB SNAP), J2 UART TTL", "PGD, PGC, MCLR, RX", "TX"],
  ], [2.6, 4, 1.8, 2.6]),
];

// ---------- 3. Gestion de projet ----------
const gestion = [
  H1("3. Documents de gestion de projet"),
  H2("3.1 Organigramme des tâches"),
  P("Chaque tâche élémentaire ci-dessous se retrouve dans le planning actualisé (annexe A5)."),
  puce([B("1. Gestion de projet")]), puce("Planning, Kanban, dépôt Git commun, réunions", 1),
  puce([B("2. Étude")]), puce("Cahier des charges et pré-étude", 1), puce("Relevé de la plaque de la pompe, choix des seuils Imin et Imax", 1), puce("Choix des composants : capteur, optotriac, triac, protections", 1),
  puce([B("3. Matériel")]), puce("Correction du schéma structurel", 1), puce("Calculs et simulations LTspice", 1), puce("Routage du PCB, contrôle DRC", 1), puce("Fabrication, câblage", 1),
  puce([B("4. Logiciel")]), puce("Configuration MCC (horloge, ADC, timer, ports)", 1), puce("Pilote LCD, mesure du courant efficace", 1), puce("Machine d'états, autotest, alarmes", 1),
  puce([B("5. Essais")]), puce("Basse tension seule, puis charge résistive, puis pompe", 1),
  puce([B("6. Documents")]), puce("Rapports de conception et de fabrication, soutenances", 1),
  H2("3.2 Planning actualisé"),
  P([r("Proposition de répartition des séances restantes, à reporter dans le diagramme de Gantt. "), AFAIRE("valider les dates avec le binôme")]),
  tableau(["Période", "Tâches", "Responsable"], [
    ["09/11 – 16/11", "Relevé de la pompe ; décisions (déphasage, capteur, triac) ; configuration MCC", "…"],
    ["16/11 – 30/11", "Correction du schéma ; pilote LCD et lecture de l'ADC", "…"],
    ["30/11 – 14/12", "Routage et DRC du PCB ; machine d'états et autotest", "…"],
    ["14/12 – 11/01", "Rédaction du rapport de conception ; demande de composants", "…"],
    ["S4", "Fabrication, essais, rapport de fabrication, soutenance finale", "…"],
  ], [2, 6.5, 2]),
  H2("3.3 Retards et moyens de rattrapage"),
  P("Retards constatés :"),
  puce("la carte réutilisée contenait des liaisons rompues et des composants hérités, qu'il a fallu relever et nettoyer ;"),
  puce("les caractéristiques de la pompe (courant nominal et de démarrage) ne sont pas encore connues ;"),
  puce("le programmateur MPLAB SNAP a été bloqué lors d'une mise à jour automatique le 12 octobre."),
  P("Moyens de rattrapage, la date de fin restant inchangée :"),
  puce("conduire en parallèle la partie matérielle (schéma, PCB) et la partie logicielle (MCC, programme) ;"),
  puce("tester le programme sur la partie basse tension avant tout raccordement au secteur ;"),
  puce("partager les fichiers par un dépôt Git commun, synchronisé en un double-clic."),
];

// ---------- 4. Schéma structurel ----------
const schema = [
  H1("4. Schéma structurel détaillé"),
  P("Le schéma structurel complet figure en annexe A1. Il reprend la disposition et les noms du synoptique détaillé. Cette partie décrit chaque fonction et justifie le choix et la valeur des composants."),
  H2("4.1 Corrections à apporter avant validation"),
  P("Le contrôle de la carte adaptée a mis en évidence les points suivants, à corriger dans le schéma avant de le figer :"),
  tableau(["Point", "Constat", "Correction"], [
    ["Mesure → PIC", "Sortie de U3 sur le net AN0, entrée du PIC sur « capteur de courant »", "Un seul net V_CAPTEUR"],
    ["Commande → optotriac", "Sortie RB2 et résistance R7 sur deux nets différents", "Un seul net CMD_OPTO"],
    ["Primaire du capteur", "IP+ et IP− non câblés", "Insérer le capteur en série avec le moteur"],
    ["Optotriac", "FODM3011, 4 broches, tenue 250 V", "MOC3041, 6 broches, tenue 400 V : symbole et empreinte"],
    ["Raccordement secteur", "Symbole AC à une broche ; R4 en série avec le moteur", "Bornier phase/neutre, fusible, bornier moteur ; supprimer R4"],
    ["R3, R4, R7", "Valeur « R », sans empreinte", "Valeurs calculées en 4.5"],
    ["Buzzer BZ1", "Borne + reliée à GND", "Inverser, choisir la référence"],
    ["Découplage", "Pas de 100 nF sur U3 ; C2 à 33 mm du PIC", "Ajouter et rapprocher"],
    ["Mise au point", "Pas de point de test", "Points de test +5V, GND, V_CAPTEUR, CMD_OPTO"],
  ], [2.2, 4.3, 3.6]),
  H2("4.2 FP1 — Alimentation"),
  P("La carte reçoit 5 V continus par le bornier J1. C1 = 47 µF constitue la réserve d'énergie ; C2 = 100 nF découple le PIC et C5 = 100 nF le LCD. Aucun régulateur n'est présent : la source 5 V doit être isolée du secteur."),
  P([AFAIRE("préciser la source 5 V : bloc externe ou module secteur/5 V isolé sur la carte")]),
  H2("4.3 FP2 — Mesure du courant"),
  P("Le courant du moteur traverse le primaire de l'ACS756 (broches IP+ et IP−). Sa sortie VIOUT vaut :"),
  P([r("V_CAPTEUR = 2,5 V + 0,020 × i(t)", { bold: true })], { alignment: AlignmentType.CENTER }),
  P("Un filtre passe-bas R = 1 kΩ, C = 10 nF limite le bruit à l'entrée de l'ADC. Sa fréquence de coupure fc = 1 / (2π × 1 kΩ × 10 nF) ≈ 15,9 kHz est très au-dessus de 50 Hz : le signal utile n'est ni atténué ni déphasé de façon notable (−0,18° à 50 Hz, simulation LTspice en annexe A4)."),
  P("L'ADC 12 bits sous 5 V a un pas de 5 / 4096 ≈ 1,22 mV, soit 1,22 / 20 = 61 mA par pas."),
  encadre([P([B("Limite. "), r("Pour une pompe d'environ 300 W (≈ 1,3 A sous 230 V, hypothèse à confirmer par la plaque), la variante ±100 A n'utilise qu'environ 21 pas de l'ADC. Deux solutions : un capteur de plus petit calibre (par exemple ACS712-5A, 185 mV/A), ou une amplification ×5 centrée sur 2,5 V (12 mA par pas) en conservant la mesure du courant de démarrage. "), AFAIRE("solution retenue")], { spacing: { after: 0 } })], ORANGE_FOND),
  H2("4.4 FP3 — Traitement : PIC16F1788"),
  P("Le PIC est alimenté en 5 V (VDD broche 20, VSS broches 8 et 19). R12 = 10 kΩ rappelle MCLR au +5 V ; le bouton SW4 réalise le reset. L'oscillateur interne est utilisé, ce qui libère RA6 et RA7 pour le LCD (VCAPEN désactivé, entrées du LCD configurées en numérique)."),
  tableau(["Fonction", "Port", "Broche DIP28"], [
    ["Mesure du courant (ADC)", "RA0 / AN0", "2"],
    ["LCD RS, RW, E", "RA1, RA2, RA3", "3, 4, 5"],
    ["LCD D4, D5, D6, D7", "RA4, RA5, RA7, RA6", "6, 7, 9, 10"],
    ["LED D4", "RB0", "21"],
    ["Buzzer", "RB1", "22"],
    ["Commande de l'optotriac", "RB2", "23"],
    ["UART TX, RX", "RC6, RC7", "17, 18"],
    ["Programmation ICSP CLK, DAT", "RB6, RB7", "27, 28"],
    ["Reset", "MCLR", "1"],
  ], [5, 3, 2]),
  H2("4.5 FP4 — Commande de puissance"),
  H3("Commande de la LED de l'optotriac"),
  P("La LED du MOC3041 est commandée par un transistor NPN plutôt que directement par le PIC : il fournit le courant nécessaire et, grâce à la résistance de rappel base-émetteur, garde l'optotriac bloqué pendant le reset du PIC."),
  P("Le MOC3041 garantit l'amorçage pour un courant de LED d'au plus 15 mA (IFT max). Le calcul est fait dans le cas défavorable : alimentation 5 V − 5 %, chute de la LED 1,5 V, transistor saturé 0,2 V, résistance + 5 % :"),
  P([r("I_min = (4,75 − 1,5 − 0,2) / (1,05 × R_LED)", { bold: true })], { alignment: AlignmentType.CENTER }),
  tableau(["R_LED", "I_min simulé", "I nominal", "Conclusion"], [
    ["150 Ω", "19,4 mA", "23,7 mA", "Retenue : marge au-dessus de 15 mA"],
    ["180 Ω", "16,1 mA", "19,7 mA", "Marge faible"],
    ["220 Ω", "13,2 mA", "16,1 mA", "Amorçage non garanti"],
    ["270 Ω", "10,8 mA", "13,2 mA", "Amorçage non garanti (ancienne valeur)"],
  ], [2, 2.2, 2.2, 4.5]),
  P("R_LED = 150 Ω, ¼ W (puissance maximale calculée 0,115 W). Base du transistor : 1 kΩ en série, 100 kΩ de rappel ; avec VOH = 4 V et VBE = 0,9 V, I_B ≈ 2,95 mA, suffisant pour saturer un transistor à 30 mA. Ces valeurs sont à vérifier avec la fiche du transistor retenu."),
  H3("Commande du triac"),
  P("Valeurs de départ issues du schéma d'application du fabricant pour 240 V, à valider avec le triac réel : Rg = 360 Ω entre A2 et la sortie du MOC3041, Rgk = 330 Ω entre gâchette et A1, snubber 39 Ω + 10 nF (classe X) aux bornes du triac. Le courant de gâchette vaut approximativement :"),
  P([r("I_G ≈ (|V_A2-A1| − V_TM − V_GT) / Rg − V_GT / Rgk", { bold: true })], { alignment: AlignmentType.CENTER }),
  P([r("Topologie : phase → fusible → IP+ / IP− du capteur → moteur → A2 du triac ; A1 → neutre. "), AFAIRE("référence du triac (courant, IGT, boîtier) et calcul du dissipateur")]),
  H3("Sécurité électrique"),
  P("La tension secteur atteint 325 V crête (358 V à 253 V efficaces). Le MOC3041 tient 400 V en blocage, d'où une marge limitée à protéger (fusible, limitation des surtensions). Les masses GND et PIC ne sont jamais reliées au neutre ou à A1. Sur le PCB, les pistes secteur sont séparées de la partie 5 V par une distance d'isolement de plusieurs millimètres, à déterminer selon la norme : la règle générale de 0,5 mm du projet ne convient pas au secteur."),
  H2("4.6 FP5 — Interface utilisateur"),
  P("Le LCD 162B (DS1) est commandé en mode 4 bits (DB4 à DB7) ; DB0 à DB3 ne sont pas connectés. Son brochage particulier (1 LEDA, 2 LEDK, 3 VSS, 4 VDD, 5 VO, 6 RS, 7 RW, 8 E) a été vérifié sur la documentation du fabricant."),
  puce([B("Contraste : "), r("pont R11 = 56 kΩ, R13 = 1,5 kΩ → VO = 5 × 1,5 / 57,5 ≈ 0,13 V.")]),
  puce([B("Rétroéclairage : "), r("R14 = 270 Ω → I = (5 − 3,2) / 270 ≈ 6,7 mA.")]),
  puce([B("LED D4 : "), r("R10 = 270 Ω → I = (5 − 2) / 270 ≈ 11 mA, compatible avec une sortie du PIC (25 mA max).")]),
  puce([B("Buzzer BZ1 : "), r("commandé par RB1 à travers R9 = 270 Ω. "), AFAIRE("référence, courant, polarité corrigée, transistor éventuel")]),
  H2("4.7 FP6 — Programmation et débogage"),
  P("Le connecteur J4 reçoit le programmateur MPLAB SNAP : 1 MCLR/VPP, 2 +5 V, 3 GND, 4 ICSPDAT (RB7), 5 ICSPCLK (RB6). Le SNAP n'alimente pas la carte : le 5 V est fourni par J1. Le connecteur J2 donne accès à la liaison série TTL (TX broche 5, RX broche 4, GND broche 1) pour afficher les mesures brutes pendant la mise au point."),
  H2("4.8 Circuit imprimé"),
  P("La carte mesure 90 × 80 mm. Le routage est réalisé côté cuivre B.Cu ; deux straps filaires (GND 14 mm et MCLR 10 mm) évitent un circuit double face. Les 21 empreintes de l'ancienne carte absentes du schéma ont été retirées. Règles : piste 0,6 mm (0,7 mm pour +5V et GND), isolement 0,5 mm, plan de masse sur B.Cu."),
  image(path.join(REPO, "04_Maquette_3D/03_carte_kicad.png"), 12, 0.75),
  legende("Figure 3 — Rendu 3D de la carte (implantation actuelle, avant ajout de la partie puissance)"),
];

// ---------- 5. Nomenclature ----------
const nouv = (t) => [r(t, { size: 19 }), r(" (ajout)", { size: 17, color: TEAL, italics: true })];
const nomenclature = [
  H1("5. Nomenclature"),
  P("Les composants sont classés par famille. Les lignes « ajout » proviennent des corrections du chapitre 4 ; leurs repères sont à attribuer dans KiCad. Les tensions de service marquées « … » sont à compléter."),
  H2("Résistances (¼ W, 5 %, sauf mention)"),
  tableau(["Repère", "Valeur", "Rôle", "Empreinte"], [
    ["R7", "150 Ω", "LED du MOC3041", "Axial DIN0207 P10,16"],
    ["R3", "360 Ω, ½ W", "Rg, gâchette du triac (supporte les impulsions)", "Axial DIN0207 P10,16"],
    ["R4", "—", "À supprimer (en série avec le moteur)", "—"],
    ["R9, R10, R14", "270 Ω", "Buzzer, LED D4, rétroéclairage LCD", "Axial DIN0207 P10,16"],
    ["R11", "56 kΩ", "Contraste LCD", "Axial DIN0207 P10,16"],
    ["R12", "10 kΩ", "Rappel de MCLR", "Axial DIN0207 P10,16"],
    ["R13", "1,5 kΩ", "Contraste LCD", "Axial DIN0207 P10,16"],
    [nouv("R_B"), "1 kΩ", "Base du transistor", "Axial DIN0207 P10,16"],
    [nouv("R_BE"), "100 kΩ", "Rappel base-émetteur", "Axial DIN0207 P10,16"],
    [nouv("R_GK"), "330 Ω", "Gâchette – A1 du triac", "Axial DIN0207 P10,16"],
    [nouv("R_SN"), "39 Ω, 1 W", "Snubber", "à définir"],
    [nouv("R_F"), "1 kΩ", "Filtre d'entrée ADC", "Axial DIN0207 P10,16"],
  ], [1.6, 1.6, 4.2, 2.6]),
  H2("Condensateurs"),
  tableau(["Repère", "Valeur", "Tension", "Type", "Empreinte"], [
    ["C1", "47 µF", "… V", "Électrolytique radial", "CP_Radial_D6.3mm_P2.50mm"],
    ["C2, C5", "100 nF", "… V", "Céramique disque", "Disc_D4.7mm_W2.5mm_P5.00mm"],
    [nouv("C_U3"), "100 nF", "… V", "Céramique, découplage ACS756", "Disc_D4.7mm_W2.5mm_P5.00mm"],
    [nouv("C_F"), "10 nF", "… V", "Céramique, filtre ADC", "Disc_D4.7mm_W2.5mm_P5.00mm"],
    [nouv("C_SN"), "10 nF", "275 V AC", "Polypropylène classe X2, snubber", "à définir"],
  ], [1.6, 1.4, 1.4, 3, 3.2]),
  H2("Semi-conducteurs et circuits intégrés"),
  tableau(["Repère", "Désignation", "Boîtier / empreinte"], [
    ["U1", "PIC16F1788-I/SP, microcontrôleur", "DIP-28 W7,62 mm"],
    ["U3", "ACS756 (calibre à confirmer), capteur à effet Hall", "Allegro CB-PFF"],
    ["TRIAC1", "MOC3041, optotriac zéro-crossing (remplace le FODM3011)", "DIP-6 W7,62 mm"],
    ["Triac_de_puissance1", "Triac 600 V, référence à choisir", "TO-220 + dissipateur"],
    [nouv("Q1"), "Transistor NPN petits signaux, référence à choisir", "TO-92"],
    ["D4", "LED rouge 5 mm", "LED_D5.0mm"],
  ], [2.4, 5, 2.6]),
  H2("Connectique, affichage et divers"),
  tableau(["Repère", "Désignation", "Empreinte"], [
    ["J1", "Bornier 2 points, alimentation 5 V", "Bornier 1×02 P5 mm"],
    ["J2", "Barrette 6 broches, liaison série TTL", "PinHeader 1×06 P2,54 mm"],
    ["J4", "Barrette 8 broches, programmation ICSP (SNAP)", "PicKit_4_Snap 1×08 P2,54 mm"],
    [nouv("J_SECT"), "Bornier secteur phase / neutre", "Bornier 1×02 P5 mm, 250 V"],
    [nouv("J_MOT"), "Bornier moteur", "Bornier 1×02 P5 mm, 250 V"],
    [nouv("F1"), "Fusible et porte-fusible, calibre selon la pompe", "à définir"],
    ["DS1", "Afficheur LCD 16×2 Displaytech 162B", "162B"],
    ["BZ1", "Buzzer, référence à choisir", "à définir"],
    ["SW4", "Bouton poussoir Omron B3F-1000 (reset)", "SW_TH_Tactile_Omron_B3F-10xx"],
    ["M1", "Moteur de la pompe (hors carte)", "—"],
    ["—", "Boîtier étanche IP adapté à une cave", "—"],
  ], [2.4, 5, 2.6]),
  vide(),
  P([B("Demande de composants. "), r("Document séparé (annexe A6), sans repères : désignation précise, quantité, fabricant, distributeur, référence distributeur, prix et délai. Classement : résistances puis condensateurs par valeurs croissantes, semi-conducteurs, circuits intégrés, connectique.")]),
];

// ---------- 6. Organigrammes ----------
const organigrammes = [
  H1("6. Organigrammes des programmes"),
  P("Le programme est développé sous MPLAB X avec le compilateur XC8. La configuration du PIC (horloge, ports, ADC, timer, UART) est générée par MCC Melody ; le code applicatif est ajouté dans la structure produite."),
  H2("6.1 Machine d'états"),
  image(path.join(HERE, "fig_etats.png"), 16.5, 1125 / 2507),
  legende("Figure 4 — Machine d'états du programme"),
  tableau(["État", "Rôle", "Sortie de l'état"], [
    ["ARRÊT", "Moteur coupé, affichage de l'état et des compteurs", "3 jours écoulés → AUTO-TEST ; eau détectée → FONCTIONNEMENT"],
    ["AUTO-TEST", "Moteur commandé au plus 60 s, mesure du courant", "Test correct → ARRÊT ; courant hors seuils → DÉFAUT"],
    ["FONCTIONNEMENT", "Pompage réel, surveillance continue", "Fin du pompage → ARRÊT ; courant hors seuils → DÉFAUT"],
    ["DÉFAUT", "Moteur coupé, message et alarme selon la cause", "Acquittement → ARRÊT"],
  ], [2.2, 4, 4.2]),
  H2("6.2 Programme principal"),
  num("Initialiser le PIC : SYSTEM_Initialize() généré par MCC.", "num1"),
  num("Initialiser le LCD en mode 4 bits, afficher l'état ARRÊT.", "num1"),
  num("Mesurer l'offset du capteur moteur arrêté.", "num1"),
  num("Boucle infinie : exécuter l'état courant, mettre à jour l'affichage, gérer les alarmes.", "num1"),
  H2("6.3 Mesure du courant efficace"),
  num("Lancer N conversions de AN0 à cadence fixe sur un nombre entier de périodes de 20 ms.", "num2"),
  num("Pour chaque échantillon : retrancher l'offset, élever au carré, additionner.", "num2"),
  num("Diviser la somme par N, prendre la racine carrée.", "num2"),
  num("Convertir en ampères : division par 20 mV/A (pas de 61 mA).", "num2"),
  H2("6.4 Base de temps et autotest"),
  num("Un timer génère une interruption périodique (bits GIE et PEIE activés).", "num3"),
  num("La routine d'interruption incrémente un compteur de secondes.", "num3"),
  num("À 259 200 s (3 jours) sans fonctionnement, passage en AUTO-TEST.", "num3"),
  num("Pendant le test : ignorer le pic de démarrage (temporisation), comparer le courant à Imin et Imax, arrêter au bout de 60 s.", "num3"),
  H2("6.5 Décision"),
  num("Pendant la temporisation de démarrage : pas de comparaison.", "num4"),
  num("I > Imax : moteur coupé, DÉFAUT « moteur bloqué », LED et buzzer.", "num4"),
  num("I < Imin : moteur coupé, DÉFAUT « pompe à vide », signal différencié.", "num4"),
  num([r("Sinon : fonctionnement autorisé. "), AFAIRE("valeurs de Imin, Imax et de la temporisation")], "num4"),
  P([AFAIRE("dessiner ces organigrammes sous draw.io avec les symboles normalisés")]),
];

// ---------- 7. Conclusion ----------
const conclusion = [
  H1("7. Conclusion"),
  P("À ce stade de la conception, l'architecture de la carte est définie : mesure du courant par effet Hall, traitement par PIC16F1788, commande du moteur par optotriac et triac, interface par LCD, LED et buzzer. Les principaux composants ont été choisis et dimensionnés dans le cas le plus défavorable, en s'appuyant sur des simulations LTspice."),
  P("La carte existante a été adaptée et nettoyée ; son contrôle a fait apparaître des liaisons à corriger, listées au chapitre 4. La configuration du microcontrôleur est prise en charge par MCC et la structure du programme est définie par une machine à quatre états."),
  P("Les principales difficultés ont été la reprise d'une carte conçue pour un autre usage, l'absence des données de la pompe et le blocage du programmateur lors d'une mise à jour. Le planning a été actualisé en conséquence, sans modifier la date de fin."),
  P([B("Reste à faire : "), r("relever la plaque de la pompe et fixer les seuils ; trancher la question du déphasage et du calibre du capteur ; corriger le schéma ; router la partie puissance ; écrire et tester le programme ; établir la demande de composants.")]),
  P([AFAIRE("avancement chiffré par bloc et part de travail de chacun")]),
];

// ---------- Annexes ----------
const annexes = [
  H1("Annexes"),
  P("Chaque annexe est citée dans le rapport. Insérer ici, dans l'ordre :"),
  ...[
    "A1 — Schéma structurel complet (export PDF de KiCad, format A3, avec cartouche)",
    "A2 — Typon et implantation du PCB",
    "A3 — Extraits de fiches techniques : PIC16F1788, ACS756, MOC3041, triac retenu, LCD 162B",
    "A4 — Simulations LTspice (dossier 05_Notes/Etude_MOC3041)",
    "A5 — Diagramme de Gantt actualisé",
    "A6 — Demande de composants",
  ].map((t) => puce(t)),
];

// ---------- Document ----------
const doc = new Document({
  creator: "Groupe 311",
  title: "Compte rendu de conception — vide-cave",
  styles: {
    default: { document: { run: { font: FONT, size: 22 } } },
    paragraphStyles: [
      { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true, run: { font: "Cambria", size: 34, bold: true, color: BLEU }, paragraph: { spacing: { before: 240, after: 200 }, keepNext: true, outlineLevel: 0 } },
      { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true, run: { font: "Cambria", size: 27, bold: true, color: TEAL }, paragraph: { spacing: { before: 280, after: 120 }, keepNext: true, outlineLevel: 1 } },
      { id: "Heading3", name: "Heading 3", basedOn: "Normal", next: "Normal", quickFormat: true, run: { font: FONT, size: 23, bold: true, color: BLEU }, paragraph: { spacing: { before: 180, after: 80 }, keepNext: true, outlineLevel: 2 } },
    ],
  },
  numbering: {
    config: [
      { reference: "puces", levels: [
        { level: 0, format: LevelFormat.BULLET, text: "•", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 540, hanging: 270 } } } },
        { level: 1, format: LevelFormat.BULLET, text: "–", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 1080, hanging: 270 } } } },
      ] },
      ...["num", "num1", "num2", "num3", "num4"].map((ref) => ({ reference: ref, levels: [{ level: 0, format: LevelFormat.DECIMAL, text: "%1.", alignment: AlignmentType.LEFT, style: { paragraph: { indent: { left: 540, hanging: 360 } } } }] })),
    ],
  },
  sections: [
    { properties: { page: { margin: { top: 1134, bottom: 1134, left: 1134, right: 1134 } } }, children: couverture },
    {
      properties: { page: { margin: { top: 1134, bottom: 1134, left: 1134, right: 1134 } } },
      headers: { default: new Header({ children: [new Paragraph({ alignment: AlignmentType.RIGHT, children: [r("Compte rendu de conception — Vide-cave — Groupe 311", { size: 16, color: GRIS })] })] }) },
      footers: { default: new Footer({ children: [new Paragraph({ alignment: AlignmentType.CENTER, children: [new TextRun({ children: [PageNumber.CURRENT], font: FONT, size: 18, color: GRIS })] })] }) },
      children: [...sommaire, ...intro, ...presentation, ...gestion, ...schema, ...nomenclature, ...organigrammes, ...conclusion, ...annexes],
    },
  ],
});

Packer.toBuffer(doc).then((buf) => { fs.writeFileSync(OUT, buf); console.log("écrit :", OUT); });

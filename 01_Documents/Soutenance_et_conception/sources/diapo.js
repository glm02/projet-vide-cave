// Diaporama de soutenance — projet vide-cave (BUT GEII 2, SAé 31/42, groupe 311)
const path = require("path");
const pptxgen = require("pptxgenjs");
const { applyTheme } = require("C:/Users/rafae/.claude/skills/synced/dfb36960-0dc4-44cc-80b9-ff98913aa552_9ccb30b6-6aed-49e5-8b9a-ee19b03d218c/pptx/scripts/apply_theme.js");

const REPO = "C:/Users/rafae/orca/workspaces/Projet vide cave/spiderfish";
const OUT = process.argv[2] || "Soutenance_vide-cave.pptx";
const img = (p) => path.join(REPO, p);

const THEME = {
  name: "Vide-cave GEII",
  headFontFace: "Cambria",
  bodyFontFace: "Calibri",
  colors: {
    dk1: "1B2A33", lt1: "FFFFFF", dk2: "0E3B4A", lt2: "EEF3F5",
    accent1: "0E7C86", accent2: "F2A93B", accent3: "C8473D",
    accent4: "3E8E5E", accent5: "6B7F8C", accent6: "BFDCE2",
    hlink: "0E7C86", folHlink: "6B7F8C",
  },
};
const HEX = THEME.colors;

const pres = new pptxgen();
pres.layout = "LAYOUT_WIDE"; // 13.33 x 7.5
pres.theme = { headFontFace: THEME.headFontFace, bodyFontFace: THEME.bodyFontFace };
pres.title = "Surveillance d'une pompe de vide-cave";
pres.subject = "Soutenance SAé 31/42 — BUT GEII 2";
const C = pres.SchemeColor;

// ---------- Layouts ----------
const footer = { text: "Vide-cave · SAé 31/42 · Groupe 311", options: { x: 0.6, y: 7.0, w: 6, h: 0.3, fontSize: 10, color: C.text2 } };
pres.defineSlideMaster({
  title: "TITRE",
  background: { color: HEX.dk2 },
  objects: [
    { placeholder: { options: { name: "title", type: "title", x: 0.7, y: 1.6, w: 7.2, h: 2.0, fontSize: 40, bold: true, color: C.background1, valign: "bottom" }, text: "" } },
    { placeholder: { options: { name: "body", type: "body", x: 0.7, y: 3.75, w: 7.2, h: 1.0, fontSize: 18, color: C.accent6, valign: "top" }, text: "" } },
  ],
});
pres.defineSlideMaster({
  title: "CONTENU",
  background: { color: HEX.lt1 },
  margin: [0.5, 0.6, 0.6, 0.6],
  objects: [
    { text: footer },
    { placeholder: { options: { name: "title", type: "title", x: 0.6, y: 0.35, w: 12.1, h: 0.85, fontSize: 32, bold: true, color: C.text2, valign: "middle" }, text: "" } },
  ],
  slideNumber: { x: 12.2, y: 7.0, w: 0.5, h: 0.3, fontSize: 10, color: C.text2, align: "right" },
});

// ---------- Aides ----------
const T = (s, text, o) => s.addText(text, Object.assign({ isTextBox: true, fontSize: 15, color: C.text1, valign: "top", margin: 0 }, o));
const card = (s, x, y, w, h, fill, name) =>
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w, h, rectRadius: 0.12, fill: { color: fill || C.background2 }, line: { color: fill || C.background2 }, objectName: name });
const pastille = (s, x, y, label, fill, d) => {
  d = d || 0.55;
  s.addShape(pres.shapes.OVAL, { x, y, w: d, h: d, fill: { color: fill }, line: { color: fill } });
  T(s, label, { x, y, w: d, h: d, align: "center", valign: "middle", bold: true, fontSize: 16, color: C.background1 });
};
const fleche = (s, x1, y1, x2, y2, color) =>
  s.addShape(pres.shapes.LINE, { x: Math.min(x1, x2), y: Math.min(y1, y2), w: Math.abs(x2 - x1) || 0.001, h: Math.abs(y2 - y1) || 0.001, flipH: x2 < x1, flipV: y2 < y1, line: { color: color || HEX.dk2, width: 2, endArrowType: "triangle" } });

// ================= 1. Titre =================
pres.addSection({ title: "Introduction" });
let s = pres.addSlide({ masterName: "TITRE", sectionTitle: "Introduction" });
s.addText("Surveillance et protection d'une pompe de vide-cave", { placeholder: "title" });
s.addText("Mesure du courant moteur, coupure par triac, alarmes et autotest — carte à PIC16F1788", { placeholder: "body" });
T(s, [
  { text: "[Prénom NOM] · [Prénom NOM]", options: { bold: true, breakLine: true } },
  { text: "BUT GEII 2 — SAé 31/42 — Groupe 311", options: { breakLine: true } },
  { text: "Encadrant : [Nom de l'enseignant]", options: { breakLine: true } },
  { text: "IUT Lyon 1 · Département GEII · 2026-2027" },
], { x: 0.7, y: 5.1, w: 7.2, h: 1.5, fontSize: 15, color: C.background1, paraSpaceAfter: 4 });
s.addImage({ path: img("04_Maquette_3D/01_installation.png"), x: 8.3, y: 1.35, w: 4.5, h: 3.375, rounding: false, altText: "Maquette 3D : pompe dans son puisard et coffret de commande" });
T(s, "Maquette 3D de l'installation (Blender)", { x: 8.3, y: 4.8, w: 4.5, h: 0.3, fontSize: 11, color: C.accent6, italic: true });
s.addNotes("ORATEUR A (30 s). Bonjour, nous sommes [A] et [B]. Notre projet : surveiller la pompe d'un vide-cave pour détecter ses pannes avant qu'on en ait besoin. Je présente le contexte, le cahier des charges et l'architecture ; [B] détaillera la réalisation.");

// ================= 2. Contexte =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Introduction" });
s.addText("Une pompe qui dort peut tomber en panne sans prévenir", { placeholder: "title" });
const ctx = [
  ["1", "Une pompe vide-cave évacue l'eau d'infiltration, puis reste à l'arrêt parfois pendant des mois."],
  ["2", "Blocage, grippage, pompe à sec, défaut électrique : la panne reste invisible."],
  ["3", "On la découvre le jour de l'inondation, au pire moment."],
];
ctx.forEach((r, i) => {
  const y = 1.6 + i * 1.35;
  pastille(s, 0.7, y, r[0], i === 2 ? C.accent3 : C.accent1);
  T(s, r[1], { x: 1.5, y: y - 0.02, w: 5.6, h: 1.0, fontSize: 17 });
});
card(s, 0.7, 5.6, 6.4, 0.95, C.background2, "objectif");
T(s, [{ text: "Objectif : ", options: { bold: true, color: C.accent1 } }, { text: "vérifier automatiquement la pompe et réagir seul en cas de défaut." }], { x: 0.95, y: 5.72, w: 6.0, h: 0.75, fontSize: 16, valign: "middle" });
s.addImage({ path: img("04_Maquette_3D/04_interieur_moteur_pompe.png"), x: 7.6, y: 1.5, w: 5.1, h: 3.825, altText: "Vue intérieure du moteur de la pompe" });
T(s, "Moteur asynchrone monophasé 230 V de la pompe (maquette)", { x: 7.6, y: 5.4, w: 5.1, h: 0.3, fontSize: 11, color: C.text2, italic: true });
s.addNotes("ORATEUR A (1 min). Expliquer le problème : fonctionnement intermittent, panne invisible. Insister sur la conséquence : la panne est découverte au pire moment. D'où l'idée : faire surveiller la pompe par une carte électronique.");

// ================= 3. Cahier des charges =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Introduction" });
s.addText("Le courant du moteur révèle son état", { placeholder: "title" });
const cas = [
  ["Courant trop fort", "Moteur bloqué ou grippé", "Arrêt + alarme", C.accent3],
  ["Courant normal", "La pompe évacue l'eau", "Fonctionnement autorisé", C.accent4],
  ["Courant trop faible", "Pompe à vide, sans eau", "Arrêt + information", C.accent2],
];
cas.forEach((c, i) => {
  const x = 0.6 + i * 4.1;
  card(s, x, 1.5, 3.8, 2.4, C.background2, "cas" + i);
  s.addShape(pres.shapes.OVAL, { x: x + 0.3, y: 1.75, w: 0.4, h: 0.4, fill: { color: c[3] }, line: { color: c[3] } });
  T(s, c[0], { x: x + 0.85, y: 1.75, w: 2.8, h: 0.45, fontSize: 19, bold: true, valign: "middle" });
  T(s, c[1], { x: x + 0.3, y: 2.45, w: 3.3, h: 0.6, fontSize: 15 });
  T(s, c[2], { x: x + 0.3, y: 3.15, w: 3.3, h: 0.5, fontSize: 15, bold: true, color: c[3] });
});
const chiffres = [["3 jours", "autotest automatique"], ["≤ 60 s", "durée d'un autotest"], ["230 V", "pompe secteur, à isoler"], ["LCD · LED · buzzer", "information de l'utilisateur"]];
chiffres.forEach((c, i) => {
  const x = 0.6 + i * 3.1;
  T(s, c[0], { x, y: 4.45, w: 2.9, h: 0.8, fontSize: i === 3 ? 22 : 34, bold: true, color: C.accent1, valign: "bottom", fontFace: "Cambria" });
  T(s, c[1], { x, y: 5.3, w: 2.9, h: 0.5, fontSize: 14, color: C.text2 });
});
T(s, "Hors périmètre : la pompe, le flotteur de niveau, le Wi-Fi (évolution possible).", { x: 0.6, y: 6.1, w: 12, h: 0.4, fontSize: 13, italic: true, color: C.text2 });
s.addNotes("ORATEUR A (1 min). Le principe de détection repose sur le courant absorbé : il dépend de la charge mécanique du moteur. Trois cas. Le cahier des charges impose un autotest tous les 3 jours, d'au plus une minute, et une information locale par LCD, LED et buzzer. Préciser ce qui est hors périmètre.");

// ================= 4. Synoptique =================
pres.addSection({ title: "Architecture et choix" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Architecture et choix" });
s.addText("Six fonctions, séparées par une barrière d'isolement", { placeholder: "title" });
// zone secteur
s.addShape(pres.shapes.RECTANGLE, { x: 0.6, y: 1.45, w: 4.3, h: 5.2, fill: { color: "FBEDEC" }, line: { color: HEX.accent3, width: 1 }, objectName: "zone-secteur" });
T(s, "Côté secteur 230 V", { x: 0.8, y: 1.55, w: 3.9, h: 0.35, fontSize: 13, bold: true, color: C.accent3 });
s.addShape(pres.shapes.RECTANGLE, { x: 5.6, y: 1.45, w: 7.1, h: 5.2, fill: { color: HEX.lt2 }, line: { color: HEX.accent1, width: 1 }, objectName: "zone-5V" });
T(s, "Côté basse tension 5 V", { x: 5.8, y: 1.55, w: 4, h: 0.35, fontSize: 13, bold: true, color: C.accent1 });
s.addShape(pres.shapes.LINE, { x: 5.25, y: 1.45, w: 0.001, h: 5.2, line: { color: HEX.accent3, width: 2.5, dashType: "dash" } });
T(s, "isolement", { x: 4.85, y: 6.68, w: 0.8, h: 0.25, fontSize: 10, color: C.accent3, align: "center" });
const bloc = (x, y, w, h, titre, sous, fill) => {
  card(s, x, y, w, h, fill || C.background1, titre);
  T(s, [{ text: titre, options: { bold: true, breakLine: true } }, { text: sous, options: { fontSize: 12, color: C.text2 } }], { x: x + 0.12, y: y + 0.08, w: w - 0.24, h: h - 0.16, fontSize: 14, valign: "middle", align: "center" });
};
bloc(0.9, 2.1, 1.7, 0.95, "Secteur", "phase / neutre, fusible");
bloc(0.9, 4.0, 1.7, 0.95, "FP4 Triac", "commutation pompe");
bloc(3.0, 5.4, 1.7, 0.95, "Moteur", "pompe 230 V");
bloc(3.0, 2.1, 1.7, 0.95, "FP2 ACS756", "capteur à effet Hall");
bloc(5.95, 4.0, 1.9, 0.95, "FP4 MOC3041", "optotriac + transistor");
bloc(5.95, 5.4, 1.9, 0.95, "FP1 Alim 5 V", "bornier J1, découplage");
bloc(8.4, 3.0, 2.0, 1.4, "FP3 PIC16F1788", "ADC 12 bits, timers, machine d'états", C.accent6);
bloc(10.75, 2.1, 1.75, 0.95, "FP5 IHM", "LCD 16×2, LED, buzzer");
bloc(10.75, 4.0, 1.75, 0.95, "FP6 Prog/debug", "ICSP SNAP, UART");
fleche(s, 1.75, 3.05, 1.75, 4.0);           // secteur -> triac
fleche(s, 2.6, 4.75, 3.0, 5.6);             // triac -> moteur
fleche(s, 3.85, 5.4, 3.85, 3.05);           // moteur courant -> ACS
fleche(s, 4.7, 2.575, 8.4, 3.4, HEX.accent1); // ACS -> PIC (V_CAPTEUR)
T(s, "V_CAPTEUR → AN0", { x: 6.0, y: 2.55, w: 2.0, h: 0.3, fontSize: 11, color: C.accent1, bold: true });
fleche(s, 8.4, 4.0, 7.85, 4.45, HEX.accent1); // PIC -> MOC
T(s, "CMD_OPTO (RB2)", { x: 5.95, y: 4.98, w: 1.9, h: 0.3, align: "center", fontSize: 11, color: C.accent1, bold: true });
fleche(s, 5.95, 4.475, 2.6, 4.475, HEX.accent3); // MOC -> triac (gâchette)
T(s, "gâchette", { x: 4.0, y: 4.15, w: 1.1, h: 0.3, fontSize: 11, color: C.accent3, bold: true });
fleche(s, 10.4, 3.45, 10.75, 2.6);
fleche(s, 10.75, 4.45, 10.4, 3.95);
fleche(s, 7.85, 5.85, 8.4, 4.4);
s.addNotes("ORATEUR A (1 min). Lire de gauche à droite : le courant du moteur traverse le capteur à effet Hall, dont la tension image arrive sur l'entrée analogique AN0 du PIC. Le PIC décide et commande, par RB2, l'optotriac qui déclenche le triac de puissance. Point clé : la barrière d'isolement. Le capteur Hall et l'optotriac sont les deux seuls éléments qui la traversent, sans liaison électrique.");

// ================= 5. Choix =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Architecture et choix" });
s.addText("Ce qui a changé depuis la pré-étude, et pourquoi", { placeholder: "title" });
const H = (t) => ({ text: t, options: { bold: true, color: HEX.lt1, fill: { color: HEX.dk2 } } });
const rows = [
  [H("Fonction"), H("Pré-étude"), H("Conception"), H("Raison du choix")],
  ["Traitement", "ESP32", { text: "PIC16F1788", options: { bold: true } }, "Vu en cours (MPLAB X, MCC, XC8) ; ADC 12 bits ; Wi-Fi reporté en évolution"],
  ["Coupure", "Relais", { text: "MOC3041 + triac", options: { bold: true } }, "Pas d'usure, silencieux, commutation au zéro de tension, isolement par l'optotriac"],
  ["Mesure I", "Hall / TC / shunt", { text: "ACS756 (Hall)", options: { bold: true } }, "Isolement intrinsèque, sortie directement lisible par l'ADC"],
  ["Déphasage", "Prévu", { text: "À décider", options: { bold: true, color: HEX.accent3 } }, "Demande une voie tension isolée (détecteur de zéro) absente de la carte"],
  ["Carte", "À concevoir", { text: "Carte G206 adaptée", options: { bold: true } }, "Base existante : alimentation, PIC, LCD, ICSP déjà routés"],
];
s.addTable(rows, { x: 0.6, y: 1.5, w: 12.1, colW: [1.7, 2.0, 2.5, 5.9], fontSize: 14, color: HEX.dk1, border: { type: "solid", pt: 0.75, color: "C9D6DB" }, fill: { color: HEX.lt1 }, rowH: 0.62, valign: "middle", margin: 0.08 });
s.addNotes("ORATEUR A (1 min). Le jury connaît notre pré-étude : il faut justifier chaque changement. Le passage au PIC, le triac au lieu du relais. Pour le déphasage, être honnête : il nécessite une voie de mesure de la tension, pas encore présente. [Dire ici la décision prise : ajout d'un détecteur de zéro isolé OU retrait du périmètre.] Transition : « Je laisse [B] détailler la réalisation. »");

// ================= 6. Mesure du courant =================
pres.addSection({ title: "Réalisation" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Réalisation" });
s.addText("Mesurer le courant sans contact électrique avec le secteur", { placeholder: "title" });
const chaine = [["Courant moteur", "i(t), 50 Hz"], ["ACS756", "effet Hall"], ["Filtre RC", "1 kΩ / 10 nF"], ["ADC 12 bits", "entrée AN0"], ["Max − min", "sur 20 ms"]];
chaine.forEach((c, i) => {
  const x = 0.6 + i * 2.48;
  card(s, x, 1.55, 2.05, 1.05, i === 1 ? C.accent6 : C.background2, "chaine" + i);
  T(s, [{ text: c[0], options: { bold: true, breakLine: true } }, { text: c[1], options: { fontSize: 12, color: C.text2 } }], { x: x + 0.1, y: 1.6, w: 1.85, h: 0.95, fontSize: 15, align: "center", valign: "middle" });
  if (i < 4) fleche(s, x + 2.05, 2.075, x + 2.48, 2.075);
});
const stats = [["20 mV/A", "sensibilité du capteur"], ["2,5 V", "sortie à courant nul"], ["1,22 mV", "quantum q = 5 V / 4096 (61 mA)"], ["15,9 kHz", "coupure du filtre RC"]];
stats.forEach((c, i) => {
  const x = 0.6 + i * 3.1;
  T(s, c[0], { x, y: 3.0, w: 2.9, h: 0.75, fontSize: 32, bold: true, color: C.accent1, fontFace: "Cambria", valign: "bottom" });
  T(s, c[1], { x, y: 3.8, w: 2.9, h: 0.45, fontSize: 14, color: C.text2 });
});
card(s, 0.6, 4.6, 6.6, 1.9, C.background2, "rms");
T(s, [
  { text: "Le PIC travaille en points du CAN", options: { bold: true, breakLine: true } },
  { text: "Sur une période de 20 ms, il garde la valeur max et la valeur min lues sur AN0. Amplitude = max − min, comparée à SEUIL_HAUT et SEUIL_BAS en points. Pas de float." },
], { x: 0.85, y: 4.75, w: 6.1, h: 1.6, fontSize: 14, paraSpaceAfter: 4 });
card(s, 7.5, 4.6, 5.2, 1.9, "FDF1DD", "limite");
T(s, [
  { text: "Limite identifiée", options: { bold: true, color: HEX.accent3, breakLine: true } },
  { text: "Calibre ±100 A pour une pompe de quelques ampères (≈ 1,3 A en négligeant le cos φ) : ~21 points du CAN seulement. Pistes : capteur de plus petit calibre ou gain ×5 centré sur 2,5 V." },
], { x: 7.75, y: 4.75, w: 4.7, h: 1.6, fontSize: 14, paraSpaceAfter: 4 });
s.addNotes("ORATEUR B (1 min). Le capteur à effet Hall mesure le champ créé par le courant : pas de contact électrique avec le secteur. Sortie 2,5 V au repos plus 20 mV par ampère. Le filtre RC coupe à 15,9 kHz, il ne déforme pas le 50 Hz. Le PIC travaille en points du CAN : quantum 1,22 mV. Il garde le max et le min sur 20 ms ; l'amplitude max − min est comparée aux seuils, sans float. Montrer qu'on a vu la limite : le calibre 100 A est trop large pour notre pompe ; dire la solution retenue. (Courant de 1,3 A = hypothèse 300 W / 230 V, plaque à relever.)");

// ================= 7. Commande de puissance =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Réalisation" });
s.addText("Commander la pompe : optotriac MOC3041 et triac", { placeholder: "title" });
const cmd = ["PIC RB2", "Transistor", "LED MOC3041", "Triac", "Moteur"];
cmd.forEach((c, i) => {
  const x = 0.6 + i * 1.32;
  card(s, x, 1.5, 1.12, 0.75, i === 2 ? C.accent6 : C.background2, "cmd" + i);
  T(s, c, { x: x + 0.04, y: 1.5, w: 1.04, h: 0.75, fontSize: 12, bold: true, align: "center", valign: "middle" });
  if (i < 4) fleche(s, x + 1.12, 1.875, x + 1.32, 1.875);
});
card(s, 0.6, 2.6, 6.4, 1.85, C.background2, "calcul");
T(s, [
  { text: "Résistance de la LED : 150 Ω", options: { bold: true, fontSize: 18, color: HEX.accent1, breakLine: true } },
  { text: "Le MOC3041 exige au plus 15 mA pour s'amorcer (IFT max).", options: { breakLine: true } },
  { text: "Cas défavorable : 5 V − 5 %, LED 1,5 V, transistor 0,2 V, R + 5 %", options: { breakLine: true } },
  { text: "I = (4,75 − 1,5 − 0,2) / (1,05 × 150) ≈ 19,4 mA > 15 mA", options: { bold: true } },
], { x: 0.85, y: 2.75, w: 6.0, h: 1.6, fontSize: 14, paraSpaceAfter: 5 });
T(s, [
  { text: "Côté secteur : ", options: { bold: true } },
  { text: "Rg 360 Ω, Rgk 330 Ω et snubber 39 Ω + 10 nF (exemple fabricant), à valider avec le triac réel. Commutation au zéro de tension : peu de parasites." },
], { x: 0.6, y: 4.75, w: 6.4, h: 1.3, fontSize: 14 });
s.addChart(pres.charts.BAR, [{ name: "Courant LED minimal (mA)", labels: ["150 Ω", "180 Ω", "220 Ω", "270 Ω"], values: [19.37, 16.14, 13.2, 10.76] }], {
  x: 7.4, y: 1.45, w: 5.3, h: 4.6, barDir: "col",
  showTitle: true, title: "Courant LED dans le pire cas (LTspice)", titleFontSize: 14, titleColor: HEX.dk2, titleFontFace: "+mn-lt",
  showValue: true, dataLabelPosition: "outEnd", dataLabelFormatCode: "0.0", dataLabelFontSize: 12, dataLabelColor: HEX.dk1, dataLabelFontFace: "+mn-lt",
  chartColors: [HEX.accent1, HEX.accent5, HEX.accent2, HEX.accent3], invertedColors: [HEX.accent1, HEX.accent5, HEX.accent2, HEX.accent3],
  catAxisLabelColor: HEX.dk1, valAxisLabelColor: HEX.accent5, catAxisLabelFontSize: 13, valAxisLabelFontSize: 11, catAxisLabelFontFace: "+mn-lt", valAxisLabelFontFace: "+mn-lt",
  valAxisMinVal: 0, valAxisMaxVal: 25, valGridLine: { color: "DDE5E8", size: 0.75 }, catGridLine: { style: "none" }, showLegend: false,
  objectName: "graph-rled",
});
T(s, "Seuil d'amorçage : 15 mA — 220 Ω et 270 Ω ne garantissent pas l'amorçage.", { x: 7.4, y: 6.15, w: 5.3, h: 0.5, fontSize: 12, italic: true, color: C.accent3 });
s.addNotes("ORATEUR B (1 min). Le PIC ne commande pas directement la LED : un transistor fournit les 20 mA et garde l'optotriac bloqué au reset. Calcul de la résistance dans le pire cas, vérifié sous LTspice : 150 Ω garantit l'amorçage, l'ancienne valeur de 270 Ω non. Côté secteur, valeurs de départ de la fiche constructeur, à confirmer quand le triac sera choisi.");

// ================= 8. Carte =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Réalisation" });
s.addText("Une carte existante adaptée et nettoyée", { placeholder: "title" });
s.addImage({ path: img("04_Maquette_3D/03_carte_kicad.png"), x: 0.6, y: 1.45, w: 6.4, h: 4.8, altText: "Rendu 3D de la carte KiCad G206" });
const faits = [["90 × 80 mm", "carte simple face, routage côté cuivre B.Cu"], ["21", "empreintes de l'ancienne carte retirées du PCB"], ["2 straps", "GND 14 mm et MCLR 10 mm au lieu d'un double face"]];
faits.forEach((f, i) => {
  const y = 1.5 + i * 1.05;
  T(s, f[0], { x: 7.4, y, w: 2.3, h: 0.8, fontSize: 26, bold: true, color: C.accent1, fontFace: "Cambria", valign: "middle" });
  T(s, f[1], { x: 9.75, y, w: 3.0, h: 0.8, fontSize: 14, valign: "middle" });
});
card(s, 7.4, 4.75, 5.3, 1.2, "FDF1DD", "a-corriger");
T(s, [
  { text: "À corriger au schéma", options: { bold: true, color: HEX.accent3, breakLine: true } },
  { text: "liaisons capteur → AN0 et RB2 → optotriac, primaire du capteur, bornier secteur, MOC3041 6 broches, polarité du buzzer." },
], { x: 7.6, y: 4.85, w: 4.95, h: 1.0, fontSize: 13, paraSpaceAfter: 3 });
s.addNotes("ORATEUR B (45 s). Nous partons d'une carte existante pour gagner du temps. Nous avons retiré les 21 empreintes inutiles, gardé un routage simple face fabriquable à l'IUT, avec deux straps. Le contrôle a mis en évidence des liaisons à corriger : les citer, et dire où on en est.");

// ================= 9. Logiciel =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Réalisation" });
s.addText("Le programme : une machine à quatre états", { placeholder: "title" });
const etat = (x, y, nom, fill, sous) => {
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w: 2.5, h: 1.0, rectRadius: 0.5, fill: { color: fill }, line: { color: fill }, objectName: nom });
  T(s, [{ text: nom, options: { bold: true, breakLine: true } }, { text: sous, options: { fontSize: 11 } }], { x, y, w: 2.5, h: 1.0, align: "center", valign: "middle", fontSize: 16, color: C.background1 });
};
etat(0.7, 3.2, "ARRÊT", HEX.accent5, "attente, affichage");
etat(4.1, 1.6, "AUTO-TEST", HEX.accent1, "tous les 3 jours, ≤ 60 s");
etat(4.1, 4.8, "FONCTIONNEMENT", HEX.accent4, "pompage réel");
etat(7.5, 3.2, "DÉFAUT", HEX.accent3, "bloqué ou à vide");
fleche(s, 2.2, 3.2, 4.1, 2.2); T(s, "Heures = 72", { x: 1.6, y: 2.25, w: 2.0, h: 0.3, fontSize: 11, color: C.text2 });
fleche(s, 2.2, 4.2, 4.1, 5.2); T(s, "eau détectée", { x: 1.8, y: 4.95, w: 1.8, h: 0.3, fontSize: 11, color: C.text2 });
fleche(s, 6.6, 2.2, 7.9, 3.2); T(s, "hors seuils", { x: 7.0, y: 2.3, w: 1.6, h: 0.3, fontSize: 11, color: C.accent3 });
fleche(s, 6.6, 5.2, 7.9, 4.2); T(s, "hors seuils", { x: 7.0, y: 4.9, w: 1.6, h: 0.3, fontSize: 11, color: C.accent3 });
fleche(s, 4.1, 2.45, 2.6, 3.2); T(s, "test OK", { x: 3.3, y: 3.0, w: 1.2, h: 0.3, fontSize: 11, color: C.accent4 });
card(s, 10.4, 1.5, 2.3, 3.7, C.background2, "decision");
T(s, [
  { text: "Décision", options: { bold: true, breakLine: true } },
  { text: "Temporisation au démarrage (pic de courant normal)", options: { bullet: true, breakLine: true } },
  { text: "amplitude > SEUIL_HAUT : bloqué", options: { bullet: true, breakLine: true } },
  { text: "amplitude < SEUIL_BAS : à vide", options: { bullet: true, breakLine: true } },
  { text: "IT 10 ms : Nb_10ms + 1, lu en section critique", options: { bullet: true, breakLine: true } },
  { text: "Heures = 72 → autotest (uint8_t)", options: { bullet: true } },
], { x: 10.55, y: 1.65, w: 2.05, h: 3.45, fontSize: 13, paraSpaceAfter: 6 });
T(s, "Codé avec switch(etat) ; broches, CAN et Timer2 configurés par MCC (MPLAB X, XC8).", { x: 0.7, y: 6.15, w: 9.5, h: 0.35, fontSize: 13, italic: true, color: C.text2 });
s.addNotes("ORATEUR B (45 s). Le logiciel est une machine à états. Le Timer2 lève une IT toutes les 10 ms ; la routine d'IT ajoute seulement 1 à Nb_10ms ; le main le recopie en section critique (GIE = 0 le temps de la copie) et compte secondes, minutes, heures en uint8_t. Un simple flag perdrait des pas pendant les 20 ms de mesure. Depuis ARRÊT, deux déclencheurs : Heures = 72 lance l'autotest, l'arrivée d'eau lance le pompage. Dans les deux cas, si le courant sort des seuils, on passe en DÉFAUT : moteur coupé, message et alarme différents selon la cause. On ignore le pic de démarrage. Dire honnêtement l'avancement du code.");

// ================= 10. Avancement =================
pres.addSection({ title: "Bilan" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Bilan" });
s.addText("Avancement et plan de rattrapage", { placeholder: "title" });
const av = [["Cahier des charges, pré-étude", 100], ["Choix des composants", 60], ["Schéma structurel", 40], ["PCB", 30], ["Programme PIC", 15], ["Essais", 0]];
av.forEach((a, i) => {
  const y = 1.55 + i * 0.68;
  T(s, a[0], { x: 0.6, y, w: 3.2, h: 0.45, fontSize: 14, valign: "middle" });
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x: 3.9, y: y + 0.1, w: 3.5, h: 0.26, rectRadius: 0.13, fill: { color: "DDE5E8" }, line: { color: "DDE5E8" } });
  if (a[1] > 0) s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x: 3.9, y: y + 0.1, w: 3.5 * a[1] / 100, h: 0.26, rectRadius: 0.13, fill: { color: a[1] >= 50 ? HEX.accent4 : HEX.accent2 }, line: { color: a[1] >= 50 ? HEX.accent4 : HEX.accent2 } });
  T(s, a[1] + " %", { x: 7.5, y, w: 0.8, h: 0.45, fontSize: 14, bold: true, valign: "middle" });
});
T(s, "Estimation au 4 novembre 2026, à ajuster.", { x: 0.6, y: 5.75, w: 7.5, h: 0.3, fontSize: 11, italic: true, color: C.text2 });
card(s, 8.7, 1.5, 4.0, 4.8, C.background2, "rattrapage");
T(s, [
  { text: "Retards", options: { bold: true, color: HEX.accent3, breakLine: true } },
  { text: "Carte héritée avec liaisons rompues", options: { bullet: true, breakLine: true } },
  { text: "Données de la pompe manquantes", options: { bullet: true, breakLine: true } },
  { text: "Programmateur SNAP bloqué (12/10)", options: { bullet: true, breakLine: true } },
  { text: "Rattrapage", options: { bold: true, color: HEX.accent4, breakLine: true } },
  { text: "Schéma/PCB et programme menés en parallèle", options: { bullet: true, breakLine: true } },
  { text: "Tests basse tension avant le secteur", options: { bullet: true, breakLine: true } },
  { text: "Dépôt Git commun, synchronisation en un clic", options: { bullet: true } },
], { x: 8.95, y: 1.65, w: 3.55, h: 4.5, fontSize: 13, paraSpaceAfter: 5 });
s.addNotes("ORATEUR A (45 s). Montrer le planning actualisé (Gantt) si possible. Donner les retards sans se cacher et surtout les moyens de rattrapage. La date de fin ne change pas. Ajuster les pourcentages le jour J.");

// ================= 11. Conclusion =================
s = pres.addSlide({ masterName: "TITRE", sectionTitle: "Bilan" });
s.addText("Conclusion", { placeholder: "title" });
s.addText("Une surveillance par le courant, isolée du secteur, qui teste la pompe avant qu'on en ait besoin", { placeholder: "body" });
const concl = [
  ["Acquis techniques", "Mesure par effet Hall, commande isolée par optotriac, calculs pire cas, simulation LTspice, KiCad, PIC et MCC"],
  ["Méthodes", "Cahier des charges, gestion des versions avec Git, notes de suivi, décisions tracées"],
  ["Part de chacun", "[A] : … · [B] : … (à compléter)"],
];
concl.forEach((c, i) => {
  const y = 5.0 + i * 0.68;
  T(s, c[0], { x: 0.7, y, w: 2.6, h: 0.6, fontSize: 15, bold: true, color: C.accent2, valign: "middle" });
  T(s, c[1], { x: 3.3, y, w: 9.3, h: 0.6, fontSize: 14, color: C.background1, valign: "middle" });
});
s.addImage({ path: img("04_Maquette_3D/02_coffret_eclate.png"), x: 8.6, y: 0.7, w: 4.2, h: 3.15, altText: "Vue éclatée du coffret de commande" });
s.addNotes("ORATEURS A et B (45 s). Chacun dit ce qu'il a fait et ce qu'il a appris. Terminer par : « Merci, nous sommes prêts pour vos questions. »");

(async () => {
  await pres.writeFile({ fileName: OUT });
  await applyTheme(OUT, THEME);
  console.log("écrit :", OUT);
})();

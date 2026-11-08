// Diaporama de soutenance — projet vide-cave (BUT GEII 2, SAé 31/42, groupe 311)
// Version alignée sur le vrai projet : composants du schéma (ACS756, BTA12, MOC3023,
// 6N136, PIC16F1788) et code réel de main_simple.c (5 états, boucle de 100 ms, CAN lu sur 8 bits).
const path = require("path");
const pptxgen = require("pptxgenjs");
const { applyTheme } = require("C:/Users/rafae/.claude/skills/synced/dfb36960-0dc4-44cc-80b9-ff98913aa552_9ccb30b6-6aed-49e5-8b9a-ee19b03d218c/pptx/scripts/apply_theme.js");

const REPO = "C:/Users/rafae/orca/workspaces/Projet vide cave/spiderfish";
const OUT = process.argv[2] || "Soutenance_vide-cave.pptx";
const img = (p) => path.join(REPO, p);
const comp = (n) => path.join(__dirname, "img_composants", n);

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
const CODE_BG = "16262F";
const ALERTE_BG = "FDF1DD";

const pres = new pptxgen();
pres.layout = "LAYOUT_WIDE"; // 13.33 x 7.5
pres.theme = { headFontFace: THEME.headFontFace, bodyFontFace: THEME.bodyFontFace };
pres.title = "Surveillance d'une pompe de vide-cave";
pres.subject = "Soutenance SAé 31/42 — BUT GEII 2";
const C = pres.SchemeColor;

// ---------- Layouts ----------
pres.defineSlideMaster({
  title: "TITRE",
  background: { color: HEX.dk2 },
  objects: [
    { placeholder: { options: { name: "title", type: "title", x: 0.7, y: 1.3, w: 7.2, h: 2.2, fontSize: 40, bold: true, color: C.background1, valign: "bottom", align: "left" }, text: "" } },
    { placeholder: { options: { name: "body", type: "body", x: 0.7, y: 3.65, w: 7.2, h: 1.0, fontSize: 18, color: C.accent6, valign: "top" }, text: "" } },
  ],
});
pres.defineSlideMaster({
  title: "CONTENU",
  background: { color: HEX.lt1 },
  margin: [0.5, 0.6, 0.6, 0.6],
  objects: [
    { text: { text: "Vide-cave · SAé 31/42 · Groupe 311", options: { x: 0.6, y: 7.0, w: 6, h: 0.3, fontSize: 10, color: C.text2 } } },
    { placeholder: { options: { name: "title", type: "title", x: 0.6, y: 0.35, w: 12.1, h: 0.85, fontSize: 30, bold: true, color: C.text2, valign: "middle", align: "left" }, text: "" } },
  ],
  slideNumber: { x: 12.2, y: 7.0, w: 0.5, h: 0.3, fontSize: 10, color: C.text2, align: "right" },
});

// ---------- Aides ----------
const T = (s, text, o) => s.addText(text, Object.assign({ isTextBox: true, fontSize: 15, color: C.text1, valign: "top", margin: 0 }, o));
const card = (s, x, y, w, h, fill, name) =>
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w, h, rectRadius: 0.1, fill: { color: fill || C.background2 }, line: { color: fill || C.background2 }, objectName: name });
const pastille = (s, x, y, label, fill, d) => {
  d = d || 0.55;
  s.addShape(pres.shapes.OVAL, { x, y, w: d, h: d, fill: { color: fill }, line: { color: fill } });
  T(s, label, { x, y, w: d, h: d, align: "center", valign: "middle", bold: true, fontSize: 16, color: C.background1 });
};
const fleche = (s, x1, y1, x2, y2, color) =>
  s.addShape(pres.shapes.LINE, { x: Math.min(x1, x2), y: Math.min(y1, y2), w: Math.abs(x2 - x1) || 0.001, h: Math.abs(y2 - y1) || 0.001, flipH: x2 < x1, flipV: y2 < y1, line: { color: color || HEX.dk2, width: 2, endArrowType: "triangle" } });
const code = (s, x, y, w, h, lignes, taille) => {
  card(s, x, y, w, h, CODE_BG, "code");
  const runs = lignes.map((l, i) => ({
    text: l.length ? l : " ",
    options: { color: l.trim().startsWith("//") ? "8FD19E" : "E8F1F4", breakLine: i < lignes.length - 1 },
  }));
  T(s, runs, { x: x + 0.2, y: y + 0.14, w: w - 0.35, h: h - 0.28, fontFace: "Courier New", fontSize: taille || 13 });
};
const H = (t) => ({ text: t, options: { bold: true, color: HEX.lt1, fill: { color: HEX.dk2 } } });

// ================= 1. Titre =================
pres.addSection({ title: "Introduction" });
let s = pres.addSlide({ masterName: "TITRE", sectionTitle: "Introduction" });
s.addText("Surveillance d'une pompe de vide-cave", { placeholder: "title" });
s.addText("Connaître l'état du moteur en mesurant son courant : carte à PIC16F1788, triac, capteur à effet Hall", { placeholder: "body" });
T(s, [
  { text: "[Prénom NOM] · [Prénom NOM]", options: { bold: true, breakLine: true } },
  { text: "BUT GEII 2 — SAé 31/42 — Groupe 311", options: { breakLine: true } },
  { text: "Encadrant : [Nom de l'enseignant]", options: { breakLine: true } },
  { text: "IUT Lyon 1 · Département GEII · 2026-2027" },
], { x: 0.7, y: 5.0, w: 7.2, h: 1.5, fontSize: 15, color: C.background1, paraSpaceAfter: 4 });
s.addImage({ path: img("04_Maquette_3D/01_installation.png"), x: 8.3, y: 1.35, w: 4.5, h: 3.375, altText: "Maquette 3D : pompe dans son puisard et coffret de commande" });
T(s, "Maquette 3D de l'installation (Blender)", { x: 8.3, y: 4.8, w: 4.5, h: 0.3, fontSize: 11, color: C.accent6, italic: true });
s.addNotes("30 s. Bonjour, nous sommes [prénom] et [prénom]. Notre projet, c'est une carte qui surveille la pompe d'un vide-cave en mesurant le courant de son moteur. C'est une soutenance intermédiaire : je vais présenter le besoin, les composants qu'on a choisis, le code qu'on a déjà écrit, et ce qu'il nous reste à faire.");

// ================= 2. Contexte =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Introduction" });
s.addText("Une pompe en cave peut tomber en panne sans qu'on le voie", { placeholder: "title" });
[
  ["1", "La pompe vide l'eau de la cave, puis reste à l'arrêt parfois pendant des mois.", C.accent1],
  ["2", "Elle est dans une cave, pas facile d'accès : personne ne va vérifier qu'elle marche.", C.accent1],
  ["3", "Si elle est bloquée, on ne le découvre que le jour de l'inondation.", C.accent3],
].forEach((r, i) => {
  const y = 1.6 + i * 1.3;
  pastille(s, 0.7, y, r[0], r[2]);
  T(s, r[1], { x: 1.5, y: y - 0.02, w: 5.6, h: 1.0, fontSize: 17 });
});
card(s, 0.7, 5.5, 6.4, 1.05, C.background2, "objectif");
T(s, [{ text: "Objectif : ", options: { bold: true, color: C.accent1 } }, { text: "connaître l'état du moteur sans descendre à la cave, juste en mesurant son courant." }], { x: 0.95, y: 5.6, w: 5.95, h: 0.85, fontSize: 16, valign: "middle" });
s.addImage({ path: img("04_Maquette_3D/04_interieur_moteur_pompe.png"), x: 7.6, y: 1.5, w: 5.1, h: 3.825, altText: "Vue intérieure du moteur de la pompe" });
T(s, "Moteur asynchrone monophasé 230 V (maquette 3D)", { x: 7.6, y: 5.4, w: 5.1, h: 0.3, fontSize: 11, color: C.text2, italic: true });
s.addNotes("1 min. Le problème : la pompe est dans une cave, on ne va pas vérifier tous les jours si elle marche. Si elle est bloquée, on s'en rend compte quand la cave est inondée. Le but de notre système, c'est de connaître l'état du moteur sans avoir besoin de descendre : passant, bloqué ou à vide. C'est prévu pour un usage domestique, une maison individuelle.");

// ================= 3. Principe =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Introduction" });
s.addText("Le courant nous dit dans quel état est le moteur", { placeholder: "title" });
[
  ["Courant trop fort", "Le moteur force : il est bloqué", "On coupe le moteur et on alerte", C.accent3],
  ["Courant normal", "La pompe vide l'eau", "On laisse tourner", C.accent4],
  ["Courant trop faible", "La pompe tourne à vide, sans eau", "On coupe le moteur", C.accent2],
].forEach((c, i) => {
  const x = 0.6 + i * 4.1;
  card(s, x, 1.5, 3.8, 2.3, C.background2, "cas" + i);
  s.addShape(pres.shapes.OVAL, { x: x + 0.3, y: 1.72, w: 0.4, h: 0.4, fill: { color: c[3] }, line: { color: c[3] } });
  T(s, c[0], { x: x + 0.85, y: 1.72, w: 2.8, h: 0.42, fontSize: 19, bold: true, valign: "middle" });
  T(s, c[1], { x: x + 0.3, y: 2.35, w: 3.3, h: 0.6, fontSize: 15 });
  T(s, c[2], { x: x + 0.3, y: 3.05, w: 3.3, h: 0.5, fontSize: 15, bold: true, color: c[3] });
});
card(s, 0.6, 4.1, 12.0, 1.0, ALERTE_BG, "demarrage");
T(s, [{ text: "Le piège : le démarrage. ", options: { bold: true, color: HEX.accent3 } }, { text: "Un moteur appelle beaucoup de courant quand il démarre, c'est normal. Donc on tolère ce pic quelques secondes : si le courant redescend, tout va bien ; s'il reste fort, le moteur est bloqué." }], { x: 0.85, y: 4.17, w: 11.5, h: 0.86, fontSize: 15, valign: "middle" });
[["3 jours", "la pompe se met en marche toute seule"], ["230 V", "secteur, à isoler de la partie 5 V"], ["LCD · LED", "pour afficher l'état du moteur"]].forEach((c, i) => {
  const x = 0.6 + i * 4.1;
  T(s, c[0], { x, y: 5.3, w: 3.8, h: 0.7, fontSize: 30, bold: true, color: C.accent1, fontFace: "Cambria", valign: "bottom" });
  T(s, c[1], { x, y: 6.05, w: 3.8, h: 0.4, fontSize: 14, color: C.text2 });
});
s.addNotes("1 min. Tout repose sur une idée : le courant dépend de ce que le moteur doit entraîner. S'il est bloqué, il force et le courant monte. S'il tourne à vide, il n'a rien à pousser et le courant baisse. Le piège, c'est le démarrage : il y a un pic de courant qui est normal, par exemple 3 A puis ça redescend vers 1 A (ordre de grandeur, on doit encore le mesurer sur la vraie pompe). Donc on laisse quelques secondes avant de décider.");

// ================= 4. Synoptique =================
pres.addSection({ title: "Composants" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Composants" });
s.addText("Trois composants isolent le 230 V de la partie 5 V", { placeholder: "title" });
s.addShape(pres.shapes.RECTANGLE, { x: 0.6, y: 1.4, w: 3.55, h: 5.3, fill: { color: "FBEDEC" }, line: { color: HEX.accent3, width: 1 }, objectName: "zone-secteur" });
T(s, "Côté secteur 230 V", { x: 0.8, y: 1.48, w: 3.2, h: 0.32, fontSize: 13, bold: true, color: C.accent3 });
s.addShape(pres.shapes.RECTANGLE, { x: 6.35, y: 1.4, w: 6.35, h: 5.3, fill: { color: HEX.lt2 }, line: { color: HEX.accent1, width: 1 }, objectName: "zone-5V" });
T(s, "Côté basse tension 5 V", { x: 6.55, y: 1.48, w: 4, h: 0.32, fontSize: 13, bold: true, color: C.accent1 });
s.addShape(pres.shapes.LINE, { x: 5.25, y: 1.4, w: 0.001, h: 5.3, line: { color: HEX.accent3, width: 2.5, dashType: "dash" } });
const bloc = (x, y, w, h, titre, sous, fill, ligne) => {
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w, h, rectRadius: 0.1, fill: { color: fill || HEX.lt1 }, line: { color: ligne || fill || HEX.lt1, width: ligne ? 1.5 : 0.5 }, objectName: titre });
  T(s, [{ text: titre, options: { bold: true, breakLine: true } }, { text: sous, options: { fontSize: 12, color: C.text2 } }], { x: x + 0.08, y: y + 0.05, w: w - 0.16, h: h - 0.1, fontSize: 14, valign: "middle", align: "center" });
};
bloc(0.85, 1.95, 2.5, 0.95, "Moteur", "pompe 230 V");
bloc(0.85, 3.6, 2.5, 0.95, "BTA12", "triac : marche ou arrêt");
bloc(0.85, 5.25, 2.5, 0.95, "Secteur", "phase et neutre");
bloc(4.35, 1.95, 1.8, 0.95, "ACS756", "capteur de courant", HEX.lt1, HEX.accent2);
bloc(4.35, 3.6, 1.8, 0.95, "MOC3023", "optotriac", HEX.lt1, HEX.accent2);
bloc(4.35, 5.25, 1.8, 0.95, "6N136", "optocoupleur", HEX.lt1, HEX.accent2);
bloc(7.95, 2.55, 2.2, 1.95, "PIC16F1788", "lit le courant, décide, commande", HEX.accent6);
bloc(10.7, 2.0, 1.8, 0.95, "LCD · LED", "affiche l'état");
bloc(10.7, 4.1, 1.8, 0.95, "Programmation", "SNAP, UART");
fleche(s, 2.1, 5.25, 2.1, 4.55);
fleche(s, 2.1, 3.6, 2.1, 2.9);
fleche(s, 3.35, 2.42, 4.35, 2.42, HEX.accent3);
fleche(s, 4.35, 4.07, 3.35, 4.07, HEX.accent3);
fleche(s, 3.35, 5.72, 4.35, 5.72, HEX.accent3);
fleche(s, 6.15, 2.42, 7.95, 3.1, HEX.accent1);
T(s, "AN0 (RA0)", { x: 6.5, y: 2.2, w: 1.3, h: 0.28, fontSize: 12, color: C.accent1, bold: true });
fleche(s, 7.95, 3.85, 6.15, 4.07, HEX.accent1);
T(s, "RB2", { x: 6.5, y: 4.15, w: 1.0, h: 0.28, fontSize: 12, color: C.accent1, bold: true });
fleche(s, 6.15, 5.72, 8.9, 4.5, HEX.accent1);
T(s, "passage par zéro", { x: 6.5, y: 5.75, w: 2.2, h: 0.28, fontSize: 12, color: C.accent1, bold: true });
fleche(s, 10.15, 3.1, 10.7, 2.5);
fleche(s, 10.7, 4.5, 10.15, 4.0);
T(s, "gâchette", { x: 3.37, y: 4.13, w: 0.78, h: 0.25, fontSize: 11, color: C.accent3, bold: true, align: "center" });
s.addNotes("1 min. À gauche le secteur et la pompe, à droite le 5 V avec le PIC. Il n'y a aucun fil entre les deux : les trois composants au milieu font le lien sans contact électrique. L'ACS756 mesure le courant du moteur par effet Hall et envoie une tension sur AN0. Le PIC décide et commande par RB2 l'optotriac MOC3023, qui déclenche la gâchette du triac BTA12. Le 6N136 sert à détecter quand la tension passe par zéro, pour déclencher les mesures de courant au bon moment.");

// ================= 5. Changements =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Composants" });
s.addText("Ce qui a changé depuis la pré-étude, et pourquoi", { placeholder: "title" });
s.addTable([
  [H("Fonction"), H("Pré-étude"), H("Aujourd'hui"), H("Pourquoi")],
  ["Couper le moteur", "Relais", { text: "Triac BTA12 + optotriac MOC3023", options: { bold: true } }, "Pas de pièce qui s'use, le PIC le commande à travers une isolation"],
  ["Microcontrôleur", "ESP32", { text: "PIC16F1788", options: { bold: true } }, "Celui de la carte SAE22 qu'on connaît. L'ESP32 reste une évolution pour envoyer une alerte par mail"],
  ["Mesure au bon moment", "Pas prévu", { text: "Optocoupleur 6N136", options: { bold: true } }, "Repérer le passage par zéro pour déclencher les mesures et, à terme, mesurer le déphasage"],
  ["Niveau d'eau", "Flotteur", { text: "Pas de flotteur", options: { bold: true } }, "La pompe démarre tous les 3 jours : le courant dit s'il y a de l'eau"],
  ["Carte", "À concevoir", { text: "Carte SAE22 adaptée", options: { bold: true } }, "On garde PIC, liaison série, LED, LCD ; on enlève la liaison avec le hacheur"],
], { x: 0.6, y: 1.5, w: 12.1, colW: [2.3, 1.5, 3.0, 5.3], fontSize: 14, color: HEX.dk1, border: { type: "solid", pt: 0.75, color: "C9D6DB" }, fill: { color: HEX.lt1 }, rowH: 0.78, valign: "middle", margin: 0.08 });
s.addNotes("1 min. Au départ on devait partir sur un relais, finalement on est passé au triac. On a ajouté l'optocoupleur 6N136 pour savoir à quel moment la tension passe par zéro, et déclencher les mesures de courant au bon endroit. Et on repart de la carte de la SAE22 du robot, qu'on modifie : on garde ce qui nous sert, on enlève la liaison avec le hacheur.");

// ================= 6. Composants puissance =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Composants" });
s.addText("Les composants de la partie mesure et puissance", { placeholder: "title" });
const fiche = (x, y, image, iw, ih, titre, ref, role, chiffre) => {
  card(s, x, y, 5.95, 2.5, C.background2, titre);
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x: x + 0.15, y: y + 0.15, w: 1.95, h: 2.2, rectRadius: 0.08, fill: { color: HEX.lt1 }, line: { color: HEX.lt1 } });
  s.addImage({ path: comp(image), x: x + 0.15 + (1.95 - iw) / 2, y: y + 0.15 + (2.2 - ih) / 2, w: iw, h: ih, altText: titre });
  T(s, [
    { text: titre, options: { bold: true, fontSize: 16, color: HEX.dk2, breakLine: true } },
    { text: ref, options: { fontSize: 12, color: C.text2, breakLine: true } },
    { text: role, options: { breakLine: true } },
    { text: chiffre, options: { bold: true, color: HEX.accent1 } },
  ], { x: x + 2.25, y: y + 0.13, w: 3.55, h: 2.25, fontSize: 13, paraSpaceAfter: 3 });
};
fiche(0.6, 1.4, "acs756.png", 1.4, 1.556, "Capteur de courant", "ACS756xCB-100B-PFF · Allegro", "Transforme le courant du moteur en tension, par effet Hall, sans contact avec le secteur.", "2,5 V à 0 A, puis 20 mV par ampère");
fiche(6.75, 1.4, "bta12.png", 1.75, 1.568, "Triac", "BTA12 · STMicroelectronics · TO-220 isolé", "L'interrupteur de puissance : laisse passer ou non le 230 V vers la pompe.", "12 A efficaces au maximum");
fiche(0.6, 4.05, "moc3023.png", 1.85, 1.236, "Optotriac", "MOC3023M · Fairchild / onsemi · DIP-6", "Le PIC allume une LED, la lumière déclenche la gâchette du triac. Aucun fil entre les deux.", "S'amorce à 5 mA max, tient 400 V crête");
fiche(6.75, 4.05, "6n136.png", 1.85, 1.443, "Optocoupleur", "6N136 · DIP-8", "Donne au PIC l'image du secteur, isolée, pour repérer le passage par zéro de la tension.", "T = 20 ms, deux zéros par période");
s.addNotes("1 min. Quatre composants clés. Le capteur Allegro transforme le courant en tension : 2,5 V au repos plus 20 mV par ampère. Le triac BTA12, c'est l'interrupteur de puissance. L'optotriac MOC3023 le commande : une LED côté PIC, un phototriac côté secteur. Il lui faut 5 mA au maximum pour s'amorcer. Le 6N136 sert à détecter le passage par zéro de la tension. La plupart de ces composants nous ont été fournis à l'IUT.");

// ================= 7. Nomenclature carte =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Composants" });
s.addText("Le microcontrôleur, l'afficheur et la nomenclature", { placeholder: "title" });
card(s, 0.6, 1.4, 5.3, 2.55, C.background2, "pic");
s.addShape(pres.shapes.RECTANGLE, { x: 0.75, y: 1.55, w: 2.7, h: 2.25, fill: { color: HEX.lt1 }, line: { color: HEX.lt1 } });
s.addImage({ path: comp("pic.png"), x: 0.9, y: 1.6, w: 2.393, h: 2.15, altText: "Brochage du PIC16F1788" });
T(s, [
  { text: "PIC16F1788", options: { bold: true, fontSize: 16, color: HEX.dk2, breakLine: true } },
  { text: "Microchip · DIP-28", options: { fontSize: 12, color: C.text2, breakLine: true } },
  { text: "CAN 12 bits, sorties, UART.", options: { breakLine: true } },
  { text: "RA0 : capteur · RB2 : optotriac · RB0 : LED", options: { bold: true, color: HEX.accent1 } },
], { x: 3.55, y: 1.5, w: 2.25, h: 2.35, fontSize: 13, paraSpaceAfter: 3 });
card(s, 0.6, 4.1, 5.3, 2.55, C.background2, "lcd");
s.addShape(pres.shapes.RECTANGLE, { x: 0.75, y: 4.25, w: 2.7, h: 2.25, fill: { color: HEX.lt1 }, line: { color: HEX.lt1 } });
s.addImage({ path: comp("lcd.png"), x: 0.98, y: 4.3, w: 2.249, h: 2.15, altText: "Schéma du LCD en mode 4 bits" });
T(s, [
  { text: "Afficheur LCD", options: { bold: true, fontSize: 16, color: HEX.dk2, breakLine: true } },
  { text: "Displaytech 162B · 2 × 16", options: { fontSize: 12, color: C.text2, breakLine: true } },
  { text: "Affiche l'état du moteur.", options: { breakLine: true } },
  { text: "Mode 4 bits : RA1 à RA7", options: { bold: true, color: HEX.accent1 } },
], { x: 3.55, y: 4.2, w: 2.25, h: 2.35, fontSize: 13, paraSpaceAfter: 3 });
s.addTable([
  [H("Repère"), H("Valeur"), H("Rôle")],
  ["C1", "47 µF", "Réserve d'énergie sur le 5 V"],
  ["C5", "100 nF", "Découplage du LCD"],
  ["R11, R13", "56 kΩ, 1,5 kΩ", "Contraste du LCD"],
  ["R14", "270 Ω", "Rétroéclairage du LCD"],
  ["R12", "10 kΩ", "Rappel de MCLR au 5 V"],
  ["J1, J2, J4", "Connecteurs", "Alimentation, série, programmation"],
  [{ text: "R7", options: { bold: true } }, { text: "≈ 330 Ω à valider", options: { color: HEX.accent3 } }, "LED de l'optotriac (calcul page suivante)"],
  [{ text: "R3, R4", options: { bold: true } }, { text: "À fixer", options: { color: HEX.accent3 } }, "Commande de la gâchette du triac"],
  [{ text: "R1, R2, R5, C2", options: { bold: true } }, { text: "À fixer", options: { color: HEX.accent3 } }, "Détection du zéro (6N136)"],
], { x: 6.2, y: 1.4, w: 6.5, colW: [1.55, 1.75, 3.2], fontSize: 13, color: HEX.dk1, border: { type: "solid", pt: 0.75, color: "C9D6DB" }, fill: { color: HEX.lt1 }, rowH: 0.5, valign: "middle", margin: 0.06 });
s.addNotes("45 s. Le PIC16F1788, c'est celui de la carte SAE22. J'ai choisi les broches dans la documentation : RA0 pour l'entrée analogique, RB2 pour commander l'optotriac. Le LCD est en mode 4 bits pour économiser des broches. En rouge, les valeurs de résistances qu'il nous reste à fixer avant d'imprimer la carte : je le dis clairement.");

// ================= 8. Schéma puissance =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Composants" });
s.addText("Le schéma de la partie puissance et de la détection du zéro", { placeholder: "title" });
s.addImage({ path: comp("sch_triac.png"), x: 0.6, y: 1.5, w: 6.7, h: 3.206, altText: "Schéma KiCad : optotriac, triac et moteur" });
s.addImage({ path: comp("sch_6n136.png"), x: 7.4, y: 1.7, w: 5.3, h: 2.324, altText: "Schéma KiCad : optocoupleur 6N136" });
T(s, "Version de travail KiCad du 2 novembre : le symbole « octo-triac » est à remplacer par le MOC3023 (6 broches)", { x: 0.6, y: 4.82, w: 12.1, h: 0.3, fontSize: 12, italic: true, color: C.text2 });
card(s, 0.6, 5.2, 6.5, 1.5, C.background2, "calcul");
T(s, [
  { text: "Calcul de R7, la LED de l'optotriac", options: { bold: true, color: HEX.accent1, breakLine: true } },
  { text: "Pire cas : R7 = (5 V − 1,5 V) / 10 mA = 350 Ω, donc 330 Ω.", options: { breakLine: true } },
  { text: "On obtient 10,6 mA : au-dessus des 5 mA d'amorçage, le PIC peut le fournir." },
], { x: 0.8, y: 5.28, w: 6.1, h: 1.34, fontSize: 14, paraSpaceAfter: 2 });
card(s, 7.3, 5.2, 5.4, 1.5, ALERTE_BG, "reste");
T(s, [
  { text: "À corriger avant d'imprimer", options: { bold: true, color: HEX.accent3, breakLine: true } },
  { text: "Passer le courant du moteur dans le capteur, ajouter le bornier secteur, revoir R4 en série avec le moteur." },
], { x: 7.5, y: 5.28, w: 5.0, h: 1.34, fontSize: 14, paraSpaceAfter: 2 });
s.addNotes("1 min. À gauche la commande : RB2 passe par R7 et allume la LED de l'optotriac, qui déclenche la gâchette du triac. Pour R7, le calcul : 5 V moins 1,5 V de LED, divisé par 10 mA, ça donne 350 Ω, on prend 330 Ω. À droite le 6N136 pour le passage par zéro. On s'est inspirés de schémas existants pour le triac parce qu'on n'en avait jamais vu, mais il faut adapter à nos composants. Ce schéma n'est pas encore définitif et je sais ce qu'il reste à corriger.");

// ================= 9. Lecture analogique =================
pres.addSection({ title: "Code" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Code" });
s.addText("La lecture analogique transforme une tension en nombre", { placeholder: "title" });
[["Tension", "entre 0 et 5 V sur RA0"], ["CAN du PIC", "convertisseur analogique numérique"], ["Nombre", "que le programme peut comparer"]].forEach((c, i) => {
  const x = 0.6 + i * 4.2;
  card(s, x, 1.4, 3.7, 0.95, i === 1 ? C.accent6 : C.background2, "can" + i);
  T(s, [{ text: c[0], options: { bold: true, breakLine: true } }, { text: c[1], options: { fontSize: 12, color: C.text2 } }], { x: x + 0.1, y: 1.44, w: 3.5, h: 0.87, fontSize: 16, align: "center", valign: "middle" });
  if (i < 2) fleche(s, x + 3.7, 1.875, x + 4.2, 1.875);
});
s.addTable([
  [H("Tension sur RA0"), H("Sur 8 bits (0 à 255)"), H("Sur 12 bits (0 à 4095)")],
  ["0 V", "0", "0"],
  ["2,5 V (capteur à 0 A)", "128", "2048"],
  ["5 V", "255", "4095"],
  [{ text: "Plus petit écart visible (quantum)", options: { bold: true } }, { text: "5 V / 256 = 19,5 mV", options: { bold: true } }, { text: "5 V / 4096 = 1,22 mV", options: { bold: true } }],
  [{ text: "Avec le capteur à 20 mV/A", options: { bold: true } }, { text: "1 point ≈ 1 A", options: { bold: true, color: HEX.accent3 } }, { text: "1 point ≈ 61 mA", options: { bold: true, color: HEX.accent4 } }],
], { x: 0.6, y: 2.6, w: 7.7, colW: [3.3, 2.2, 2.2], fontSize: 14, color: HEX.dk1, border: { type: "solid", pt: 0.75, color: "C9D6DB" }, fill: { color: HEX.lt1 }, rowH: 0.56, valign: "middle", margin: 0.07 });
card(s, 8.6, 2.6, 4.1, 3.4, C.background2, "bits");
T(s, [
  { text: "C'est quoi un bit ?", options: { bold: true, color: HEX.accent1, breakLine: true } },
  { text: "Une case qui vaut 0 ou 1. Plus on a de cases, plus on a de valeurs.", options: { breakLine: true } },
  { text: "8 bits : 2⁸ = 256 valeurs.", options: { breakLine: true } },
  { text: "12 bits : 2¹² = 4096 valeurs.", options: { breakLine: true } },
  { text: "Notre choix", options: { bold: true, color: HEX.accent1, breakLine: true } },
  { text: "8 bits suffisent pour tester avec le potentiomètre. Il faudra 12 bits avec le vrai capteur." },
], { x: 8.8, y: 2.7, w: 3.7, h: 3.2, fontSize: 14, paraSpaceAfter: 4 });
s.addNotes("1 min 30. Le PIC ne comprend pas une tension, il comprend des nombres. Le CAN, le convertisseur analogique numérique, découpe la plage de 0 à 5 V en petites marches et donne le numéro de la marche. Sur 8 bits il y a 256 marches, chaque marche fait 19,5 mV. Sur 12 bits il y en a 4096, chaque marche fait 1,22 mV. Pour le test au potentiomètre, 8 bits suffisent. Mais avec le vrai capteur à 20 mV par ampère, sur 8 bits une marche vaut déjà environ 1 A : c'est trop grossier pour une pompe d'environ 1 A. Donc on passera à 12 bits. Le PIC sait le faire : c'est juste le code à adapter.");

// ================= 10. Code : lecture =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Code" });
s.addText("Dans le code : lire le CAN et garder un nombre de 8 bits", { placeholder: "title" });
code(s, 0.6, 1.4, 7.1, 2.45, [
  "// main_simple.c",
  "uint8_t courant;     // nombre de 0 a 255",
  "",
  "uint8_t Lire_Courant(void)",
  "{",
  "  return (uint8_t)(ADC_GetConversion(",
  "    Capteur_COURANT, negChannel_ADNREF) >> 8);",
  "}",
], 14);
card(s, 0.6, 4.05, 7.1, 2.1, C.background2, "exemple");
T(s, [
  { text: "Exemple : le potentiomètre est à 2,5 V", options: { bold: true, color: HEX.accent1, breakLine: true } },
  { text: "Le CAN mesure sur 12 bits : 2048.", options: { breakLine: true } },
  { text: "Le résultat est rangé à gauche dans 16 bits, alors >> 8 garde les 8 bits de gauche.", options: { breakLine: true } },
  { text: "Lire_Courant() renvoie 128, c'est-à-dire le milieu de 0 à 255." },
], { x: 0.85, y: 4.16, w: 6.6, h: 1.9, fontSize: 15, paraSpaceAfter: 4 });
[
  ["uint8_t", "u = sans signe (unsigned), int = entier, 8 = 8 bits. Il va de 0 à 255. On prend le plus petit type qui suffit : le PIC calcule en 8 bits."],
  ["ADC_GetConversion", "Fonction générée par MCC : elle choisit l'entrée, lance la conversion, attend la fin et renvoie le résultat."],
  ["Capteur_COURANT", "Le nom donné à RA0 dans MCC, configurée en entrée analogique."],
  [">> 8", "Décale de 8 bits vers la droite : on garde seulement les 8 bits les plus importants."],
].forEach((r, i) => {
  const y = [1.4, 2.95, 4.1, 5.25][i];
  T(s, [{ text: r[0], options: { bold: true, fontFace: "Courier New", color: HEX.accent1, breakLine: true } }, { text: r[1], options: { fontSize: 14 } }], { x: 8.0, y, w: 4.7, h: i === 0 ? 1.5 : 1.1, fontSize: 15, paraSpaceAfter: 2 });
});
s.addNotes("1 min. Voilà ma fonction de lecture. uint8_t, ça veut dire entier non signé sur 8 bits, donc un nombre de 0 à 255. ADC_GetConversion est générée par MCC : je choisis l'entrée, elle lance la conversion et me rend le résultat. Le CAN travaille sur 12 bits, le résultat est rangé à gauche, donc en décalant de 8 je garde les 8 bits de poids fort. À 2,5 V, ça donne 128. L'intérêt d'avoir tout mis dans une seule fonction : quand on aura le vrai capteur, il n'y aura que l'intérieur de cette fonction à changer.");

// ================= 11. Code : états =================
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Code" });
s.addText("Une machine à cinq états décide de ce que fait le moteur", { placeholder: "title" });
const etat = (x, y, w, nom, fill) => {
  s.addShape(pres.shapes.ROUNDED_RECTANGLE, { x, y, w, h: 0.62, rectRadius: 0.3, fill: { color: fill }, line: { color: fill }, objectName: nom });
  T(s, nom, { x, y, w, h: 0.62, align: "center", valign: "middle", fontSize: 13, bold: true, color: C.background1 });
};
etat(0.6, 1.72, 1.45, "ARRET", HEX.accent5);
etat(2.85, 1.72, 1.85, "DEMARRAGE", HEX.accent1);
etat(5.5, 1.72, 1.45, "MARCHE", HEX.accent4);
etat(8.6, 1.35, 1.75, "BLOQUE", HEX.accent3);
etat(8.6, 2.1, 1.75, "A_VIDE", HEX.accent2);
fleche(s, 2.05, 2.03, 2.85, 2.03);
fleche(s, 4.7, 2.03, 5.5, 2.03);
fleche(s, 6.95, 1.95, 8.6, 1.66);
fleche(s, 6.95, 2.1, 8.6, 2.41);
T(s, "Moteur coupé. On n'en sort que par le bouton reset.", { x: 10.5, y: 1.5, w: 2.2, h: 1.05, fontSize: 12, color: C.text2, valign: "middle" });
code(s, 0.6, 2.95, 6.9, 3.65, [
  "case ETAT_MARCHE:",
  "  MOTEUR_ON();",
  "  if (courant > SEUIL_HAUT)      // trop fort",
  "  {",
  "    compteur_haut = compteur_haut + 1;",
  "    compteur_bas = 0;",
  "  }",
  "  else if (courant < SEUIL_BAS)  // trop faible",
  "  { ... }",
  "  if (compteur_haut >= DUREE_10S)",
  "  {",
  "    etat = ETAT_BLOQUE;",
  "  }",
  "  break;",
], 12.5);
[
  ["La boucle", "Le while(1) lit le courant et passe dans le switch(etat) toutes les 100 ms."],
  ["Les seuils", "#define SEUIL_BAS 91 et SEUIL_HAUT 173 : les 3 zones du potentiomètre. À refaire avec le vrai capteur."],
  ["Le démarrage", "3 s tolérées. Si le courant est encore trop fort à 5 s : BLOQUE."],
  ["La confirmation", "100 tours de 100 ms = 10 s de défaut avant de couper : un pic isolé ne coupe pas la pompe."],
].forEach((r, i) => {
  const y = 2.97 + i * 0.96;
  T(s, [{ text: r[0], options: { bold: true, color: HEX.accent1, breakLine: true } }, { text: r[1], options: { fontSize: 13.5 } }], { x: 7.8, y, w: 4.9, h: 0.92, fontSize: 15, paraSpaceAfter: 1 });
});
s.addNotes("1 min 30. Le programme est une machine à états : une variable etat et un switch avec un case par état. Dans MARCHE : si le courant dépasse le seuil haut, j'augmente un compteur ; s'il redevient normal, je remets à zéro. Au bout de 100 tours de 100 ms, soit 10 secondes, je passe en BLOQUE et je coupe le moteur. Comme ça, un pic isolé ne coupe pas la pompe. Pour le moment, le potentiomètre remplace le courant : c'est pour ça que les seuils sont 91 et 173. Le __delay_ms(100) bloque le programme pendant l'attente : pour la suite, on veut passer par des interruptions pour que le PIC puisse faire autre chose en même temps.");

// ================= 12. Bilan =================
pres.addSection({ title: "Bilan" });
s = pres.addSlide({ masterName: "CONTENU", sectionTitle: "Bilan" });
s.addText("Ce qu'on a, ce qu'il reste à faire, ce qu'on va obtenir", { placeholder: "title" });
s.addTable([
  [H("Bloc"), H("Ce qu'on a"), H("Reste à faire"), H("Ce qu'on va obtenir")],
  [{ text: "Mesure", options: { bold: true } }, "Lecture analogique testée au potentiomètre, valeurs relevées sous RealTerm", "Brancher l'ACS756, passer à 12 bits, mesurer sur la période", "Le vrai courant du moteur"],
  [{ text: "Commande", options: { bold: true } }, "BTA12 et MOC3023 choisis, schéma KiCad", "Fixer les résistances, tester sur la pompe", "Le PIC coupe le moteur en 230 V"],
  [{ text: "Détection du zéro", options: { bold: true } }, "6N136 ajouté au schéma", "Fixer ses valeurs, le relier au programme", "Des mesures au bon moment"],
  [{ text: "Programme", options: { bold: true } }, "Deux seuils testés sur LED ; machine à 5 états écrite et compilée", "Vrais seuils, interruptions, lancement tous les 3 jours", "Bloqué, à vide ou en marche reconnu seul"],
  [{ text: "Carte", options: { bold: true } }, "Carte SAE22 pour les essais, typons en cours", "Finir le schéma, imprimer, souder", "La carte finale du projet"],
  [{ text: "Évolutions", options: { bold: true } }, "Pilote LCD et affichage de l'état écrits", "LCD sur la carte, boutons, ESP32", "Alerte par mail, sans descendre à la cave"],
], { x: 0.6, y: 1.35, w: 12.1, colW: [1.9, 3.8, 3.3, 3.1], fontSize: 13, color: HEX.dk1, border: { type: "solid", pt: 0.75, color: "C9D6DB" }, fill: { color: HEX.lt1 }, rowH: 0.72, valign: "middle", margin: 0.07 });
T(s, "Prochaine étape : imprimer la carte, puis tester bloc par bloc : capteur et triac d'abord, puis optocoupleur, puis LED et LCD.", { x: 0.6, y: 6.5, w: 12.1, h: 0.4, fontSize: 14, bold: true, color: C.accent1 });
s.addNotes("1 min. Je distingue bien ce qui est fait de ce qui reste. Ce qui marche aujourd'hui : la lecture analogique et les seuils, testés au potentiomètre sur l'ancienne carte. On n'a pas encore de résultat avec le vrai capteur et le 230 V, parce que la carte n'est pas imprimée. On a un peu de retard sur le code. Pour rattraper : on avance bloc par bloc, et le code est prêt à recevoir le vrai capteur, il n'y aura que la fonction de lecture à changer. La date de fin ne change pas. [Montrer le planning si besoin.]");

// ================= 13. Conclusion =================
s = pres.addSlide({ masterName: "TITRE", sectionTitle: "Bilan" });
s.addText("Conclusion", { placeholder: "title" });
s.addText("Le principe et le code sont là : il reste à passer du potentiomètre au vrai capteur, sur la carte finale", { placeholder: "body" });
[
  ["Acquis", "Lecture analogique et CAN, configuration MCC sous MPLAB, machine à états, triac et optotriac, KiCad, Git à deux"],
  ["Difficultés", "On a cherché des composants sur internet alors qu'ils étaient à l'IUT : il fallait demander tôt. Connexion du SNAP, reprise d'une carte existante"],
  ["Part de chacun", "Moi : choix des composants, broches du PIC, code et lecture analogique. Mon binôme : KiCad, schéma, typons"],
].forEach((c, i) => {
  const y = 4.75 + i * 0.75;
  T(s, c[0], { x: 0.7, y, w: 2.2, h: 0.68, fontSize: 15, bold: true, color: C.accent2, valign: "middle" });
  T(s, c[1], { x: 2.9, y, w: 9.8, h: 0.68, fontSize: 14, color: C.background1, valign: "middle" });
});
s.addImage({ path: img("04_Maquette_3D/02_coffret_eclate.png"), x: 8.6, y: 0.7, w: 4.2, h: 3.15, altText: "Vue éclatée du coffret de commande" });
s.addNotes("45 s. Pour conclure : on a le principe, les composants et une bonne base de code. Il nous reste à imprimer la carte, tester avec le vrai capteur et adapter le code. Ce que je retiens : la lecture analogique, qu'on n'avait pas encore vue en cours, et le fait qu'il faut demander les composants disponibles dès le début. Merci, je suis prêt pour vos questions.");

(async () => {
  await pres.writeFile({ fileName: OUT });
  await applyTheme(OUT, THEME);
  console.log("écrit :", OUT);
})();

# Préparer la soutenance et le rapport de conception — Vide-cave

SAé 31/42 · BUT GEII 2ᵉ année · Groupe 311 · 2026-2027 · état du projet au 4 novembre 2026.

Ce guide reprend les deux consignes de l'IUT (« Le compte rendu de conception » et
« Préparer la soutenance orale ») et les applique **à notre projet**, avec les données
déjà présentes dans le dépôt. Les valeurs marquées *à confirmer* ne sont pas encore
validées : ne pas les présenter comme définitives.

---

## 0. Le message à faire passer (à savoir par cœur)

> « Une pompe de vide-cave reste à l'arrêt pendant des mois : si elle tombe en panne,
> on ne le découvre que le jour de l'inondation. Notre carte, à base de PIC16F1788,
> **mesure le courant du moteur** pour reconnaître un moteur **bloqué** (courant trop
> fort) ou une pompe **à vide** (courant trop faible), **coupe le moteur** par un
> optotriac et un triac, **prévient** l'utilisateur (LCD, LED, buzzer) et **teste la
> pompe tout seul tous les 3 jours** pendant au plus 1 minute. »

Trois idées clés que le jury doit retenir :
1. **Surveiller par le courant** : c'est l'image de la charge mécanique du moteur.
2. **Isoler le secteur 230 V de la logique 5 V** : capteur à effet Hall + optotriac.
3. **Autotest périodique** : on vérifie la pompe avant d'en avoir besoin.

---

## 1. Ce qui change depuis la pré-étude (à justifier, le jury va le demander)

| Sujet | Pré-étude | Conception actuelle | Justification à donner |
|---|---|---|---|
| Microcontrôleur | ESP32 pressenti | **PIC16F1788** | Étudié en cours GEII (MPLAB X, MCC, XC8), ADC 12 bits, timers, UART ; le Wi-Fi devient une évolution, pas le cœur du projet |
| Coupure du moteur | Relais électromécanique | **Optotriac MOC3041 + triac de puissance** | Pas d'usure mécanique, commutation au zéro de tension (moins de parasites), silencieux, isolation par l'optotriac |
| Mesure du courant | Hall / TC / shunt comparés | **ACS756 (effet Hall)** | Isolation intrinsèque, sortie directement lisible par l'ADC (2,5 V + 20 mV/A) — *calibre à rediscuter, voir §5* |
| Déphasage | Mesuré (ZMPT101B / H11AA1) | **Non câblé sur la carte** | **Décision à prendre** : soit on ajoute une voie tension isolée (détecteur de zéro type H11AA1), soit on retire le déphasage du périmètre et on le dit clairement |
| Carte | À concevoir | **Adaptation de la carte G206 (ancienne carte robot)** | Gain de temps : alimentation, PIC, LCD, ICSP déjà routés ; anciens composants retirés du PCB |
| Flotteur, Wi-Fi | Évoqués | Hors carte actuelle | À présenter comme hors périmètre ou évolution |

---

## 2. Rapport de conception : quoi mettre dans chaque partie

Ordre imposé par la consigne. Pour chaque partie : **à écrire**, **déjà disponible**,
**manquant**.

### a. Couverture
- Titre du **sujet** : « Surveillance et protection d'une pompe de vide-cave par mesure
  du courant — carte de commande à PIC16F1788 ».
- Noms des deux étudiants, groupe 311, BUT GEII 2, IUT Lyon 1 — département GEII,
  date de remise, **nom de l'enseignant encadrant**, année 2026-2027.

### b. Sommaire
- Sommaire du rapport **et** liste numérotée des annexes avec pages.

### c. Introduction (1 page)
- Problème (panne invisible), objectif, résumé du cahier des charges, synoptique simple :

```text
Secteur 230 V ──► [Triac] ──► Moteur pompe
                     ▲             │ courant
                [Optotriac]   [Capteur Hall]
                     ▲             │ tension image
                     └──── [PIC16F1788] ──► LCD · LED · buzzer
```

- Disponible : chapitres 1 et 2 de la pré-étude (à résumer, pas à recopier).

### d. Présentation approfondie + synoptique détaillé (A4, de gauche à droite)
Découper en fonctions avec **les mêmes noms partout** (texte, synoptique, schéma) :

| Fonction | Contenu | Entrées → sorties (noms à garder) |
|---|---|---|
| FP1 Alimentation | Bornier J1 5 V, C1 47 µF, C2/C5 100 nF | 5V_EXT → +5V, GND |
| FP2 Mesure du courant | ACS756 (U3), filtre RC 1 kΩ / 10 nF | I_MOTEUR → V_CAPTEUR (2,5 V + 20 mV/A) → AN0 |
| FP3 Traitement | PIC16F1788 (U1), reset SW4/R12, horloge interne | AN0 → CMD_OPTO, LCD, LED, BUZ |
| FP4 Commande de puissance | Transistor + R_LED 150 Ω, MOC3041, triac, Rg, Rgk, snubber | CMD_OPTO → PHASE_MOTEUR |
| FP5 Interface utilisateur | LCD 162B en 4 bits, LED D4/R10, buzzer BZ1/R9 | — |
| FP6 Programmation / debug | J4 ICSP (SNAP), J2 UART TTL | PGD, PGC, MCLR, TX, RX |
| (FP7 Référence de tension) | Détecteur de zéro isolé — **seulement si déphasage gardé** | SECTEUR → ZC → entrée capture |

Notions nouvelles à expliquer brièvement : effet Hall, optotriac à commutation au zéro,
triac et charge inductive (snubber), valeur efficace numérique, déphasage d'un moteur
asynchrone.

- Disponible : Synoptique V3 (draw.io), note « 01 - Fonctionnement et architecture ».
- Manquant : **synoptique détaillé avec ces noms de signaux**, cartouche (titre,
  auteurs, date).

### e. Gestion de projet
- **Organigramme des tâches** (WBS) : Gestion · Matériel (schéma, calculs, PCB,
  fabrication) · Logiciel (MCC, mesure, états, LCD, autotest) · Essais · Documents.
- **Planning actualisé** (le fichier `GANT P2506736.ods` n'a que les tâches A à D :
  le compléter) avec prévisionnel ET réel, et dire comment on rattrape le retard,
  **date de fin inchangée**.
- Retards réels à assumer : carte héritée avec des liaisons cassées, données de la
  pompe absentes, programmateur SNAP bloqué lors d'une mise à jour (12/10).
- Moyens de rattrapage : travail en parallèle (un sur le schéma/PCB, un sur le
  firmware MCC), firmware testé d'abord sur la partie basse tension seule, dépôt Git
  partagé (`SYNC.bat`).

### f. Schéma structurel détaillé (le cœur du rapport)
**Le schéma KiCad doit être corrigé AVANT d'être mis dans le rapport.** Liste bloquante :

- [ ] Relier la sortie de U3 à RA0/AN0 (aujourd'hui deux étiquettes différentes : `AN0` et `capteur de courant`).
- [ ] Relier RB2 du PIC à la commande de l'optotriac (étiquettes `RB2` et `commande octo coupleur ` différentes).
- [ ] Faire passer le courant du moteur dans IP+/IP− de l'ACS756.
- [ ] Remplacer le FODM3011 (4 broches, 250 V : trop faible pour 325 V crête) par le **MOC3041** (6 broches) : symbole **et** empreinte.
- [ ] Bornier secteur phase/neutre + bornier moteur + fusible ; supprimer R4 en série avec le moteur.
- [ ] Donner valeur et empreinte à R3, R4, R7 ; ajouter le transistor de commande de la LED.
- [ ] Buzzer : polarité inversée (+ à GND), choisir la référence.
- [ ] 100 nF de découplage au pied de l'ACS756 ; rapprocher C2 du PIC (33 mm aujourd'hui).
- [ ] Points de test : +5V, GND, V_CAPTEUR, CMD_OPTO ; cartouche à renommer (porte encore le titre de la carte robot).
- [ ] Distances d'isolement secteur / 5 V sur le PCB (la règle 0,5 mm du projet ne suffit pas pour du 230 V).

**Calculs à écrire et justifier** (déjà faits dans `05_Notes/Etude_MOC3041/DIMENSIONNEMENT.md`) :

| Élément | Valeur | Calcul / raison |
|---|---|---|
| R_LED MOC3041 | **150 Ω ¼ W** | IFT max 15 mA ; I_min = (4,75 − 1,5 − 0,2) / (1,05 × 150) ≈ 19,4 mA > 15 mA. 270 Ω donnerait 10,8 mA : insuffisant |
| Base transistor | 1 kΩ + 100 kΩ de rappel | I_B ≈ 2,95 mA pour saturer à ~30 mA ; transistor bloqué au reset |
| Rg (A2 → MOC) | 360 Ω | Exemple fiche onsemi en 240 V ; limite le courant de gâchette |
| Rgk (G → A1) | 330 Ω | Immunité aux dv/dt |
| Snubber | 39 Ω + 10 nF (classe X) | Charge inductive : à valider avec le triac réel |
| ACS756 | 20 mV/A, 2,5 V à 0 A | ADC 12 bits sous 5 V : 1,22 mV/pas → **61 mA par pas** |
| Filtre ADC | 1 kΩ / 10 nF | fc = 1/(2π·1k·10n) ≈ 15,9 kHz, n'affecte pas le 50 Hz |
| Contraste LCD | R11 56 kΩ / R13 1,5 kΩ | VO ≈ 5 × 1,5/57,5 ≈ 0,13 V |
| Rétroéclairage | R14 270 Ω | (5 − 3,2)/270 ≈ 6,7 mA |
| LED D4 | R10 270 Ω | (5 − 2)/270 ≈ 11 mA, compatible sortie PIC (25 mA max) |
| Reset | R12 10 kΩ | Rappel de MCLR au +5 V, SW4 à la masse |

### g. Nomenclature
- Point de départ : `03_Electronique/G206A_CAMEL_TP_CAO/Nomenclature_G206_groupee.csv`.
- Classer par famille (passifs, semi-conducteurs, CI, connectique, mécanique) avec
  repère, valeur, tension de service, type, **empreinte KiCad**.
- **Ajouter** ce qui manque : MOC3041, triac (ex. BTA16-600B *à confirmer*),
  transistor, fusible + porte-fusible, borniers secteur, condensateur X2, buzzer,
  dissipateur, boîtier IP, alimentation 5 V isolée.

**Demande de composants** = document séparé, sans repères (R1…), avec quantité,
référence fabricant, distributeur, référence distributeur, prix, délai.

### h. Organigrammes du programme
Écrits dans le style du cours INFO3 (MPLAB X + MCC) : pas de `float`, travail en
**points du CAN**, IT courte, plus petit type possible. Détail et squelette de
programme : chapitre 6 du rapport. À dessiner (draw.io) :
1. **Programme principal** : `SYSTEM_Initialize()` (broches `ANSEL`/`TRIS`, CAN, Timer2
   configurés **une seule fois**), init LCD, enregistrement de `IT_Timer2`, puis
   autorisation des IT **en dernier** (`GIE` + `PEIE`, car le Timer2 est une IT
   interne), puis `while(1) // ne se termine jamais`.
2. **Machine à états** : variable `etat` + `switch(etat)`, un `case` par état
   (ARRET, AUTO_TEST, FONCTIONNEMENT, DEFAUT), transitions par `if`.
3. **Base de temps** : Timer2 à 10 ms (FOSC = 4 MHz, TCY = 1 µs, prescaler 16 ×
   (PR2 + 1 = 125) × postscaler 5, donc PR2 = 124). L'IT **ne fait que lever
   `Flag_10ms`** (variable globale `volatile`) ; le `main` remet le flag à 0 le plus
   tôt possible et compte `Compteur_10ms`, `Secondes`, `Minutes`, `Heures`, tous en
   `uint8_t`. Heures = 72 → autotest ; 60 s au plus pour le test.
4. **Mesure** (`Mesure_Amplitude()`) : sur 20 ms, lire AN0, garder `maxi` et `mini` →
   amplitude = maxi − mini en **points du CAN** (q = 5 V / 4096 = 1,22 mV). Pas de
   racine carrée, pas de `float`, pas besoin de l'offset de 2,5 V.
5. **Décision** : temporisation au démarrage, puis amplitude > `SEUIL_HAUT` → DEFAUT
   bloqué ; amplitude < `SEUIL_BAS` → DEFAUT à vide. Seuils en points (`#define`).
6. **Affichage / alarmes** : `sprintf` dans un `char ligne_lcd[]`, LCD rafraîchi
   **seulement si l'état ou la mesure a changé** (évite le scintillement).

### i. Conclusion
- Avancement chiffré par bloc, difficultés rencontrées, ce qui reste à faire, apports.

### j. Annexes
Extraits de fiches techniques (PIC16F1788, ACS756, MOC3041, triac, LCD 162B),
simulations LTspice, règles PCB, brochage PIC (`Projet vide cave.X/README.md`),
rapport d'analyse de la carte, captures Blender / Anatomy.

---

## 3. Soutenance orale : déroulé pour un binôme (10 min + 10 à 20 min de questions)

| Temps | Qui | Diapo | Contenu |
|---|---|---|---|
| 0:00 | A | 1 Titre | Présentation des deux, sujet, encadrant |
| 0:30 | A | 2 Contexte | Le problème (photo / rendu Blender de l'installation) |
| 1:30 | A | 3 Cahier des charges | Fonctions, autotest 3 j / 1 min, critères |
| 2:30 | A | 4 Synoptique | Les 6 fonctions, l'isolation secteur / 5 V |
| 3:30 | A | 5 Choix et évolutions | Tableau §1 : ESP32 → PIC, relais → triac |
| 4:30 | A → B | Transition | « Je laisse X détailler la réalisation » |
| 5:00 | B | 6 Mesure du courant | ACS756, 20 mV/A, ADC, calcul RMS |
| 6:00 | B | 7 Commande de puissance | MOC3041, calcul R_LED 150 Ω, triac, snubber (visualisation Anatomy) |
| 7:00 | B | 8 Carte / PCB | Carte adaptée, nettoyage, routage face cuivre + straps |
| 7:45 | B | 9 Logiciel | Machine d'états + organigramme |
| 8:30 | A | 10 Avancement | Planning actualisé, fait / reste à faire, rattrapage |
| 9:15 | A + B | 11 Conclusion | Acquis techniques et personnels, part de chacun |

Règles : 5 min chacun, une seule idée par diapo, un schéma plutôt qu'un paragraphe,
répéter chronométré au moins deux fois, devant quelqu'un d'autre si possible.

Démonstrations possibles (courtes, déjà prêtes) : visualisation Anatomy
(`07_Visualisation_Anatomy/index.html` et `simulation.html`), rendus Blender
(`04_Maquette_3D/*.png`). Elles **illustrent** : ne pas les présenter comme des essais
de la carte réelle.

Rendre visible **la part personnelle** de chacun : qui a fait quoi (schéma, calculs,
PCB, firmware, maquette, documents).

---

## 4. Questions probables du jury et éléments de réponse

**Fonctionnement / choix**
1. *Pourquoi le courant pour surveiller ?* — Le courant d'un moteur asynchrone dépend
   du couple résistant : bloqué → courant proche du courant de démarrage (plusieurs fois
   le nominal) ; à vide → courant faible, surtout magnétisant.
2. *Pourquoi un triac et pas un relais ?* — Pas d'usure, silencieux, commutation au zéro
   de tension. Limites : chute ~1 V donc dissipation, courant de fuite, snubber
   nécessaire, et un triac en panne reste souvent passant (penser au fusible).
3. *À quoi sert l'optotriac ?* — Isoler le PIC du secteur (isolement de plusieurs kV)
   et déclencher la gâchette du triac de puissance.
4. *Que veut dire « zéro-crossing » pour le MOC3041 ?* — Il ne s'amorce que près du
   passage par zéro de la tension : moins de parasites. Ce n'est pas un variateur.
5. *Pourquoi le PIC et plus l'ESP32 ?* — Voir §1.
6. *Le déphasage, vous le mesurez comment ?* — Réponse honnête selon la décision :
   voie de zéro de tension isolée + capture timer, φ = 360° × Δt / 20 ms ; ou bien
   retiré du périmètre parce que le courant suffit pour les deux défauts.

**Calculs**
7. *Comment avez-vous choisi 150 Ω ?* — Calcul du courant LED dans le cas défavorable
   (5 V − 5 %, résistance + 5 %) pour rester au-dessus de IFT = 15 mA. Simulé sous LTspice.
8. *Pourquoi un transistor pour la LED ?* — ~20 mA, on soulage la sortie du PIC et
   l'optotriac reste bloqué au reset grâce au rappel de base.
9. *Quelle résolution de mesure ?* — 61 mA par pas avec l'ACS756 ±100 A.
10. *Un capteur 100 A pour une pompe de ~1,3 A, ce n'est pas surdimensionné ?* — Oui,
    c'est la limite actuelle : 1,3 A ≈ 21 pas d'ADC. Pistes : capteur à plus petit
    calibre (ex. ACS712-5A, 185 mV/A) ou amplification autour de 2,5 V (×5 → 12 mA par
    pas), en gardant la mesure du courant de démarrage. **Préparer la réponse avant
    l'oral** (et décider du capteur).
11. *Comment mesurez-vous le courant dans le programme ?* — En points du CAN, sans
    `float` : sur une période de 20 ms on garde le max et le min lus sur AN0,
    amplitude = max − min, comparée à des seuils en points (`#define`). Quantum
    q = 5 V / 4096 = 1,22 mV, soit 61 mA par point.
12. *Comment comptez-vous 3 jours ?* — Timer2 en interruption toutes les 10 ms ; l'IT
    lève seulement un flag, le `main` compte secondes, minutes, heures en `uint8_t`
    (Heures = 72). Pas de `__delay_ms` : pendant un délai le PIC ne voit rien.
12 bis. *Pourquoi `PEIE = 1` ?* — Le Timer2 est une interruption interne
    (périphérique) : il faut `PEIE` en plus de `GIE` et du bit `TMR2IE`.
12 ter. *Pourquoi l'IT est-elle si courte ?* — Sinon le main est bloqué : pas de
    `printf`, pas de `float`, pas de délai dans l'IT ; elle lève un flag, le main
    fait le reste. `Flag_10ms` est `volatile` car partagée entre l'IT et le main.
13. *Que se passe-t-il au démarrage du moteur ?* — Pic de courant normal : on ignore
    la mesure pendant une temporisation avant de comparer à Imax.

**Sécurité / réalisation**
14. *Comment est assurée la sécurité électrique ?* — Isolation Hall + optotriac, masses
    séparées (GND ≠ neutre), fusible, distances d'isolement sur le PCB, boîtier IP
    (cave humide), essais d'abord sans secteur.
15. *Pourquoi la carte G206 ?* — Réutilisation d'une base connue ; on a retiré
    21 empreintes inutiles du PCB et relevé les liaisons à corriger.
16. *Pourquoi des straps ?* — Carte simple face (cuivre côté B.Cu) fabriquée à l'IUT,
    quelques ponts filaires au lieu d'un double face.
17. *Comment allez-vous tester ?* — Partie basse tension d'abord (LCD, LED, ADC avec
    une tension de test), puis puissance sur charge résistive, puis la pompe.
18. *Et si le courant secteur est coupé ?* — Point ouvert de la pré-étude (batterie) :
    savoir dire si c'est retenu ou non.

**Gestion de projet**
19. *Où en êtes-vous par rapport au planning ? Comment rattraper ?*
20. *Qu'avez-vous appris ? Qu'est-ce que vous referiez autrement ?* — Exemple :
    relever la plaque de la pompe dès le début ; noms de nets identiques aux deux
    bouts ; Git pour travailler à deux.

---

## 5. Décisions à trancher avant d'écrire (avec l'enseignant si besoin)

- [ ] Relever la **plaque de la pompe** : tension, puissance, courant nominal, courant de démarrage. Sans ça, Imin/Imax et le triac ne sont pas justifiables.
- [ ] **Déphasage** : gardé (ajout voie de zéro isolée) ou retiré du périmètre ?
- [ ] **Capteur** : on garde l'ACS756 ±100 A ou on prend un calibre adapté ?
- [ ] **Triac** : référence exacte (IGT, courant, boîtier, dissipateur).
- [ ] **Alimentation 5 V** : module isolé secteur → 5 V sur la carte ou bloc externe ?
- [ ] Flotteur, Wi-Fi, batterie : dans ou hors périmètre ?
- [ ] Bouton d'**acquittement** distinct du reset SW4 ?

---

## 6. Ce qu'il faut retenir (fiche mémo chiffres)

- Secteur 230 V eff → **325 V crête** (358 V à 253 V) ; 50 Hz → T = 20 ms.
- Pompe ~300 W → I ≈ **1,3 A** (*hypothèse, plaque à relever*).
- ACS756 : **20 mV/A**, **2,5 V à 0 A** ; CAN 12 bits : quantum **q = 5 V / 4096 = 1,22 mV**, soit **61 mA par point**.
- MOC3041 : IFT max **15 mA**, tenue **400 V**, zéro-crossing ; R_LED **150 Ω**.
- FODM3011 retiré : **250 V** seulement.
- Filtre RC 1 kΩ / 10 nF : **fc ≈ 15,9 kHz**.
- Autotest : **Heures = 72** (compteurs `uint8_t`), durée ≤ **60 s** ; base de temps Timer2 **10 ms**, PR2 = **124**.
- Déphasage : φ = 360° × Δt / T ; 1° ↔ **55 µs**.
- PIC16F1788 : AN0 = mesure, RB2 = commande opto, RB0 = LED, RB1 = buzzer,
  RA1-RA7 = LCD 4 bits, RC6/RC7 = UART, RB6/RB7 = programmation.
- Carte 90 × 80 mm, routage B.Cu, 2 straps (GND 14 mm, MCLR 10 mm).

---

## 7. Liste de travail (qui fait quoi : à répartir)

| Tâche | Pour | Qui |
|---|---|---|
| Relever la plaque de la pompe | Calculs, seuils | |
| Trancher les décisions §5 | Tout | |
| Corriger le schéma KiCad (liste §2.f) | Schéma structurel | |
| Synoptique détaillé avec noms de signaux | Partie d | |
| WBS + Gantt actualisé (compléter le .ods) | Partie e | |
| Nomenclature complète + demande de composants | Partie g | |
| Générer le code MCC + organigrammes | Partie h | |
| Rédiger le rapport sur le document collaboratif | Rendu fin S3 | |
| Diaporama 11 diapos + 2 répétitions chronométrées | Soutenance | |
| Mettre à jour le Kanban (pourcentage ou couleur) | Suivi | |

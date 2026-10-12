# Visualisation du projet avec Anatomy

Ouvrir **[index.html](index.html)** dans un navigateur. La page est autonome, sans compte, sans serveur et sans accès à une API. La variante sombre est [index-dark.html](index-dark.html).

Cliquer un composant, survoler ses pièces ou utiliser les flèches dans le cadre pour consulter son rôle et les points encore incomplets. Les boutons donnent un accès direct au PIC, au capteur, à l'optotriac, au LCD et aux connecteurs.

## Étude MOC3041 et mesure de courant

Ouvrir **[simulation.html](simulation.html)** ou [sa variante sombre](simulation-dark.html). Régler RLED, courant supposé, déphasage, gain et IGT hypothétique ; lire l'oscillogramme au ralenti ou le parcourir avec le curseur. Le modèle R-L est pédagogique et ne valide pas le réamorçage d'un MOC sur moteur inductif. Les calculs et les bancs LTspice exécutés sont dans [l'étude de dimensionnement](../05_Notes/Etude_MOC3041/DIMENSIONNEMENT.md). Aucun fichier KiCad n'est modifié par cette simulation.

Reconstruction : `node build-simulation.mjs`, puis `node build-simulation.mjs --dark`. La géométrie réutilise l'illustration actuelle déjà auditée. Les contrôles couvrent affichage téléphone/bureau, thème sombre, pause, paramètres et absence de débordement.

## Ce que montre la vue

- Les 21 empreintes conservées dans le PCB G206 après nettoyage, avec leurs positions X/Y réellement exportées de KiCad.
- Le capteur U3 et l'optotriac TRIAC1 encore hors carte. Ils sont représentés sur des supports d'inspection illustratifs, sans changement d'implantation dans le fichier KiCad.
- Le rôle de la commande, de l'isolation optique et du circuit de puissance, avec les liaisons encore ouvertes clairement décrites.
- Une image du cuivre réel du PCB : B.Cu bleu, deux straps rouges F.Cu.

Les boîtiers, hauteurs et supports sont simplifiés ; ce n'est ni un export STEP, ni un plan d'assemblage, ni un dimensionnement de puissance. Le triac de puissance et le buzzer manquants ne sont pas remplacés par des pièces supposées. Le schéma et le PCB KiCad restent les sources de vérité.

## Reconstruire la page

Node.js 20.10 ou ultérieur suffit. Aucun `npm install` n'est nécessaire.

```sh
node build.mjs
node build.mjs --dark
node build.mjs --audit
```

`implantation.json` contient les positions et pastilles extraites du PCB ; si le PCB évolue, actualiser cet export et `pcb-nettoye.png` avant de reconstruire la page. Les notes de fonctionnement intégrées dans `build.mjs` doivent aussi être relues après correction du schéma.

## Origine et vérification

Le dessin utilise le kit SVG isométrique du skill [Anatomy](https://github.com/wheresryan22/anatomy). Sa licence MIT est conservée dans [LICENSE.anatomy](LICENSE.anatomy). Les positions et les textes décrivent notre projet G206 ; aucun circuit ne se trouve modifié par ce visualiseur.

Les captures de contrôle couvrent le bureau, le téléphone, le thème sombre, la navigation au clavier et les composants hors carte. Les vérifications géométriques concernent uniquement l'illustration simplifiée et ne valident pas le PCB pour fabrication.

Voir [le rapport technique](../05_Notes/Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md), [les règles PCB](../05_Notes/Analyse_G206_2026-10-12/REGLES_PCB.md) et [les composants CSV](../03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv).

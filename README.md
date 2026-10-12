# Projet vide-cave — GEII

Étude d'une commande de pompe vide-cave avec surveillance du courant, affichage et alarmes. Le projet KiCad de travail est la carte **G206_CAMEL_TP_CAO**, adaptée d'une ancienne carte. **Statut : EN COURS — non prêt pour fabrication.** L'étude de commande et de puissance reste à terminer : la présence des sources dans ce dépôt ne constitue pas une validation pour fabrication ou mise sous tension.

## Ouvrir le projet

- **KiCad de travail :** [G206_CAMEL_TP_CAO.kicad_pro](03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_pro).
- **Notes Obsidian :** ouvrir `05_Notes/Obsidian` comme coffre, puis [00 — Accueil](05_Notes/Obsidian/00%20-%20Accueil.md).
- **Analyse de la carte et comparaison aux documents :** [rapport du 12 octobre 2026](05_Notes/Analyse_G206_2026-10-12/RAPPORT_ANALYSE.md).
- **Étude MOC3041 :** [dimensionnement proposé et bancs LTspice](05_Notes/Etude_MOC3041/DIMENSIONNEMENT.md), [simulation Anatomy](07_Visualisation_Anatomy/simulation.html) et [travail à deux sur GitHub](05_Notes/Obsidian/09%20-%20Travail%20a%20deux%20et%20MOC3041.md). Les valeurs secteur restent conditionnées au triac et à la pompe réels.
- **Composants :** [CSV détaillé, une ligne par référence](03_Electronique/G206A_CAMEL_TP_CAO/Composants_G206.csv) et [nomenclature groupée](03_Electronique/G206A_CAMEL_TP_CAO/Nomenclature_G206_groupee.csv). Le schéma compte 24 composants ; le CSV détaillé indique aussi les écarts avec le PCB.
- **Rapport de pré-étude :** [document Word](01_Documents/Rapport/Rapport_pre-etude_vide-cave.docx).
- **Synoptique modifiable :** [Synoptique V3](02_Synoptiques/Synoptique%20V3.drawio).
- **Maquette :** [fichier Blender](04_Maquette_3D/Vide_cave_GEII.blend) et [notice](04_Maquette_3D/LISEZ_MOI.md).
- **Visualisation pédagogique Anatomy :** ouvrir [index.html](07_Visualisation_Anatomy/index.html) dans un navigateur, ou consulter [l'aperçu](07_Visualisation_Anatomy/preview.png) et [la notice](07_Visualisation_Anatomy/README.md). Cette visualisation explique le projet ; elle ne produit pas le PCB. Sur GitHub, télécharger ou cloner le dépôt pour ouvrir la page localement.

## Sources et versions

| Dossier | Contenu et statut |
| --- | --- |
| `01_Documents` | Consignes, rapport Word, présentation et planning |
| `02_Synoptiques` | Sources Draw.io et versions exportées |
| `03_Electronique/G206A_CAMEL_TP_CAO` | **Carte de travail à poursuivre**, schéma, PCB et bibliothèques locales |
| `03_Electronique/Carte_vide_cave_PIC_Allegro_TRIAC` | Proposition indépendante ; ne remplace pas la carte G206 et n'applique pas son routage simple face |
| `03_Electronique/Projets_KiCad_initiaux` | Versions antérieures conservées comme références |
| `04_Maquette_3D` | Scène Blender, rendus et scripts existants |
| `05_Notes/Obsidian` | Coffre de suivi : fonctionnement, contraintes, décisions, anomalies et tâches |
| `05_Notes/PROJET VIDE CAVE` | Notes personnelles préexistantes conservées |
| `05_Notes/Analyse_G206_2026-10-12` | Rapport de contrôle et tableau de comparaison des composants |
| `06_Archives_et_exports` | Archives, anciens exports et journal de rangement |
| `07_Visualisation_Anatomy` | Visualisation du fonctionnement et des blocs du projet |

Les Gerbers et typons déjà présents sont des exports antérieurs. **Les régénérer seulement après validation du schéma et du PCB** ; leur présence ne garantit pas leur concordance avec les sources actuelles.

## Consignes de conception

1. Router les connexions courantes sur **B.Cu, bleu**.
2. Limiter **F.Cu, rouge** à quelques straps filaires ; cette carte n'est pas à généraliser en double face.
3. Les composants supprimés du schéma lors de l'adaptation de l'ancienne carte sont à nettoyer sur le PCB.
4. Conserver les composants présents au schéma. Ne pas déplacer ni remplacer les autres composants conservés pour effectuer ce nettoyage.
5. Garder les règles KiCad existantes. La consigne bleu/straps est une décision de fabrication à vérifier visuellement, pas une restriction automatique du DRC.

Voir [les règles PCB](05_Notes/Obsidian/02%20-%20Regles%20PCB.md) et [les problèmes à résoudre](05_Notes/Obsidian/04%20-%20Anomalies%20et%20verifications.md). Les valeurs de courant et la plaque moteur restent à renseigner ; les hypothèses inscrites dans une variante ne sont pas des mesures de la pompe.

## Travailler depuis un autre ordinateur

Installer Git, KiCad **10.0.6 ou une version 10 compatible**, puis Obsidian si souhaité. Les bibliothèques locales de G206 sont dans le dépôt et leurs tables utilisent `${KIPRJMOD}` : conserver l'ensemble du dossier KiCad au même endroit. Ne pas ajouter de bibliothèques par des chemins absolus propres à un PC.

```sh
git clone https://github.com/glm02/projet-vide-cave.git
cd projet-vide-cave
git pull --ff-only
```

Ouvrir ensuite le `.kicad_pro` indiqué plus haut. Pour enregistrer et partager une session :

```sh
git status
git add -- "03_Electronique/G206A_CAMEL_TP_CAO" "05_Notes/Obsidian"
git diff --cached --stat
git commit -m "Décrire la modification réalisée sur la carte"
git push
```

Adapter les chemins ajoutés aux documents réellement modifiés. Pour un rapport Word, une présentation ou une scène Blender, fermer le fichier avant de le versionner. Faire `git pull --ff-only` avant de commencer une nouvelle session. **Une personne à la fois sur le même schéma/PCB ou document binaire** : Git conserve l'historique mais ne fusionne pas automatiquement un routage ou un Word. Si le pull refuse, conserver les changements locaux et résoudre l'écart avant de poursuivre ; ne pas forcer le push.

Le schéma peut être modifié volontairement sans importer immédiatement toutes les différences dans le PCB. Avant « Mettre à jour le PCB depuis le schéma », vérifier les composants, empreintes, réseaux et conséquences dans le rapport : les divergences de R3, R4 et R7 nécessitent une décision explicite.

## Sauvegardes et portabilité

Git versionne les sources, bibliothèques et documents utiles. Il ignore les préférences KiCad `.kicad_prl`, les verrous, caches, sauvegardes automatiques, copies temporaires de l'analyse, état des fenêtres Obsidian et l'alias local `maquette_3d`. Les fichiers ignorés restent sur le PC : le dépôt distant ne les sauvegarde pas. Les archives documentaires existantes sont conservées.

Les scripts historiques de la variante et quelques anciennes notices utilisent encore des chemins propres au PC d'origine. Ils sont conservés comme sources historiques ; leur exécution sur un autre poste peut demander une adaptation des chemins d'outils. Le [guide Git du coffre](05_Notes/Obsidian/07%20-%20Guide%20Git%20et%20portabilite.md) distingue ces scripts de l'ouverture du projet G206.

Le dépôt contient des documents scolaires, bibliothèques locales et ressources tierces. Aucun changement de licence globale n'est imposé aux fichiers préexistants. Les ressources Anatomy conservent leur licence et leur attribution propres.

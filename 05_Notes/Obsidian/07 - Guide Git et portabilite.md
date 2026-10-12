---
tags: [vide-cave, git, outils]
date: 2026-10-12
---
# Guide Git et portabilité

Retour : [[00 - Accueil]] · Dépôt : [glm02/projet-vide-cave](https://github.com/glm02/projet-vide-cave)

## Premier poste supplémentaire

```sh
git clone https://github.com/glm02/projet-vide-cave.git
cd projet-vide-cave
```

Installer KiCad 10 compatible avec la version 10.0.6 utilisée pour l'analyse, puis ouvrir `03_Electronique/G206A_CAMEL_TP_CAO/G206_CAMEL_TP_CAO.kicad_pro`. Les tables des bibliothèques G206 utilisent `${KIPRJMOD}` et les fichiers associés sont versionnés. Ne pas déplacer uniquement le `.kicad_pro`.

Pour les notes, ouvrir le dossier `05_Notes/Obsidian` comme coffre existant. Aucun plugin communautaire n'est nécessaire. Les notes personnelles antérieures restent dans `05_Notes/PROJET VIDE CAVE`.

## Avant et après une séance

1. Fermer les fichiers modifiés sur l'autre poste et y faire commit/push.
2. Ici, vérifier `git status`, puis `git pull --ff-only` avant de travailler.
3. Modifier les sources utiles et enregistrer les décisions dans le journal.
4. Enregistrer et fermer KiCad/Word/Blender.
5. Contrôler `git status` et ajouter uniquement les changements concernés.
6. Vérifier `git diff --cached --stat`, faire commit puis push.

```sh
git add -- "03_Electronique/G206A_CAMEL_TP_CAO" "05_Notes/Obsidian"
git diff --cached --stat
git commit -m "Corriger la connexion de mesure du courant"
git push
```

Le message doit décrire la modification réellement faite. Pour écrire sur GitHub depuis un nouveau poste, se connecter à GitHub avec le compte autorisé, par exemple avec `gh auth login` si GitHub CLI est installé. Aucun identifiant ni jeton ne doit être enregistré dans le projet.

## Collaboration

Une personne à la fois travaille sur un même PCB, un même schéma, un Word, un PowerPoint ou un Blender. Une branche peut isoler un travail, mais elle ne rend pas les conflits de routage automatiques. Si deux personnes travaillent en parallèle, répartir les fichiers et les noter dans [[08 - Journal de travail]].

Si `git pull --ff-only` refuse de continuer, conserver les modifications locales et examiner l'historique avec la personne qui a modifié le dépôt. Éviter tout push forcé ou suppression de fichiers pour contourner l'écart.

## Ce qui suit et ce qui reste local

| Versionné | Local, ignoré |
| --- | --- |
| `.kicad_pro`, `.kicad_sch`, `.kicad_pcb`, bibliothèques et tables locales | `.kicad_prl`, verrous, `fp-info-cache`, `.history`, sauvegardes KiCad |
| Documents scolaires et sources Draw.io | Verrous Office/Draw.io |
| Notes Markdown et configuration minimale du coffre | État des fenêtres Obsidian, cache et plugins personnels |
| Scène Blender et scripts existants | `.blend1`, caches Python, `.runtime`, alias `maquette_3d` |
| Rapport d'analyse et comparaison initiale des composants | Snapshots, scripts temporaires, JSON et rendus de travail de l'analyse |

Les sauvegardes ignorées ne sont pas copiées depuis GitHub. L'archive avant nettoyage reste sur le PC d'origine ; un commit Git couvre les fichiers suivis à partir du premier enregistrement du dépôt.

Les anciennes notices et quelques scripts de génération de la variante citent encore des chemins propres au PC d'origine. L'ouverture de G206 est portable grâce à ses bibliothèques locales ; la relance de ces scripts historiques nécessite l'adaptation des chemins d'outils. Ne pas les lancer pour remplacer ou régénérer automatiquement G206.

Les liens relatifs des notes supposent que le coffre reste dans `05_Notes/Obsidian` à l'intérieur du dépôt. Pour emporter toutes les pièces liées, cloner le dépôt complet.

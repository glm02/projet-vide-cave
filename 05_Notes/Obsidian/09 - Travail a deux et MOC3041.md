# Travail à deux et MOC3041

[[00 - Accueil]] · [[05 - Taches]]

## Étude en cours

[Dimensionnement proposé et résultats LTspice](../Etude_MOC3041/DIMENSIONNEMENT.md) · [Simulation Anatomy](../../07_Visualisation_Anatomy/simulation.html).

MOC3041 choisi pour la suite. Le schéma n'a pas encore été converti du FODM3011 à 4 broches vers le MOC à 6 broches. La référence du triac et les données de la pompe restent requises avant de figer la puissance. L'amplification du capteur est optionnelle ; commencer par offset, filtrage et ADC 12 bits puis évaluer la précision réelle.

## Autoriser le collègue à pousser

> Méthode simple retenue le 21 octobre 2026 : [GUIDE_TRAVAILLER_A_DEUX.md](../../GUIDE_TRAVAILLER_A_DEUX.md) et `SYNC.bat` à la racine (un double-clic récupère puis envoie). La méthode par branches ci-dessous reste possible pour des modifications longues.

Le 12 octobre 2026, la liste des collaborateurs GitHub ne montre que le propriétaire `glm02`. Il faut le pseudo GitHub du collègue et le message d'erreur pour confirmer le diagnostic.

Dans [les réglages d'accès du dépôt](https://github.com/glm02/projet-vide-cave/settings/access), ajouter son compte personnel. Il doit accepter l'invitation, puis utiliser sa propre authentification GitHub ; le caractère public du dépôt autorise la lecture, pas automatiquement l'écriture. Ne pas partager le compte ou le token du propriétaire. [Documentation GitHub](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/repository-access-and-collaboration/inviting-collaborators-to-a-personal-repository).

Avec GitHub Desktop, il peut cloner le dépôt, faire Commit, puis Push. Sur l'autre PC, Fetch puis Pull récupère les changements. Ce n'est pas une synchronisation en direct : les modifications non enregistrées ou non poussées restent locales. Si l'invitation n'est pas possible, un fork + pull request permet de proposer les changements ; sinon transmettre une copie datée et l'intégrer sur une branche, sans écraser le projet courant.

## Éviter de modifier le même schéma à deux

1. Avant de travailler : enregistrer/fermer KiCad et récupérer la dernière version avec Pull.
2. Se répartir les fichiers ou les feuilles hiérarchiques : par exemple commande et puissance dans des feuilles séparées, sans inventer cette séparation dans le projet actuel.
3. Chacun travaille sur sa branche et pousse des étapes courtes ; l'autre voit sa branche après Fetch. Les modifications deviennent communes après intégration dans `main` puis Pull.
4. Pour le PCB commun, se passer la main : une seule personne modifie/routage à la fois. Les conflits KiCad se résolvent en vérifiant le circuit, pas en combinant automatiquement des lignes.
5. Refaire synchronisation schéma/PCB, ERC et DRC après intégration. Ne pas utiliser `push --force` pour effacer les changements de l'autre.

Exemple terminal pour le collègue, dans le clone du projet :

```powershell
git pull --ff-only
git switch -c collegue/puissance
# Modifier puis enregistrer les fichiers dans KiCad.
git add -- 03_Electronique/G206A_CAMEL_TP_CAO
git commit -m "Avancer la partie puissance"
git push -u origin collegue/puissance
```

Une demande d'intégration (pull request) permet de relire les modifications avant fusion. Si `git pull --ff-only` refuse, arrêter l'intégration et examiner la divergence ; ne pas supprimer les modifications locales.

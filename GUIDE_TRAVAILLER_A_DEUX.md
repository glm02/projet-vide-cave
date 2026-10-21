# Travailler à deux sur la carte KiCad

Ce guide t'explique comment récupérer le projet sur ton PC et rester
synchronisé avec Rafaël : quand l'un modifie la carte, l'autre récupère la
modif en un double-clic.

L'installation se fait **une seule fois** (étapes 1 à 5, environ 15 minutes).
Après, il n'y a plus que la routine de l'étape 6.

---

## Étape 1 : créer ton compte GitHub

GitHub, c'est le « cloud » où le projet est stocké. C'est gratuit.

1. Va sur **https://github.com/signup**
2. Entre ton email, choisis un mot de passe et un **nom d'utilisateur**
   (le « pseudo »).
3. Valide le code reçu par email.
4. **Envoie ton pseudo GitHub à Rafaël** (par message). Il va t'inviter sur
   le projet.

---

## Étape 2 : accepter l'invitation

1. Tu reçois un email de GitHub : « **glm02 invited you to glm02/projet-vide-cave** ».
2. Clique sur **View invitation**, puis sur le bouton vert **Accept invitation**.

> Sans ça, tu pourras récupérer le projet mais pas envoyer tes modifs.

---

## Étape 3 : installer Git

Git, c'est le petit programme qui fait les envois et les récupérations.

1. Va sur **https://git-scm.com/download/win**
2. Clique sur **Click here to download** (le téléchargement démarre).
3. Lance le fichier téléchargé (`Git-...-64-bit.exe`).
4. Clique sur **Next** à chaque écran, **sans rien changer**, puis
   **Install**, puis **Finish**.

---

## Étape 4 : installer KiCad 10

Le projet est enregistré avec **KiCad 10**. Avec une version plus ancienne
(KiCad 8 ou 9), le schéma **ne s'ouvrira pas**.

- Si tu as déjà KiCad : ouvre-le, menu **Aide → À propos de KiCad**. Le
  numéro doit commencer par **10**.
- Sinon : télécharge-le sur **https://www.kicad.org/download/windows/**
  et installe-le avec les options par défaut.

---

## Étape 5 : récupérer le projet sur ton PC

1. Appuie sur les touches **Windows + R** en même temps.
   Une petite fenêtre « Exécuter » s'ouvre.
2. Tape **`cmd`** puis appuie sur **Entrée**. Une fenêtre noire s'ouvre.
3. Copie la ligne ci-dessous, colle-la dans la fenêtre noire (clic droit
   suffit pour coller), puis appuie sur **Entrée** :

   ```
   git clone -c core.longpaths=true https://github.com/glm02/projet-vide-cave.git "%USERPROFILE%\Documents\projet-vide-cave"
   ```

4. Attends que ça s'arrête (quelques secondes à une minute).
   Tu peux fermer la fenêtre noire.

Le projet est maintenant dans **Documents → projet-vide-cave**.

La carte KiCad est ici :

```
Documents\projet-vide-cave\03_Electronique\G206A_CAMEL_TP_CAO\G206_CAMEL_TP_CAO.kicad_pro
```

Double-clique sur ce fichier `.kicad_pro` pour ouvrir le projet dans KiCad.

> Astuce : clic droit sur `SYNC.bat` (à la racine du dossier
> `projet-vide-cave`) → **Envoyer vers → Bureau (créer un raccourci)**.
> Tu auras le bouton de synchro directement sur ton bureau.

---

## Étape 6 : la routine, à chaque séance

**Avant de travailler**

1. KiCad doit être **fermé**.
2. Double-clique sur **`SYNC.bat`** : tu récupères la dernière version de
   Rafaël.
3. Ouvre KiCad et travaille.

**Après avoir travaillé**

1. Enregistre (**Ctrl + S**) dans le schéma ET dans le PCB.
2. **Ferme KiCad.**
3. Double-clique sur **`SYNC.bat`** : tes modifs partent sur GitHub.
4. Préviens Rafaël : « j'ai synchronisé ».

La **première fois** que tu envoies, une fenêtre GitHub s'ouvre : clique
sur **Sign in with your browser**, connecte-toi et autorise. Ça ne se
demandera plus ensuite.

Le script te demande aussi ton prénom la première fois (il apparaît dans
l'historique des modifs).

---

## Ce que tu peux voir dans la fenêtre de SYNC.bat

| Message | Ce que ça veut dire | Que faire |
|---|---|---|
| `OK  Tout est à jour sur GitHub.` | Tout s'est bien passé. | Rien, ferme la fenêtre. |
| `KiCad semble ouvert` | KiCad est encore ouvert. | Ferme KiCad et relance SYNC.bat. Si KiCad est vraiment fermé (après un plantage), réponds `o`. |
| `CONFLIT : ton collègue et toi avez modifié le même fichier` | Vous avez modifié la même chose en même temps. Ton dossier a pris **sa** version ; **la tienne n'est pas perdue**, elle est mise de côté sur GitHub. | Appelle Rafaël, il récupère ta version. |
| `... fichiers ont disparu du dossier` | Beaucoup de fichiers manquent d'un coup. Par sécurité, rien n'est envoyé. | Ne touche à rien et appelle Rafaël. |
| `Impossible de joindre GitHub` | Pas d'internet, ou pas d'accès au projet. | Vérifie ta connexion et l'étape 2. |
| `Envoi impossible` | Tu n'as pas encore accepté l'invitation. | Refais l'étape 2. |
| `Git n'est pas installé` | Étape 3 oubliée. | Refais l'étape 3. |

---

## Les 3 règles d'or

1. **Toujours fermer KiCad avant SYNC.bat.** Sinon KiCad peut réécraser la
   nouvelle version avec l'ancienne qu'il a en mémoire.
2. **Ne pas modifier la même chose en même temps.** Mettez-vous d'accord :
   par exemple, l'un fait le schéma pendant que l'autre fait le routage du
   PCB. Ou bien chacun son tour.
3. **Synchroniser souvent** : au début et à la fin de chaque séance.

Pas de panique en cas d'erreur : chaque synchronisation garde une copie sur
GitHub, et on peut toujours revenir à une version précédente.

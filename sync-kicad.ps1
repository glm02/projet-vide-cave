# Synchronise le projet KiCad avec GitHub, pour travailler à deux.
# Ordre : 1) on enregistre ses modifs localement, 2) on récupère celles du
# collègue, 3) on envoie le tout. À lancer avec SYNC.bat, KiCad FERMÉ :
# si KiCad est ouvert pendant la récupération, il réécrase les nouveaux
# fichiers avec l'ancienne version qu'il a en mémoire.
param([switch]$NoPause)

$ErrorActionPreference = 'Continue'
Set-Location -LiteralPath $PSScriptRoot

function Fin($code) {
    Write-Host ""
    if (-not $NoPause) { Read-Host "Appuie sur Entrée pour fermer" | Out-Null }
    exit $code
}
function Ok($msg)     { Write-Host "  OK  $msg" -ForegroundColor Green }
function Info($msg)   { Write-Host "  ..  $msg" -ForegroundColor Cyan }
function Erreur($msg) { Write-Host "  !!  $msg" -ForegroundColor Red }

Write-Host ""
Write-Host "=== Synchronisation du projet KiCad ===" -ForegroundColor White
Write-Host ""

# --- Vérifications de base ---------------------------------------------------
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    Erreur "Git n'est pas installé. Installe-le depuis https://git-scm.com/download/win puis relance."
    Fin 1
}
git rev-parse --is-inside-work-tree *> $null
if ($LASTEXITCODE -ne 0) {
    Erreur "Ce dossier n'est pas relié à GitHub. Utilise RECUPERER-PROJET.bat pour récupérer le projet."
    Fin 1
}

# KiCad laisse un fichier .lck à côté de chaque fichier ouvert.
$verrous = Get-ChildItem -Recurse -Force -Filter *.lck -ErrorAction SilentlyContinue
if ($verrous) {
    Erreur "KiCad semble ouvert ($($verrous[0].Name)). Ferme KiCad d'abord."
    if ($NoPause) { Fin 1 }
    $rep = Read-Host "  KiCad est vraiment fermé (il a pu planter) ? Continuer quand même ? (o/N)"
    if ($rep -notmatch '^[oOyY]') { Fin 1 }
}

# Nom affiché dans l'historique : demandé une seule fois par poste.
$nom = git config user.name
if (-not $nom) {
    $nom = Read-Host "  Premier lancement : ton prénom (pour l'historique)"
    git config user.name "$nom"
    git config user.email "$($nom.ToLower() -replace '[^a-z0-9]', '')@users.noreply.github.com"
}

# Certaines empreintes ont des noms très longs : sans ça, Windows refuse de
# créer les fichiers au-delà de 260 caractères de chemin.
git config core.longpaths true

# Premier lancement sur ce poste : si la récupération initiale s'est arrêtée
# en route (chemins trop longs), des fichiers manquent. On les remet au lieu
# d'envoyer leur suppression à tout le monde.
if (-not (git config kicadsync.initialise)) {
    git reset -q
    $manquants = @(git ls-files --deleted)
    if ($manquants.Count -gt 0) {
        git checkout -q HEAD -- $manquants
        Info "$($manquants.Count) fichier(s) manquant(s) remis en place."
    }
    git config kicadsync.initialise oui
}

$branche = git symbolic-ref --short HEAD
$date = Get-Date -Format 'yyyy-MM-dd HH:mm'

# --- 1) Enregistrer ses propres modifs --------------------------------------
git add -A 2> $null

# Garde-fou : beaucoup de fichiers disparus d'un coup, c'est presque toujours
# un accident (dossier déplacé, récupération incomplète), pas un vrai ménage.
$supprimes = @(git diff --cached --name-only --diff-filter=D)
if ($supprimes.Count -gt 10) {
    git reset -q
    Erreur "$($supprimes.Count) fichiers ont disparu du dossier (ex. : $(Split-Path $supprimes[0] -Leaf))."
    Erreur "Par sécurité, rien n'a été envoyé. Préviens Rafaël."
    Fin 1
}

git diff --cached --quiet
if ($LASTEXITCODE -ne 0) {
    $resume = (git diff --cached --name-only | ForEach-Object { Split-Path $_ -Leaf }) -join ', '
    if ($resume.Length -gt 60) { $resume = $resume.Substring(0, 57) + '...' }
    git commit -q -m "Modifs de $nom le $date : $resume"
    Ok "Tes modifications sont enregistrées."
} else {
    Info "Aucune modification de ton côté."
}

# --- 2) Récupérer celles du collègue, 3) envoyer -----------------------------
# Deux essais : si le collègue envoie pile entre notre récupération et notre
# envoi, GitHub refuse et il suffit de recommencer.
for ($essai = 1; $essai -le 2; $essai++) {
    Info "Récupération de la dernière version sur GitHub..."
    git fetch -q origin
    if ($LASTEXITCODE -ne 0) {
        Erreur "Impossible de joindre GitHub (connexion internet ? accès au dépôt ?)."
        Fin 1
    }

    git rev-parse --verify -q "origin/$branche" *> $null
    if ($LASTEXITCODE -eq 0) {
        $avant = git rev-parse HEAD
        git rebase -q "origin/$branche" *> $null
        if ($LASTEXITCODE -ne 0) {
            # Vous avez modifié le même fichier tous les deux : on ne mélange pas
            # deux schémas automatiquement (risque de fichier KiCad cassé).
            # On met ta version de côté sur GitHub et on prend celle du collègue.
            git rebase --abort *> $null
            $secours = "sauvegarde-$($nom -replace '[^A-Za-z0-9]', '')-$(Get-Date -Format 'yyyyMMdd-HHmm')"
            git branch $secours
            git push -q origin $secours *> $null
            git reset -q --hard "origin/$branche"
            Erreur "CONFLIT : ton collègue et toi avez modifié le même fichier."
            Erreur "Ton dossier contient maintenant SA version."
            Erreur "La tienne est sauvegardée sur GitHub dans la branche '$secours'."
            Erreur "Mettez-vous d'accord sur quoi garder avant de continuer."
            Fin 2
        }
        if ($avant -ne (git rev-parse HEAD)) { Ok "Modifications du collègue récupérées." }
        else { Info "Rien de nouveau côté collègue." }
    }

    git push -q -u origin $branche *> $null
    if ($LASTEXITCODE -eq 0) {
        Ok "Tout est à jour sur GitHub."
        Write-Host ""
        Write-Host "Derniers changements :" -ForegroundColor White
        git --no-pager log -5 --format="  %ad  %s" --date=format:"%d/%m %H:%M"
        Fin 0
    }
    Info "GitHub a refusé l'envoi (le collègue vient d'envoyer ?), nouvel essai..."
}
Erreur "Envoi impossible. Vérifie que tu as accepté l'invitation GitHub au dépôt"
Erreur "(dans tes emails), puis relance SYNC.bat."
Fin 1

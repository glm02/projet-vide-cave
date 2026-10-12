$ErrorActionPreference = 'Stop'
$projectRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..')).TrimEnd('\')
$journalDir = $PSScriptRoot
function Inside-Workspace([string]$relativePath) {
    $absolutePath = [System.IO.Path]::GetFullPath((Join-Path $projectRoot $relativePath))
    if (-not $absolutePath.StartsWith($projectRoot + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Chemin hors du dossier autorisé : $absolutePath"
    }
    return $absolutePath
}
$moves = [System.Collections.Generic.List[object]]::new()
function Add-Move([string]$source, [string]$destination) {
    $moves.Add([pscustomobject]@{ancien=$source;nouveau=$destination})
}
Add-Move 'Carte_vide_cave_PIC_Allegro_TRIAC' '03_Electronique\Carte_vide_cave_PIC_Allegro_TRIAC'
Add-Move 'G206A_CAMEL_TP_CAO' '03_Electronique\G206A_CAMEL_TP_CAO'
Add-Move 'kicad' '03_Electronique\Projets_KiCad_initiaux'
Add-Move 'maquette_3d' '04_Maquette_3D'
Add-Move 'PROJET VIDE CAVE' '05_Notes\PROJET VIDE CAVE'
Add-Move 'Claude outputs' '06_Archives_et_exports\Claude outputs'
Add-Move 'Carte_vide_cave_PIC_Allegro_TRIAC.zip' '06_Archives_et_exports\Carte_vide_cave_PIC_Allegro_TRIAC.zip'
foreach ($file in Get-ChildItem -LiteralPath $projectRoot -Force -File) {
    if ($file.Name -match '^(\.\$)?Synoptique') {
        Add-Move $file.Name ('02_Synoptiques\' + $file.Name)
    } elseif ($file.Extension -eq '.pdf') {
        $destFolder = if ($file.Name -eq 'pickit_connexion.pdf') {'03_Electronique\Documentation'} else {'01_Documents\Consignes'}
        Add-Move $file.Name ($destFolder + '\' + $file.Name)
    } elseif ($file.Extension -eq '.docx') {
        Add-Move $file.Name ('01_Documents\Rapport\' + $file.Name)
    } elseif ($file.Extension -eq '.pptx') {
        Add-Move $file.Name ('01_Documents\Presentation\' + $file.Name)
    } elseif ($file.Extension -eq '.ods') {
        Add-Move $file.Name ('01_Documents\Planning\' + $file.Name)
    }
}
# Verify every resolved source and target before moving any directory tree.
foreach ($move in $moves) {
    $srcPath=Inside-Workspace $move.ancien
    $dstPath=Inside-Workspace $move.nouveau
    if (-not (Test-Path -LiteralPath $srcPath)) {throw "Source introuvable : $srcPath"}
    if (Test-Path -LiteralPath $dstPath) {throw "Destination existe déjà : $dstPath"}
}
$inventory = @(Get-ChildItem -LiteralPath $projectRoot -Force -Recurse -File | ForEach-Object {
    [pscustomobject]@{chemin=$_.FullName.Substring($projectRoot.Length+1);octets=$_.Length;sha256=(Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash}
})
$inventory | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $journalDir 'inventaire_avant.json') -Encoding utf8
$moves | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $journalDir 'deplacements.json') -Encoding utf8
foreach ($move in $moves) {
    $srcPath=Inside-Workspace $move.ancien
    $dstPath=Inside-Workspace $move.nouveau
    $parentDir=Split-Path -Parent $dstPath
    New-Item -ItemType Directory -Path $parentDir -Force | Out-Null
    Move-Item -LiteralPath $srcPath -Destination $dstPath
}
# Preserve the path used by the Blender window already open during this task.
$aliasPath=Inside-Workspace 'maquette_3d'
$blendTarget=Inside-Workspace '04_Maquette_3D'
New-Item -ItemType Junction -Path $aliasPath -Target $blendTarget | Out-Null
$aliasItem=Get-Item -LiteralPath $aliasPath -Force
$aliasItem.Attributes=$aliasItem.Attributes -bor [System.IO.FileAttributes]::Hidden
if ((Get-Item -LiteralPath $blendTarget -Force).Attributes -band [System.IO.FileAttributes]::Hidden) {
    throw 'Le dossier principal de la maquette doit rester visible.'
}
$checked=0
foreach ($item in $inventory) {
    $expected=$item.chemin
    foreach ($move in $moves) {
        if ($item.chemin -eq $move.ancien -or $item.chemin.StartsWith($move.ancien + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
            $expected=$move.nouveau + $item.chemin.Substring($move.ancien.Length)
            break
        }
    }
    $newPath=Inside-Workspace $expected
    if (-not (Test-Path -LiteralPath $newPath -PathType Leaf)) {throw "Fichier perdu : $expected"}
    if ((Get-FileHash -LiteralPath $newPath -Algorithm SHA256).Hash -ne $item.sha256) {throw "Contenu modifié lors du déplacement : $expected"}
    $checked++
}
[pscustomobject]@{date=(Get-Date).ToString('o');deplacements=$moves.Count;fichiers_verifies=$checked;contenu_conserve=$true;alias_blender='maquette_3d -> 04_Maquette_3D (masqué)'} | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $journalDir 'verification_deplacements.json') -Encoding utf8
Write-Output "Rangement effectué : $($moves.Count) déplacements, $checked fichiers conservés et vérifiés par SHA-256."

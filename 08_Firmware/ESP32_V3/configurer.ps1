# Écrit wifi.local.h (ignoré par git) : réseau Wi-Fi et bot Telegram des alertes.
$ErrorActionPreference = 'Stop'
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
$fichier = Join-Path $PSScriptRoot 'wifi.local.h'

function Lire-Masque([string]$invite) {
  $s = Read-Host $invite -AsSecureString
  $b = [Runtime.InteropServices.Marshal]::SecureStringToBSTR($s)
  try { [Runtime.InteropServices.Marshal]::PtrToStringBSTR($b) } finally { [Runtime.InteropServices.Marshal]::ZeroFreeBSTR($b) }
}

function Chaine-C([string]$v) { '"' + $v.Replace('\', '\\').Replace('"', '\"') + '"' }

Write-Host 'Réseau Wi-Fi 2,4 GHz (un partage de connexion du téléphone convient ; pas eduroam).'
$ssid = Read-Host 'Nom du Wi-Fi'
$motDePasse = Lire-Masque 'Mot de passe Wi-Fi'

Write-Host ''
Write-Host 'Bot Telegram : @BotFather, /newbot, puis copier le jeton. Laisser vide pour ignorer.'
$jeton = Lire-Masque 'Jeton du bot'
$chatId = ''
if ($jeton) {
  $api = "https://api.telegram.org/bot$jeton"
  try { $moi = Invoke-RestMethod "$api/getMe" -TimeoutSec 15 }
  catch { Write-Host "Jeton refusé par Telegram ($($_.Exception.Response.StatusCode.value__)). Rien n'est écrit."; exit 1 }
  Write-Host "Bot reconnu : @$($moi.result.username)"
  Write-Host "Envoie un message (par exemple /start) à @$($moi.result.username) depuis ton téléphone. J'attends 2 minutes…"
  $fin = (Get-Date).AddMinutes(2)
  while (-not $chatId -and (Get-Date) -lt $fin) {
    $maj = Invoke-RestMethod "$api/getUpdates?timeout=20" -TimeoutSec 30
    $dernier = $maj.result | Where-Object { $_.message } | Select-Object -Last 1
    if ($dernier) { $chatId = [string]$dernier.message.chat.id }
  }
  if (-not $chatId) { Write-Host "Aucun message reçu. Rien n'est écrit."; exit 1 }
  Invoke-RestMethod "$api/sendMessage" -Method Post -ContentType 'application/json; charset=utf-8' -Body ([Text.Encoding]::UTF8.GetBytes((@{ chat_id = $chatId; text = 'Vide-cave : ce chat recevra les alertes du moteur.' } | ConvertTo-Json))) | Out-Null
  Write-Host 'Conversation trouvée, message de confirmation envoyé.'
}

$contenu = @"
#pragma once

constexpr char WIFI_SSID[] = $(Chaine-C $ssid);
constexpr char WIFI_PASSWORD[] = $(Chaine-C $motDePasse);
constexpr char TELEGRAM_TOKEN[] = $(Chaine-C $jeton);
constexpr char TELEGRAM_CHAT_ID[] = $(Chaine-C $chatId);
"@
[IO.File]::WriteAllText($fichier, $contenu, (New-Object Text.UTF8Encoding $false))
Write-Host "Écrit : $fichier"

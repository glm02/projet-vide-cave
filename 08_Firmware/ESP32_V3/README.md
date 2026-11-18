# ESP32 V3 : supervision du vide-cave

L'ESP32 reçoit l'état de la pompe envoyé par le PIC, mesure le climat (SHT31-D),
garde l'historique, sert le site de supervision et envoie une alerte Telegram
quand le moteur est bloqué.

Carte : ESP32 Dev Module (CP210x). GPIO25 SDA, GPIO26 SCL, GPIO16 RX, GPIO17 TX.
SHT31-D à l'adresse 0x44.

## Mise en route

1. `powershell -ExecutionPolicy Bypass -File configurer.ps1` : Wi-Fi 2,4 GHz et
   bot Telegram (saisie masquée, jeton vérifié). Écrit `wifi.local.h`, ignoré par git.
2. Compiler et téléverser (schéma de partition `min_spiffs`, l'appli dépasse 1,2 Mo) :

   ```
   arduino-cli compile --fqbn esp32:esp32:esp32:PartitionScheme=min_spiffs .
   arduino-cli upload -p COM13 --fqbn esp32:esp32:esp32:PartitionScheme=min_spiffs .
   ```
3. Le moniteur série (115 200 bauds) affiche l'adresse du site :
   `http://<ip>/` ou `http://vide-cave.local/`.

## Site

| Vue | Contenu |
|---|---|
| État | état de la pompe, courant ADC, climat, bandeau rouge si moteur bloqué |
| Historique | 1 h (un point toutes les 10 s) ou 24 h (un point par minute) |
| Événements | 40 derniers événements, filtre alertes, bouton d'alerte de test |

API JSON : `GET /api/etat`, `GET /api/historique?plage=1h|24h`,
`GET /api/evenements`, `POST /api/test-alerte`, `GET /telemetrie`.

L'historique 24 h et les événements sont enregistrés en flash (LittleFS) toutes
les 10 minutes et à chaque événement : ils survivent à une coupure.
Le site n'est joignable que sur le réseau local.

## Alertes Telegram

| Événement | Message |
|---|---|
| Passage en `DEFAUT` | « MOTEUR BLOQUÉ » avec l'ADC |
| Toujours en défaut | rappel toutes les 30 min |
| Sortie du défaut | « Défaut acquitté » |
| Aucune trame du PIC pendant 1 min | alerte de liaison |

## Liaison avec le PIC

UART 19 200 bauds, 8N1, fin de ligne LF ou CRLF.

| Sens | Format | Exemple |
|---|---|---|
| ESP32 vers PIC | `TH,<°C>,<%HR>` ou `TH,ERR` | `TH,21.25,58.30` |
| PIC vers ESP32 | `PUMP,<ARRET/MARCHE/DEFAUT>,<ADC 0 à 4095>` | `PUMP,MARCHE,2080` |

Le PIC garde le contrôle de la pompe et publie son état au moins toutes les
secondes. Après 5 s sans trame valide, l'état devient inconnu.

## Test sans le PIC

Les mêmes trames tapées dans le moniteur série USB sont prises en compte :
`PUMP,DEFAUT,3900` simule un blocage, `TH,12.5,70` un climat (pendant 1 min),
`TEST` envoie une alerte de test.

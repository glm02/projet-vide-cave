---
tags: [vide-cave, code, memo, debutant]
date: 2026-11-04
---
# Le LCD et MCC : ce qui se passe derrière les appels

Retour : [[14 - Memo code - Commencer ici]]

Commence par savoir utiliser les fonctions. Les détails des masques et de
l'envoi au LCD peuvent être relus plus tard. Un pilote est le code qui sait
communiquer avec un composant ; l'application décide quand et pourquoi l'utiliser.

## 1. Les fonctions du LCD que tu appelles dans ton programme

| Appel | Ce qu'il demande |
|---|---|
| `LCD_Initialize();` | Préparer les broches et l'écran |
| `LCD_Clear();` | Effacer l'affichage |
| `LCD_Home();` | Revenir au début et rétablir la position de l'affichage |
| `LCD_GoTo(2, 1);` | Placer la prochaine écriture au début de la deuxième ligne |
| `LCD_WriteText("Bonjour");` | Écrire une suite de caractères |
| `LCD_WriteData('A');` | Envoyer un seul caractère |

Dans ce pilote, `GoTo` numérote les lignes à partir de 1. Une colonne adressable
par le contrôleur n'est pas forcément une colonne visible sur ton écran.

**Petit exercice :** que fait cet extrait ?

```c
LCD_Clear();
LCD_GoTo(1, 1);
LCD_WriteText("Test");
```

Il efface l'écran, place le curseur au début de la première ligne et écrit `Test`.

## 2. Pourquoi un `lcd.h` et un `lcd.c` ?

`lcd.h` annonce les fonctions disponibles et leurs paramètres. `lcd.c` contient
leur fonctionnement. La protection `#ifndef LCD_H`, `#define LCD_H`, `#endif`
évite de lire plusieurs fois les mêmes déclarations si plusieurs fichiers
incluent cet en-tête.

Le pilote local indique être repris de la partie LCD de `libsae22`. Il est
adapté à un projet MCC et ne faut pas le confondre avec toute la bibliothèque SAE22.

## 3. Les noms des broches et les registres

Dans `lcd.c`, `LCD_RS_SetHigh()` donne un nom lisible à une écriture sur RA1.
Les autres noms font la même chose pour RW, E et les quatre lignes de données.

| Nom dans le pilote | Rôle prévu |
|---|---|
| RS | Choisir une commande ou un caractère |
| RW | Choisir lecture ou écriture ; ce pilote utilise l'écriture |
| E | Valider le transfert |
| D4 à D7 | Transporter quatre bits de données à la fois |

`SetHigh` écrit 1, `SetLow` écrit 0, `Toggle` inverse la valeur et `GetValue`
lit l'entrée correspondante. Ces noms décrivent une action sur une broche.

Un registre est une petite case de réglage à l'intérieur du PIC. Dans ce code,
`LATA` porte les valeurs de sortie du port A ; `TRISA` choisit entrée ou sortie ;
`ANSELA` choisit l'usage analogique ou numérique.

## 4. Les commandes du LCD sont aussi des nombres

```c
#define LCD_CLEAR 0x01
#define LCD_DDRAM_SET 0x80
```

`0x` indique une écriture hexadécimale : `0x01` vaut 1, `0x80` vaut 128.
Le pilote donne un nom aux nombres compris par le LCD.
Les familles `ENTRY`, `DISPLAY`, `SHIFT` et `FUNCTION` regroupent les réglages
de déplacement, d'affichage, de décalage et de mode de communication.
On n'a pas besoin d'apprendre toutes leurs valeurs pour afficher un texte.

## 5. `LCD_WriteHighNibble()` : envoyer quatre bits

Un octet contient huit bits. Un « nibble » en contient quatre : un demi-octet.
Cette fonction prépare la moitié haute du nombre sur les quatre lignes de données.

Dans ce câblage, D6 et D7 sont reliées dans un ordre inversé sur RA7 et RA6.
Les lignes `bit6`, `bit7` et `x` échangent donc les bits 6 et 7 avant l'écriture.

- `&` conserve les positions autorisées par un masque.
- `|` combine des bits.
- `^` donne 1 quand les deux bits sont différents : c'est le « ou exclusif ».
- `<<` et `>>` déplacent les bits.

```c
LCD_LAT = (LCD_LAT & 0x0F) | (data & 0xF0);
```

Cette ligne garde les quatre bits bas du port et remplace ses quatre bits hauts
par ceux des données. Elle évite d'écraser les autres signaux du port.
La fonction encadre le transfert par les niveaux de E et un court délai.

Ce bloc est du détail de pilote : on peut le comprendre progressivement sans
le recopier dans chaque fonction de l'application.

## 6. `LCD_SoftReset()`

Il envoie plusieurs séquences de démarrage avec des attentes, puis demande
le mode 4 bits. Son but est de remettre l'interface LCD dans un état connu.
Ce reset logiciel du LCD n'est pas le reset du PIC.

## 7. `LCD_WriteCommand()` et `LCD_WriteData()`

Ces fonctions envoient chacune un octet en deux morceaux : les quatre bits
hauts, puis les quatre bits bas déplacés vers le haut avec `<< 4`.

La différence est le signal RS : à 0 pour une commande, à 1 pour un caractère.
RW reste à 0 pour écrire. Les attentes du pilote laissent à l'écran le temps
de traiter l'envoi ; elles ajoutent du temps à la boucle qui l'appelle.

## 8. `LCD_WriteText()` : avancer dans le texte

```c
while (*msg != 0)
{
    LCD_WriteData(*msg);
    msg++;
}
```

`msg` indique où se trouve le caractère en cours dans la mémoire : c'est un pointeur.
`*msg` lit ce caractère. `msg++` avance au caractère suivant.
Un texte C se termine par un caractère de valeur zéro, appelé `\0` : la boucle
s'arrête lorsqu'elle le rencontre. Le caractère de fin n'est pas affiché.

Dans `"Salut"`, on envoie donc S, a, l, u, t, puis on s'arrête au zéro final.

## 9. `LCD_GoTo()` : calculer la position

La fonction choisit l'adresse de départ de la ligne, ajoute `column - 1`,
garde les bits d'adresse avec `& 0x7F`, puis envoie une commande de position.

La ligne du pilote utilise `? :` pour choisir l'adresse : c'est une écriture
compacte de `if / else`. Pour la lire, imagine ceci :

```c
if (line == 1)
{
    addr = 0x00;
}
else
{
    addr = 0x40;
}
```

Les valeurs d'adresse appartiennent au fonctionnement de ce pilote.
On vérifiera les lignes et colonnes demandées plutôt que d'envoyer n'importe quel nombre.

## 10. `LCD_Clear()`, `LCD_Home()` et `LCD_Initialize()`

`Clear` et `Home` sont des fonctions courtes qui envoient chacune la commande
portant le même nom.

`Initialize` prépare RA1 à RA7 en sorties numériques tout en conservant le bit 0
des réglages existants. Les lignes `ANSELA &= 0x01` et `TRISA &= 0x01` appliquent
un masque : elles ne forcent pas à elles seules RA0 en entrée analogique.
Ce réglage de RA0 doit donc déjà être correct dans MCC.

La fonction attend ensuite, réalise le soft reset, sélectionne le mode 4 bits
et deux lignes, active l'affichage, efface l'écran puis règle l'avancée du curseur.
C'est pourquoi elle doit être appelée après la préparation générale du PIC.

## 11. `SYSTEM_Initialize()` : le point d'entrée MCC

Dans le `system.c` lu, cette fonction appelle, dans l'ordre :

| Appel | Rôle |
|---|---|
| `CLOCK_Initialize()` | Régler l'horloge |
| `PIN_MANAGER_Initialize()` | Régler les ports et les options des broches |
| `ADC_Initialize()` | Préparer le convertisseur analogique-numérique |
| `EUSART_Initialize()` | Préparer la liaison série |
| `INTERRUPT_Initialize()` | Préparer les éléments d'interruption générés |

Il n'y a pas d'initialisation Timer2 dans cette liste actuelle.
Le nom `SYSTEM_Initialize` ne signifie donc pas que tous les périphériques
possibles du PIC sont automatiquement configurés.

## 12. L'horloge et les broches

Le `clock.c` lu demande un oscillateur interne à 16 MHz. `clock.h` fournit aussi
la valeur utilisée pour calculer les délais. Cette déclaration doit correspondre
à l'horloge réellement configurée ; écrire seulement `_XTAL_FREQ` ne règle pas
l'horloge matérielle.

Le gestionnaire des broches écrit les registres au démarrage. Les macros
`LED_SetHigh()` ou `OPTO_TRIAC_SetLow()` permettent ensuite d'utiliser les
noms choisis dans MCC. Pour modifier durablement la configuration, on reprendra
les réglages dans MCC avant de générer, plutôt que de disperser les modifications.

## 13. Le pilote du CAN

`ADC_Initialize()` prépare ses registres. Dans le fichier lu, le commentaire et
la valeur de `ADCON1` indiquent une horloge CAN FOSC/2 et le format signe/valeur
absolue. Le test efficace demande FOSC/16 dans son commentaire : les réglages
réels sont donc à reprendre et à vérifier avec la documentation.

`ADC_GetConversion()` sélectionne les entrées, attend le temps d'acquisition,
lance une conversion avec `GO_nDONE`, attend qu'elle se termine, puis assemble
`ADRESH` et `ADRESL`. C'est une lecture bloquante : l'appel ne rend son résultat
qu'après la fin de conversion.

Les autres fonctions du pilote permettent de faire séparément ces étapes :
sélectionner les voies, lancer, tester la fin et récupérer le résultat.
Le programme actuel utilise surtout l'appel qui les regroupe.

## 14. La liaison série et `printf`

L'UART/EUSART envoie des caractères vers un autre appareil. `EUSART_Initialize`
la configure, `EUSART_IsTxReady` indique si l'envoi est possible, `EUSART_Write`
écrit un caractère dans le registre d'émission.

La fonction `putch()` du pilote attend que l'émission soit prête puis appelle
`EUSART_Write`. Elle permet à `printf` de sortir son texte par cette liaison.
Les fonctions `getch()` et `EUSART_Read()` concernent le sens inverse : recevoir.
Les gestionnaires d'erreurs traitent les problèmes de réception ; ce ne sont
pas des étapes de la décision moteur.

Une longue sortie série prend du temps. C'est pour cela que le test envoie
son résumé après l'acquisition, au lieu d'envoyer du texte entre chaque point.

## 15. Le fichier d'interruptions

Il prépare ici une interruption externe et un mécanisme de rappel : le pilote
peut appeler une fonction qu'on lui a indiquée. Une fonction de rappel s'appelle
aussi « callback ».

Ce fichier ne contient pas, à lui seul, le traitement Timer2 du projet final.
On ne doit pas dire « j'ai une interruption Timer2 » simplement parce qu'un
fichier `interrupt.c` existe. Le test efficace utilise la scrutation d'un drapeau.

## 16. Et `libsae22.c` ?

Cette bibliothèque contient aussi de l'initialisation, de la liaison série,
des commandes PWM et des fonctions de mesure de période. La configuration
MPLAB lue ne la liste pas dans les sources compilées de ce projet MCC.
Les fonctions de même nom ne doivent pas être ajoutées deux fois sans vérifier
leur rôle et leur sélection dans le projet.

Pour notre reprise, on commence par les fonctions réellement appelées par
l'application. On détaillera les autres périphériques seulement si on les utilise.

## Ce que je dois savoir expliquer maintenant

- [ ] La différence entre l'application et un pilote.
- [ ] À quoi servent les quatre appels LCD les plus utilisés.
- [ ] Pourquoi une déclaration dans un `.h` ne remplace pas le code du `.c`.
- [ ] Le rôle de la configuration MCC et des noms de broches.
- [ ] Pourquoi un délai, une conversion et un affichage occupent du temps.

Vérification du format CAN avec la [fiche PIC16F1788/9, section 17](https://ww1.microchip.com/downloads/en/DeviceDoc/40001675C.pdf).
Les autres descriptions de cette fiche reprennent le contenu des pilotes locaux lus.

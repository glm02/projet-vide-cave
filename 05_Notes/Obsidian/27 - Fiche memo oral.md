---
tags: [vide-cave, oral, memo]
date: 2026-11-08
---
# Fiche mémo : ce que je peux dire à l'oral

Vide-cave · SAé 31/42 · Groupe 311

**Le message à faire passer :** on surveille la pompe par son courant, le 230 V reste isolé du 5 V, et on sait exactement ce qui est fait et ce qui reste à faire.

## 1. Ce que je dis, dans l'ordre

10 minutes à deux, environ 5 minutes chacun. Puis 10 à 20 minutes de questions.

**Présentation (20 s)**

> Bonjour, nous sommes [prénom] et [prénom], du groupe 311. Notre projet porte sur la surveillance d'une pompe de vide-cave. On va présenter le besoin, les composants, le code et ce qu'il nous reste à faire.

**Le problème (45 s)**

> Une pompe de vide-cave peut rester longtemps à l'arrêt, dans une cave où on ne va pas tous les jours. Si elle est bloquée, on risque de le découvrir quand la cave est inondée. Notre objectif, c'est de connaître l'état du moteur sans descendre, juste en mesurant son courant. C'est prévu pour une maison individuelle.

**Le cahier des charges (45 s)**

> Le système mesure le courant du moteur et reconnaît trois états : en marche, bloqué ou à vide. Il laisse passer le pic de courant du démarrage, coupe le moteur en cas de défaut et affiche l'état sur un LCD. La pompe doit se mettre en marche tous les trois jours. La contrainte principale, c'est que le 230 V doit rester isolé du 5 V.

**Le principe (30 s)**

> Le courant dépend de ce que le moteur doit entraîner. Trop fort, il force : il est bloqué. Trop faible, il tourne sans eau : il est à vide. Attention au démarrage : un moteur tire beaucoup de courant au début, c'est normal, donc on attend quelques secondes avant de décider.

**Le synoptique (45 s)**

> À gauche le 230 V, avec la pompe et le triac. À droite le 5 V, avec le PIC. Trois composants font le lien sans contact électrique : le capteur de courant, l'optotriac et l'optocoupleur. Le capteur envoie sa tension sur RA0, et le PIC commande le moteur par RB2.

**Les changements depuis la pré-étude (30 s)**

> On est passés du relais au triac, qui n'a pas de pièce qui s'use. On a ajouté un optocoupleur pour repérer le passage par zéro. L'ESP32 reste pour plus tard. Et on repart de la carte de la SAE22, qu'on modifie.

**Les composants (45 s)**

> On n'a pas vraiment eu à les chercher : ils nous ont été donnés par les profs. Nous, on a regardé à quoi ils servent et comment les câbler.

**La lecture analogique (1 min)**

> Le capteur donne une tension, mais le PIC ne comprend que des nombres. C'est le CAN qui transforme la tension en nombre. Pour tester, on a utilisé le potentiomètre de la carte SAE22. Le PIC envoyait le nombre au PC, on le lisait sur RealTerm et on notait les valeurs en tournant le potentiomètre. La valeur allait de 9 à 255. On a coupé cette plage en trois zones, ce qui nous donne deux seuils de test : 91 et 173.

**Le code (1 min)**

> On lit le courant, on garde une valeur de 0 à 255 et on la compare aux deux seuils. Si le courant est normal, le compteur repart de zéro. S'il reste anormal pendant 10 secondes, on coupe le moteur. Comme ça, un pic isolé ne coupe pas la pompe. Pour le temps, on utilise une interruption du Timer2 toutes les 100 ms. Le programme suit trois états : démarrage, marche et défaut.

**Le bilan (1 min)**

> Ce qui marche aujourd'hui, c'est la lecture analogique et les seuils, testés au potentiomètre. On n'a pas encore de résultat avec le vrai capteur et le 230 V, parce que la carte n'est pas imprimée. Il nous reste à finir le schéma, imprimer la carte, tester bloc par bloc et régler les vrais seuils.

**Les retards (30 s)**

> On a perdu du temps à chercher des composants sur internet alors qu'ils étaient à l'IUT, et on a dû reprendre des schémas. On rattrape en avançant bloc par bloc : capteur et triac, puis optocoupleur, puis LED et LCD. La date de fin ne change pas.

**La conclusion (30 s)**

> Je me suis occupé des composants, des broches du PIC et du code. J'ai surtout appris la lecture analogique, qu'on n'avait pas encore vue en cours. Si c'était à refaire, je demanderais tout de suite aux profs quels composants sont disponibles. Merci, nous sommes prêts pour vos questions.

## 2. Les composants en une phrase

| Composant | Ce que je dis |
|---|---|
| **PIC16F1788** | Le microcontrôleur : il lit le courant, décide et commande le moteur. |
| **ACS756** | Il transforme le courant en tension par effet Hall, sans contact avec le secteur : 2,5 V à 0 A, plus 20 mV par ampère. |
| **BTA12-600** | Le triac : l'interrupteur qui laisse passer ou coupe le 230 V. |
| **MOC3023** | L'optotriac : le PIC allume une LED, la lumière déclenche le triac. Il s'amorce à 5 mA au maximum. |
| **6N136** | L'optocoupleur : il prévient le PIC quand la tension passe par zéro. |
| **LCD 16 × 2** | Il affiche l'état du moteur, en mode 4 bits. |

**Ne pas confondre :** le MOC3023 va **du PIC vers le 230 V**, le 6N136 va **du 230 V vers le PIC**.

## 3. Les chiffres à connaître

| Chiffre | Ce que c'est |
|---|---|
| 230 V efficaces, 325 V crête | Le secteur (230 × √2) |
| 20 ms | Une période à 50 Hz |
| 2,5 V + 20 mV/A | La sortie du capteur |
| 12 bits | Le CAN du PIC : de 0 à 4095 |
| 8 bits | Ce que je garde : de 0 à 255 |
| 19,5 mV | Un point en 8 bits (5 V / 256) |
| 1,22 mV | Un point en 12 bits (5 V / 4096) |
| 9 à 255 | Ce qu'on a lu avec le potentiomètre |
| 91 et 173 | Mes seuils de test : (255 − 9) / 3 = 82 |
| 100 ms | La mesure et la décision |
| 5 s | Le démarrage toléré |
| 10 s | Un défaut confirmé avant de couper |

## 4. Mon code, morceau par morceau

```c
Courant = ADC_GetConversion(Capteur_COURANT, negChannel_ADNREF) >> 8;
```

> Je lis la tension sur RA0. Le `>> 8` divise par 256, donc j'ai une valeur de 0 à 255.

```c
if(Courant >= SEUIL_BAS && Courant <= SEUIL_HAUT)    // courant normal
{
    Compteur = 0;
}
else if(Compteur >= 100)    // anormal depuis 10 s
{
    OPTO_TRIAC_SetLow();    // on coupe le moteur
}
```

> Si le courant est normal, je remets le compteur à zéro. Sinon, il monte. À 100, soit 10 secondes, je coupe.

```c
void IT_Timer2(void)      // appelée toutes les 100 ms
{
    Top = 1;
}
```

> L'interruption est courte : elle prévient juste le `main`, qui fait le travail.

| Mot | Ce qu'il veut dire |
|---|---|
| `uint8_t` | Entier sans signe sur 8 bits, de 0 à 255 |
| `uint16_t` | Entier sans signe sur 16 bits |
| `volatile` | La variable change dans l'interruption, il faut la relire à chaque fois |
| `#define` | Donne un nom à un nombre |
| `>> 8` | Décale de 8 bits, comme une division par 256 |
| `&&` | Et |
| `GIE`, `PEIE` | Autorisent les interruptions et celles des périphériques |

## 5. Les questions probables

### Sur le projet

**Pourquoi le courant ?**
> Il dépend de ce que le moteur doit entraîner. Avec une seule mesure, on détecte le blocage et la marche à vide.

**Pourquoi un triac et pas un relais ?**
> Il n'a pas de pièce qui s'use, et le PIC le commande à travers un optotriac.

**Pourquoi ces composants ?**
> Ils nous ont été donnés par les profs. On a vérifié qu'ils conviennent : 12 A pour le triac, 400 V crête pour l'optotriac.

**Comment est assurée la sécurité ?**
> Le 230 V et le 5 V ne sont reliés par aucun fil. La protection en amont, comme le disjoncteur, n'est pas dans notre projet d'après le professeur. Et on fait les essais sans secteur d'abord.

### Sur la mesure

**C'est quoi le CAN ?**
> Le convertisseur analogique numérique : il transforme une tension en nombre.

**Combien de bits ?**
> 12 bits sur ce PIC. J'en garde 8, ça suffit pour le test au potentiomètre.

**Et avec le vrai capteur ?**
> Il donne 20 mV par ampère. En 8 bits, un point vaut environ 1 A, c'est trop juste. Il faudra passer en 12 bits, où un point vaut 61 mA.

**Un capteur de 100 A, ce n'est pas trop ?**
> Si, mais c'est celui qu'on nous a donné. Un capteur plus petit donnerait plus de signal, c'est une amélioration possible.

**Tu lis une seule valeur alors que le courant est alternatif ?**
> Oui, c'est la limite de cette version de test. La suite, c'est de faire plusieurs mesures sur une période de 20 ms. Le 6N136 sert à savoir quand les démarrer.

### Sur le code

**Pourquoi une interruption ?**
> Pour que le PIC ne reste pas bloqué dans une attente. Il mesure toutes les 100 ms et reste libre entre deux.

**Pourquoi l'interruption est-elle si courte ?**
> Elle coupe le programme principal. Elle met juste un drapeau à 1, et le `main` fait le travail.

**Que fait MCC ?**
> Il configure l'horloge, les broches et le CAN. Moi, j'écris la logique.

**Le programme compte les 3 jours ?**
> Pas encore. Il faudra ajouter un état d'arrêt et compter les heures jusqu'à 72.

### Sur l'optocoupleur

**Comment marche le 6N136 ?**
> Dedans, il y a une LED et un transistor séparés. Sur l'alternance positive, la LED s'allume et le transistor passe la sortie à 0 V. Le PIC a ainsi un repère toutes les 20 ms.

**Il est branché ?**
> Il est prévu sur le schéma, mais pas encore relié au PIC ni testé.

### Sur l'organisation

**Qui a fait quoi ?**
> Moi : les composants, les broches du PIC et le code. Jules : KiCad, le schéma et les typons. On a beaucoup travaillé ensemble sur KiCad.

**Comment vous avez travaillé à deux ?**
> GitHub pour les fichiers KiCad, Google Drive pour les documents, et un planning pour suivre les tâches.

**Quelle a été la plus grosse difficulté ?**
> Le temps perdu sur le choix des composants, parce qu'on n'avait pas demandé aux profs ce qui était disponible. Et la connexion du programmateur SNAP.

## 6. Les pistes d'amélioration

- Un capteur plus petit, pour avoir plus de signal.
- Le réglage du zéro au démarrage.
- La mesure du déphasage, pour mieux reconnaître la marche à vide.
- Un dissipateur sur le triac et un boîtier étanche.
- Des boutons de marche forcée et de remise à zéro.
- Un ESP32 pour envoyer une alerte par mail.

## 7. Si je bloque

**Je ne sais pas répondre :**
> Je ne l'ai pas encore étudié, je préfère vérifier dans la datasheet plutôt que de dire une bêtise.

**Ce n'est pas encore fait :**
> Ce n'est pas encore fait : c'est prévu après l'impression de la carte.

**Je n'ai pas compris la question :**
> Vous pouvez reformuler, s'il vous plaît ?

## 8. À ne pas dire

- **« MOC3041 » :** c'est le **MOC3023**.
- **« 130 V » :** c'est **230 V**.
- **« Le CAN fait 8 bits » :** il fait **12 bits**, j'en garde 8.
- **« Ça marche avec la pompe » :** pas encore testé.
- **« Le 6N136 est branché » :** il est seulement prévu.
- **« Le programme compte les 3 jours » :** pas encore codé.

## 9. À vérifier avant de partir

- [ ] Les noms sur la première diapo
- [ ] La version du Timer2 : si le projet n'est pas généré en Classic, dire « réglé dans le code » et ne pas montrer `IT_Timer2`
- [ ] Ce que j'ai vraiment vu en testant le programme sur la carte
- [ ] Le fichier des diapos ouvert sur le PC de la salle, avec une copie PDF
- [ ] Une répétition chronométrée à voix haute

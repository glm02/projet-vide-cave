---
tags: [vide-cave, ameliorations, dossier]
date: 2026-11-05
---
# Idées d'améliorations

Retour : [[00 - Accueil]] · Dossier : [[13 - Questions pour rediger le dossier de conception]] · Tâches : [[05 - Taches]]

Les idées sont rangées par partie du projet. Pour chacune : ce qu'on changerait, pourquoi, et si c'est **facile**, **moyen** ou **gros travail**. Les ⭐ sont celles qui apportent le plus pour un effort raisonnable.

À la fin, un paragraphe prêt à adapter pour le dossier (questions 27, 33 et 34).

---

## 1. La mesure du courant

- ⭐ **Prendre un capteur adapté au courant de la pompe** — *moyen*
  L'ACS756 est un modèle **100 A** : il donne 20 mV par ampère. Notre pompe tire environ 1 à 1,5 A, donc le signal ne bouge que de 20 à 30 mV autour de 2,5 V. C'est très peu pour distinguer « à vide » et « en charge ».
  Avec un capteur 5 A comme l'**ACS712-05B** (185 mV/A), le même courant donne environ 9 fois plus de signal. Les seuils seraient plus faciles à régler et les mesures plus fiables.

- ⭐ **Faire un réglage du zéro au démarrage** — *facile*
  À courant nul, le capteur ne sort pas exactement 2,5 V. Au démarrage, avant d'allumer la pompe, le PIC mesure cette valeur et la retire de toutes les mesures suivantes. On gagne en précision sans rien changer au matériel.

- **Mesurer la vraie valeur efficace sur plusieurs périodes** — *moyen*
  Grâce au 6N136 qui détecte le passage par zéro, on peut faire plusieurs mesures par période et calculer une valeur efficace sur 5 ou 10 périodes au lieu d'une seule. Ça lisse les variations.

- **Utiliser le déphasage pour reconnaître la marche à vide** — *gros travail*
  À vide, un moteur a un déphasage plus grand qu'en charge. En mesurant le temps entre le zéro de la tension (6N136) et le zéro du courant, on aurait un deuxième indice en plus de l'intensité. C'est l'idée de départ du projet, mais elle demande des essais avec la vraie pompe.

## 2. La carte et la partie puissance

- ⭐ **Ajouter un dissipateur sur le triac BTA12** — *facile*
  Sans dissipateur, le triac monterait vers 97 °C dans une cave à 25 °C. Avec un petit dissipateur à clipser, il resterait vers 50 °C.

- **Ajouter un fusible temporisé** (par exemple T3,15 A) — *facile*
  Pour protéger contre un court-circuit. Temporisé, pour ne pas sauter au pic de démarrage du moteur.

- **Ajouter un réseau RC (snubber) aux bornes du triac** — *facile*
  Seulement si notre BTA12 n'est pas une version « snubberless ». Le moteur est une charge inductive : sans ce réseau, le triac peut se redéclencher tout seul à la coupure.

- **Mettre la carte dans un boîtier étanche** — *moyen*
  Une cave peut être humide, voire inondée. Un boîtier fermé, avec des presse-étoupes pour les câbles, protège la carte et l'utilisateur.

- **Séparer clairement la partie 5 V et la partie 230 V sur le PCB** — *moyen*
  Une zone pour la commande, une zone pour le secteur, avec un espace entre les deux. C'est déjà le principe avec l'optotriac et l'optocoupleur, mais on peut le rendre visible sur le typon.

## 3. Le programme

- **Fait :** ⭐ **Organiser le programme en états** (démarrage, marche, à vide, bloqué, arrêt) — *moyen*
  Avec une variable d'état et un `switch`, chaque état a son propre code et ses conditions pour passer au suivant. C'est plus clair, plus facile à tester, et ça colle directement à l'organigramme du dossier.

- **Fait :** ⭐ **Remplacer les attentes par des interruptions et un timer** — *moyen*
  Au lieu d'attendre 5 secondes sans rien faire, un timer compte le temps pendant que le PIC continue de mesurer et d'afficher.

- **Ajouter un « chien de garde » (watchdog)** — *facile*
  Si le programme plante, le PIC redémarre tout seul au lieu de laisser la pompe allumée ou bloquée.

- **Garder les seuils dans l'EEPROM du PIC** — *moyen*
  On pourrait régler les seuils avec les boutons et les garder même après une coupure de courant, sans reprogrammer le PIC.

- **Détecter une pompe qui tourne trop longtemps** — *facile*
  Si la pompe reste en charge bien plus longtemps que d'habitude, c'est peut-être une grosse arrivée d'eau ou une fuite. Le système pourrait le signaler.

## 4. L'affichage et les commandes

- ⭐ **Afficher l'état du moteur sur le LCD** — *facile*
  « En marche », « À vide », « BLOQUÉ », avec le courant mesuré et le temps avant le prochain cycle.

- **Bouton de marche forcée et bouton de remise à zéro** — *facile*
  Marche forcée pour lancer un cycle sans attendre les 3 jours. Remise à zéro pour acquitter une alarme.

- **Utiliser le buzzer et la LED pour les alarmes** — *facile*
  Bip et LED qui clignote quand le moteur est bloqué : on le remarque même sans regarder l'écran.

- **Garder un petit historique** — *moyen*
  Nombre de cycles, nombre de fois où il y avait de l'eau, dernière alarme. Utile pour savoir si la cave prend l'eau souvent.

## 5. La communication (ESP32)

- ⭐ **Envoyer une alerte quand le moteur est bloqué** — *gros travail*
  C'est l'objectif de départ : prévenir l'utilisateur sans qu'il descende à la cave. L'ESP32 reçoit l'état du PIC par la liaison série et envoie un email ou une notification sur le téléphone.

- **Envoyer un résumé après chaque cycle** — *moyen*
  « Cycle du 5/11 : de l'eau pompée pendant 2 min, tout va bien. » On sait que le système marche, même quand il n'y a pas de problème.

- **Une petite page web sur le réseau de la maison** — *gros travail*
  Pour voir l'état, l'historique et lancer une marche forcée depuis le téléphone.

## 6. Ce qu'on ferait autrement (organisation)

- **Demander tout de suite aux professeurs les composants disponibles**, avant de chercher sur internet. On a perdu du temps à choisir des composants qui étaient déjà dans la salle, puis à modifier les schémas KiCad.
- **Relever la plaque de la pompe dès le début** (tension, puissance, courant). Tout le reste en dépend : choix du capteur, seuils, dissipateur.
- **Tester chaque bloc sur plaque d'essai avant de faire le typon** : capteur seul, puis triac, puis optocoupleur.
- **Garder une seule version à jour du schéma** sur GitHub, pour ne pas se retrouver avec plusieurs versions différentes du même fichier.

---

## Paragraphe prêt à adapter pour le dossier

> Pour la suite, on a plusieurs améliorations en tête. La plus importante, ce serait de changer de capteur de courant : l'ACS756 qu'on a est fait pour 100 A, alors que notre pompe ne tire qu'environ 1 A, donc le signal est très faible. Un capteur 5 A, comme l'ACS712, donnerait beaucoup plus de précision. Côté programme, on voudrait l'organiser en états (démarrage, marche, à vide, bloqué) avec des interruptions, pour ne plus attendre sans rien faire. On veut aussi afficher l'état du moteur sur le LCD, ajouter les boutons de marche forcée et de remise à zéro, et utiliser le buzzer pour les alarmes. Enfin, l'ESP32 permettrait d'envoyer une alerte par email quand le moteur est bloqué, ce qui était l'objectif de départ du projet. Pour une version réelle, il faudrait aussi ajouter un dissipateur sur le triac, un fusible et un boîtier étanche, parce que la carte se trouve dans une cave.

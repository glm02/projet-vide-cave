---
tags: [vide-cave, dossier, conception, questions]
date: 2026-11-04
---
# Questions pour rédiger le dossier de conception

Retour : [[00 - Accueil]]

**Cette fiche sert à préparer la rédaction du dossier à partir de tes réponses.**

Réponds comme tu parles : quelques phrases, des mots-clés ou une réponse dictée
suffisent. On utilisera ensuite tes réponses pour rédiger les paragraphes, en
corrigeant la langue et en gardant ta façon de t'exprimer.

Tu peux commencer uniquement par l'introduction et la partie 1. Si tu ne sais
pas encore répondre, écris « à voir ». Pour KiCad, tu peux compléter plus tard.
Précise simplement ce qui est réalisé, ce qui a été testé et ce qui est prévu.
Aucune réponse n'est remplie à ta place.

## Introduction

### 1. En quelques mots, qu'est-ce qu'on cherche à réaliser dans ce projet ?

**Ta réponse :**
Dans notre projet, on cherche à réaliser une pompe vide cave sans flotteur qui permettrait de juste en mesurant le courant définir l'état du moteur s'il est passant, bloqué ou marche à vide. Tous les trois jours, il se mettrait en marche pour vider l'eau de la cave s'il y en a, sinon il se mettrait en arrêt et il enverrait un message si le moteur est bloqué.

### 2. Quel problème veut-on résoudre avec ce système ?

**Ta réponse :**
Pour ce système, le problème qui résout, c'est le fait de connaître l'état du moteur, savoir s'il est avide, bloqué ou passant. Et dans les futures améliorations, on voulait mettre un envoi d'email ou autre pour vérifier que le moteur marche ou non et créer une alerte si le moteur était bloqué. Donc ça répondra au problème de si le vide cave s'arrête, vu que c'est dans une cave et que ce n'est pas accessible facilement. Ça permettrait que l'utilisateur ait directement l'information sans avoir à besoin de descendre.

### 3. À qui le système doit-il servir, et dans quelle situation ?

**Ta réponse :**
Ce système sert pour les utilisateurs domestiques ménagers et pas forcément peut-être pour les professionnels mais plus pour dans les maisons individuelles ou autres.

### 4. Quels étaient les principaux objectifs demandés au début du projet ?

**Ta réponse :**Les principaux objectifs au début du projet étaient de définir les matériaux utilisés, le fonctionnement des différentes cartes, faire un test V1 pour d'abord juste voir comment allaient marcher les lectures analogiques pour les capteurs de courant et autres. Et voilà.



## 1. Présentation du projet, fonctionnement et outils

### 5. Comment le système doit-il fonctionner, du démarrage jusqu'à l'arrêt ? Explique-le avec tes mots.

**Ta réponse :**
Le système quand il démarre il actionne la pompe. S'il voit que le courant d'appel est trop fort, ça veut dire que le moteur est bloqué. S'il voit que le courant d'appel est fort, mais pendant 5 secondes par exemple, au final, ça montrera que juste c'est le démarrage du moteur, puis après il marchera. S'il est à vide, on mesura la différence entre le déphasage et de la tension du courant. Et s'il marche normalement, on le laisse marcher jusqu'à ce qu'il vide l'eau, jusqu'à ce qu'il soit à vide. Et si par exemple il est bloqué, ça envoie une information. Si il est en marche, ça envoie aussi une information pour dire qu'il y a de l'eau. Et s'il est à vide, ça l'arrête juste et voilà, c'est tout, il ne se passe rien de plus.

### 6. Quelles informations le système doit-il mesurer, et que doit-il faire avec ces informations ?

**Ta réponse :** Alors ce système il va mesurer du coup le courant d'entrée dans la pompe, notamment grâce à un capteur de courant Allegro qui sert à moduler de passer d'un courant en tension. Du coup, sa tension à peu près en sortie équivalent, du coup c'est 1 ampère et c'est égal à tant de millivolts. Je crois que c'est 20 millivolts. Voilà.


### 7. Quelles sont les grandes parties du système ? À quoi sert chacune ?

**Ta réponse :** Les grandes parties du système sont du coup la carte électronique qui sert de microcontrôleur et qui va notamment commander un Triac qui va permettre de commander la pompe pour l'actionner ou non.Le Triac va actionner la pompe, la carte électronique va vérifier le courant et si le courant est trop élevé ou non, il va décider de couper ou non la pompe à l'aide du Triac.Et après le système l'hôte c'est la pompe générale avec du coup le moteur de la pompe et voilà.


### 8. Quels outils avez-vous utilisés, et pour faire quoi ? Par exemple : KiCad, MPLAB X, MCC, un logiciel de dessin ou de simulation.

**Ta réponse :** Alors du coup on s'est aidé de plusieurs logiciels, on a utilisé MPLab pour coder une grande partie du code, on a utilisé Kikad notamment pour faire le PCB et le schéma de notre carte et on a utilisé aussi un kanban excel 


### 9. Quels choix avez-vous changés depuis la pré-étude, et pourquoi ?

**Ta réponse :**
Alors du coup ce qu'on a changé depuis la préétude, au départ on devait partir pour un relais sauf que c'était pas bon, du coup on est parti sur un triac. On a aussi rajouté un octo coupleur pardon, un octo triac pardon, non un octocoupleur pour mesurer du coup les temps où la phase du courant passe à 0, passe au maximum, puis repasse à 0, puis repasse au minimum, puis repasse à 0, pour déclencher les mesures de courant et du coup avoir les bonnes valeurs pour mesurer la valeur efficace et la valeur max pour mieux mesurer le déphasage. Donc ça ça a été une amélioration. Après dans les futures améliorations et qu'est-ce qu'on a changé depuis, on va rajouter aussi des boutons, des LED pour montrer l'état du moteur et aussi notamment écrire sur un enfin afficher sur un affiche LCD l'état du moteur s'il est bloqué, passant ou autre.


## 2. Organisation et gestion du projet

### 10. Comment vous êtes-vous réparti le travail dans le binôme ? Tu peux écrire « moi » et « mon binôme ».

**Ta réponse :**
Comment s'est réparti le travail en binôme? Au final, on a quand même beaucoup travaillé ensemble, notamment sur le Kikad. Moi j'aimais bien faire la recherche de produits, du coup moi c'est Raphaël, la recherche de composants, tout ce qui est la partie technique pour savoir quel composant on allait utiliser. J'ai aussi fait quand même pas mal parties code et Kamel s'est occupé de la partie KICAD, schéma et notamment l'intégration. Il a fait plusieurs cartes typons avec une carte avec juste le capteur de courant, le triac et l'opto triac et l'opto coupleur qui mesurera les phases. J'ai aussi regardé sur le pic 16F1788 pour voir quelle entrée on venait utiliser pour les lectures analogiques, quelle sorte on utilisait pour commander le triac ou autre.

### 11. Quelles ont été les grandes étapes du travail, dans quel ordre ?

**Ta réponse :** Tout d'abord définir le cahier des charges, ça a été notre premier gros partie, mais ça c'était avant le rapport de préétude. Une fois qu'on a eu ça, on a fait la version zéro de notre projet avec juste le capteur qui mesurait le courant et un triac qui mesurait du coup qui permettait de déclencher de déclencher l'ouverture ou la fermeture pour le circuit pour alimenter la pompe ou non. Et après l'autre étape, ça a été aussi de rajouter du coup un opto-coupleur pour mesurer le courant au bon endroit et déclencher les mesures de courant au bon endroit. On a aussi rajouté des LED et un afficheur LCD sur notre deuxième partie. Pour du coup, ce sera pour notre carte finale. Pour une fois qu'on aura tout soudé et autres pour tout tester. Là pour l'instant on fait bloc par bloc. C'est-à-dire qu'on commence juste par le capteur de courant et le triac. Après, on en rajoutera l'optocoupleur et ensuite après on rajoutera les LEDs, les LCD, tout ça. Et à venir l'ESP32, mais pour l'instant, on se tient à la version 0 et 1.


### 12. Qu'est-ce qui était prévu au départ, et qu'avez-vous réellement fait jusqu'à maintenant ?

**Ta réponse :**
Du coup au départ on avait juste prévu de faire le capteur de courant et l'opto triac. Enfin d'abord on avait commencé par un relais puis au final on s'est rendu compte que c'était pas bon. Du coup on va rajouter aussi notre capteur pour déclencher les mesures de courant au bon moment. Ça, ça a été une grosse partie. Ensuite, on s'est rendu compte qu'il nous manquait quand même pas mal de temps. Notamment sur le code où on est un petit peu en retard, mais ça devrait le faire. Après, je me suis occupé de la partie lecture analogique, notamment du traitement de signal analogique, parce qu'en fait on ne l'avait pas encore fait en cours. Mais du coup, moi j'ai pu apporter ça et comprendre comment l'utiliser et configurer avec MCC Configurator dans MPLab.

### 13. Quelles difficultés vous ont fait perdre du temps ? Comment avez-vous essayé de les résoudre ?

**Ta réponse :**
On s'est quand même pas mal éparpillés, notamment sur le choix des composants, parce qu'en fait on cherchait des composants sur internet, mais au final ils étaient dans la salle de cours. Et notre erreur a été de ne pas demander directement les composants aux professeurs et ça nous a fait perdre pas mal de temps parce que du coup à la fin on devait changer les KikaD et autres, vérifier que c'était les bonnes valeurs de résistance et autres. Donc ça ça a été un peu plus long, mais au final on s'est quand même bien débrouillé. A la fin on avait quand même une idée claire de où on allait, quelle carte on allait faire et on a bien avancé là-dessus.

### 14. Comment partagez-vous les fichiers et suivez-vous les tâches ?

**Ta réponse :**
Pour partager les fichiers, on utilisait GitHub qui permettait du coup de synchroniser les fichiers Kikad entre deux PC différents et deux utilisateurs différents. Notamment on utilisait le Git push, Git Commit et Git main pour savoir un peu où en était l'autre, qu'est-ce qu'il avait fait dessus. On utilisait aussi beaucoup de canals de communication pour parler, discuter, savoir où on en était. On avançait chez nous les week-ends notamment. Sur la partie code, moi je lui ai envoyé à peu près ce que j'étais, ce que j'en avais besoin. Et aussi pour tout ce qui était PIN du pic, savoir quel pin on utilisait pour que lui sur Kikad il sache directement l'entrée et sortie. On utilise aussi Google Drive for the documents partagés. Notamment on a tout rassemblé, toute la documentation technique et tout ça, on a tout rassemblé, donc ça a permis aussi de bien aider ça.


## 3. Conception électronique et schéma — à compléter quand tu seras prêt

### 15. Êtes-vous partis d'une carte existante ou d'un schéma créé par vous ? Pourquoi ?

**Ta réponse :**
Du coup on est parti d'une carte existante, on est parti de la carte de la SAE 22, la SAE Robot. Notamment cette carte elle nous permettait du coup de contrôler un hashard d'un robot. Nous on a gardé les liaisons sérieuses, le contrôleur, certainement LED, la lecture de courant analogique, notamment grâce à un potentiomètre. Du coup, je me suis amusé à coder, enfin je me suis amusé, j'ai vérifié et codé un programme pour faire de la lecture analogique sur réel terme. Ça nous permettait de voir et tester les lectures analogiques, de savoir quel seuil on aurait besoin, notamment le seuil minimum et le seuil max, pour voir un peu quel potentiel on pouvait mettre pour notre 8 bits. Après cette carte, on va la modifier, du coup on va virer quelques composants et on va les garder certains. Notamment on va enlever la liaison avec le hacheur. Du coup c'est les TTL je crois. Mais on gardera certaines LEDs, deux LEDs, l'afficheur LCD, mais sinon on va enlever quand même pas mal de liaisons. Et vu qu'il y a pas mal de liaisons au pic qui n'étaient pas reliés, ça nous permettra du coup derrière de faire tout ce qui était notre avantage pour la lecture analogique ou autre qui n'était pas reliée.

### 16. Quels composants principaux avez-vous retenus, et quel est le rôle de chacun ?

**Ta réponse :**
Du coup la plupart des composants qu'on a retenus c'est le capteur de courant, le triac, l'optocoupleur, le pic 16F 1788, tout ce qui était afficheur LCD et autres. Et ce qui nous manque maintenant, pardon, et le rôle de chacun, je le définirai plus tard.

### 17. Qu'avez-vous ajouté, retiré ou modifié sur la carte de départ ?

**Ta réponse :**

Ça, ce qu'on a modifié et retiré dans la carte, je l'ai déjà dit avant, mais du coup, voilà. Je pense que cette question, j'ai déjà répondu.
### 18. Quels calculs avez-vous faits pour choisir les valeurs des composants ? Explique ce que tu sais déjà, sans chercher à tout rédiger.

**Ta réponse :**
On n'a pas eu à faire énormément de calculs, à part pour le triac, l'autocoupleur et le reste sinon la plupart c'était des calculs de valeur ou autre pour les 8 bits et tout ça, et sinon des calculs de résistance. Après on s'est expérimenté, on s'est quand même inspiré de pas mal de schémas sur internet, notamment ceux pour le triac, parce qu'on n'en avait jamais vu. Du coup ça permettait qu'il y avait déjà des valeurs qui étaient affichées, mais après bon voilà, il faut toujours adapter et vérifier pour notre composant à nous.

### 19. Qu'avez-vous prévu pour l'alimentation, la commande du moteur et la protection du montage ? Qu'est-ce qui reste à vérifier ?

**Ta réponse :**

La commande du moteur du coup elle va se réaliser par le Triac. Pour l'alimentation, ce sera sur un réseau de 130 volts domestiques. Et la protection du montage, le professeur nous avait indiqué qu'elle était déjà qu'on la prenait pas en compte, qu'elle était déjà faite, comme s'il y avait déjà un disjoncteur et tout ça. Mais surtout la protection du montage, elle va être faite par notre Triac qui va mesurer si le courant est trop fort, ça va couper du coup l'alimentation. Donc ça, ça sera aussi du coup une protection qui sera rajoutée dans notre montage.

## 4. Nomenclature et matériel

### 20. De quels composants et matériels avez-vous besoin pour réaliser le système ?

**Ta réponse :**Du coup pour la réalisation des composants tout ça j'ai déjà dit la liste de matériel qu'on avait besoin et je la ressortirai s'il faut mais c'est pas trop grave et ce qu'on a à disposition on a déjà pas mal de composants que le prof nous a donné il nous manque presque rien et voilà donc on a fait que de la récup presque on n'a pas vraiment à acheter de choses pour l'instant il y aura juste à faire la carte et c'est tout


### 21. Qu'est-ce que vous avez déjà à disposition, et qu'est-ce qu'il manque encore ?

**Ta réponse :**


### 22. Quelles références ou valeurs restent à choisir ?

**Ta réponse :**
Les références qui restent à choisir c'est pour les résistances et les condensateurs, notamment pour tout ce qui est triac et autres, sinon on a déjà fait pas mal de choses mais après ça je verrai avec mon collègue, mais normalement on n'est pas trop mal.


## 5. Programme et organigrammes

### 23. Qu'est-ce que le programme doit faire, expliqué simplement, sans écrire de code ?

**Ta réponse :**
Du coup le programme il doit vérifier entre certains seuils, notamment que je vais définir grâce à des define. Je define certains seuils pour dire voilà ça c'est l'état haut, c'est lequel on ne doit pas dépasser pendant plus de 5 secondes, sinon ça veut dire qu'il y a une surtension sur le moteur, le moteur force et il n'arrive plus à avancer donc il est en il y a quelque chose qui le bloque. Donc ça va du coup c'est une partie de notre code. Après on a fait aussi une autre partie où si ça dépasse le seuil pendant 5 secondes puis après ça redescend, le courant redescend, donc là c'est bon, juste il est en état de marche, mais quand il démarre il y a un pic de courant, donc c'est normal. Ça, ça sera du coup expliqué aussi dans le code, notamment par du coup la commande du Triact qui sera faite aussi. J'ai configuré tout par MCC, tous les ports par MCC. Après, on a fait aussi, on va faire aussi afficher en fonction du moteur l'état du moteur sur un LCD. Donc ça c'est pas très compliqué, on a utilisé un case break, voilà pour faire simple.

### 24. Quelles sont les grandes étapes du programme ? Dans quelles situations passe-t-il d'une étape à une autre ?

**Ta réponse :**
Du coup la première étape c'est le démarrage, après il y a la marche, puis après il y a l'arrêt. Donc au démarrage le moteur va faire un pic de courant parce qu'il appelle pour démarrer, il appelle pas mal de courant. Après une fois qu'il est démarré, s'il continue de forcer, ça veut dire que le moteur est bloqué donc il faut le couper. Donc ça ferait l'arrêt, l'autotriaque qui commande. Sinon pendant l'étape de marche, on va dire soit il est à vide, soit il est en marche. S'il est à vide, on le coupe ensuite. S'il est en marche, on le laisse tourner jusqu'à ce qu'il soit à vide pour le couper ensuite.

### 25. Quelles parties du programme as-tu déjà écrites et essayées ?

**Ta réponse :**
Du coup pour les parties du programme que j'ai déjà écrites c'est la lecture analogique notamment avec le potentiomètre sur l'ancienne carte. Je me suis inspiré de ça pour ensuite l'adapter quand on aura enfin la carte Typon. Et cette carte après elle utilisera la lecture analogique pour mesurer le courant et aussi déclencher les mesures de courant grâce à l'autocoupleur.

### 26. Comment le programme utilise-t-il les mesures pour reconnaître un fonctionnement normal ou un défaut ?

**Ta réponse :**
Il utilise du coup les mesures du capteur de courant et pour reconnaître le fonctionnement il utilise aussi la durée donc on utilisera des interruptions pour permettre au programme de faire d'autres choses en même temps et de ne pas attendre forcément 5 secondes sans rien faire. Si jamais le moteur, le courant dépasse un certain seuil qui est vraiment énorme, là on coupe automatiquement. Mais si ça dépasse plus de 5 secondes, par exemple mettons à 3 ampères pour qu'il démarre, puis après il revient à 1 A à son niveau nominal, là c'est bon, on le laissera continuer de démarrer, mais si ça dépasse trop pendant trop longtemps, là on coupe. Et si ça dépasse vraiment énormément, là on coupe direct. Et ça, ça permettra du coup de mesurer en fonction de l'état du moteur et de l'afficher ensuite.

### 27. Quelles parties veux-tu simplifier, et qu'est-ce qu'il reste à développer ?

**Ta réponse :**



## 6. Essais et résultats

### 28. Quels essais avez-vous réellement réalisés ? Avec quel matériel ?

**Ta réponse :**
Du coup pour les essais on a réalisé une lecture de courant analogique sur l'ancienne carte. Donc ça ça m'a permis d'utiliser la fonction analogique et de la comprendre. On a utilisé RealTermes pour visualiser les valeurs. On les a mis dans un tableau pour voir un peu comment ils répondait. Ça, ça nous a permis du coup de voir les différentes fonctions analogiques, les utiliser. Et après, une fois qu'on avait ça, j'ai utilisé ça pour faire un programme. Ou si je tournais à fond le potentiomètre, ça allumait le LED. Et si je tournais à l'autre fond, ça alimentait une autre LED. Donc ça permettait du coup d'avoir deux seuils et de faire pouvoir faire commande marche.

### 29. Pour chaque essai, qu'est-ce que tu attendais et qu'est-ce que tu as observé ?

**Ta réponse :**

Du coup pour les essais j'ai quand même pas mal galéré, notamment avec la reprise et qu'on n'a pas trop de support, mais du coup ça permet de nous débrouiller plus par nous-mêmes et d'apprendre par nous-mêmes. Sinon c'était... Le manque de support c'était un peu le truc le plus dur, mais après il faut s'apprendre à se débrouiller vu que c'est un projet où on part de zéro, on ne peut pas vraiment avoir de support. Et sinon on a dû faire appel aux professeurs pour la plupart des aides sur le piquet avec la connexion snap et voilà.
### 30. Qu'est-ce qui fonctionne aujourd'hui ? Qu'est-ce qui ne fonctionne pas encore ou donne un résultat incertain ?

**Ta réponse :**

Ce qui fonctionne aujourd'hui c'est la lecture de courant analogique. Pour l'instant du coup on n'a pas encore de résultat de carte parce que du coup on l'a pas encore réalisé, enfin on l'a pas encore imprimé plutôt, mais une fois qu'on l'aura imprimé on pourra tester réellement avec les mesures de courant, avec du 230 volts et avec notre capteur qu'on a besoin. Pour du coup je pourrais définir le seuil max et minimum. Et ça nous permettra après de faire tout ce qui est commande optotriac et tout ça.
### 31. Quels essais reste-t-il à faire pour vérifier les objectifs du projet ?

**Ta réponse :**



## Conclusion

### 32. À ce stade, qu'avez-vous réussi à réaliser par rapport aux objectifs de départ ?

**Ta réponse :**
At the start on two cart electronic, one type and V2. After we have a cell final who for instance on just a little bit but not encouraged. And on a bon bout de code with two composants that we have choised. Il nous reste juste à imprimer la carte, tester avec le vrai capteur et finir le code avec ce nouveau capteur et l'adapter plutôt.

### 33. Qu'est-ce qu'il reste à terminer, et quelles seront les prochaines étapes ?

**Ta réponse :**

Les prochaines étapes elles seront d'imprimer la carte, faire les essais avec le capteur de courant et une fois qu'on aura fait ça, on pourra du coup rajouter toutes les améliorations qu'on voulait, notamment afficher sur le lecteur LCD, un bouton pour faire de la marche forcée, un bouton pour faire de la remise à zéro, un ESP32 pour faire la connexion par mail et tout ça. Donc bref, voilà.
### 34. Qu'as-tu appris pendant ce projet ? Qu'est-ce que tu ferais autrement si tu recommençais ?

**Ta réponse :** 



## Illustrations et annexes

### 35. Quelles photos peux-tu fournir pour montrer le travail réalisé ? Que voit-on sur chacune ?

**Ta réponse :**


### 36. Quels schémas, synoptiques, organigrammes, calculs ou résultats veux-tu joindre au dossier ?

**Ta réponse :**


### 37. Y a-t-il une difficulté, une décision ou une réalisation importante qu'on n'a pas encore évoquée ?

**Ta réponse :**



## Pour transformer les réponses en dossier

Une fois une partie remplie, on rassemble tes idées dans l'ordre du dossier.
On garde les faits que tu donnes, on te signale les points qui manquent et on
ajoute les légendes des illustrations. Tu relis ensuite le texte pour vérifier
qu'il correspond bien à votre travail.


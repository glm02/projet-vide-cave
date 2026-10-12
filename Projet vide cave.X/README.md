# Projet C principal : vide-cave

Ouvrir ce dossier avec `File → Open Project` dans MPLAB X. Cible : **PIC16F1788**, compilateur **XC8 3.10**, programmateur **MPLAB SNAP PG164100**. Le choix du SNAP et de son numéro de série se fait dans `Project Properties → Hardware Tools` ; le fichier partagé n'impose pas de numéro de série personnel.

**Éditer [main.c](main.c)**, visible sous `Source Files`. Si le projet était déjà ouvert pendant l'ajout, le fermer puis le rouvrir. Ce fichier contient le premier test LED, ses initialisations et ses bits de configuration. À la vérification du dossier, la configuration MCC était enregistrée, mais aucun fichier C n'y avait encore été sauvegardé ; le fichier de départ a été ajouté sans écraser de source existante.

Résultat attendu : D4, pilotée par RB0, clignote toutes les 500 ms. RB2 reste à zéro. Le test a été compilé avec le Makefile de ce projet ; il n'a pas été exécuté sur la carte ni programmé par l'assistant.

Le fichier `.mc3` créé dans MCC est conservé. Pour ce test manuel, ne pas ajouter un deuxième `main()` ou des bits de configuration générés en double. Lors du passage à MCC, choisir une seule source de configuration et intégrer le code utilisateur dans les emplacements prévus.

**Le SNAP ne fournit pas le 5 V à la carte** : alimentation externe sur J1, avec secteur et moteur déconnectés pour le premier essai. Brancher ses contacts ICSP 1 à 5 vers J4 1 à 5, en vérifiant les repères de broche 1 : MCLR, VDD de détection, GND, PGD, PGC. Garder `LVP = ON`. Un ancien PIC déjà configuré LVP OFF doit d'abord être rétabli avec un outil supportant la programmation haute tension.

`Clean and Build Project` compile ; `Make and Program Device` remplace le programme dans le PIC. Le HEX compilé se trouve dans `dist/default/production/Projet_vide_cave.X.production.hex`. Les dossiers de compilation et réglages privés ne sont pas versionnés.

[Guide détaillé C, brochage et SNAP](../08_Firmware/Test_LED_PIC16F1788/README.md) · [Fiche Microchip SNAP](https://www.microchip.com/en-us/development-tool/PG164100).

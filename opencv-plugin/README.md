# Bibliothèque contenant les algorithmes opencv
## Interface Unity
Pour construire la bibliothèque avec l'interface unity faire `make`


## Automate
Pour construire l'automate et la bibliothèque sans interface unity faire `make automate`

### Dépendances
`gengetopt`, générateur de parseur d'arguments
### Aide
Lancer `./automate --help` pour un récapitulatif des options et `./automate --detailed-help` pour des détails sur les options


Utilisation simple:  
`./automate -i <dossier d'entré> [-o <dossier sortie (défaut dossier courrant)>] [-a <action>]`  
Exemple (génère les cartes de disparité des images de ../screens/ et les places dans tmp/ avec le préfix _):  
`./automate -i ../screens -o tmp -a sgbm -p '_'`
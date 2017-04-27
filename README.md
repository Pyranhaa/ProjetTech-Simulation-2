# ProjetTech-Simulation
Simulation sur Unity3D

Ce dossier contient le projet Unity. Le projet nécessite le plugin du contrôleur robot.

# Plugin Unity
Le plugin `unity_controler.so` fait l'interface entre la scène Unity et le contrôleur du robot. Il est chargé par Unity puis utilisé par des scripts.

## Construire le plugin
Les sources se trouvent dans le répertoire `opencv-plugin`. Se placer dans ce répertoire et lancer `make` pour construire le plugin, le contrôleur et l'automate

# Automate
L'automate permet d'appliquer nos algorithmes sur des images. Faire `./automate --detailed-help` pour les descriptions des options.

# Contrôleur
Le contrôleur est une bibliothèque dynamique à lier au robot. Les fichiers qui le composent sont:
- robot_controler.cpp
- robot_controler.h
- processor.cpp
- processor.hpp

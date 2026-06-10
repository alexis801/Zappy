# Semaine 1 — du 25 au 31 mai 2026

## Ce qui a été fait

### Environnement de travail prêt
- Le dépôt GitHub est en place. À chaque proposition de changement, il vérifie tout seul que le code compile encore et que les tests passent.
- Une règle empêche de pousser n'importe quoi directement sur la version officielle : il faut toujours passer par une Pull Request avec les vérifications au vert.
- Quand une PR est acceptée, le code part automatiquement vers le dépôt de rendu de l'école.
- Le fichier qui compile les trois programmes du projet (`zappy_server`, `zappy_gui`, `zappy_ai`) est prêt.

### Toute la doc est posée
- 30 issues créées, une par fonctionnalité demandée par le sujet, avec à chaque fois une description simple et la liste de ce qui doit marcher.
- Cahier des charges écrit en mots simples.
- Diagrammes qui montrent les grandes parties du projet et comment elles communiquent.
- Page d'accueil (README) ajoutée par Alexis.
- Arborescence des dossiers créée pour que chacun travaille dans son coin.

### Le code commence
- Les valeurs du jeu (temps de vie, densités de ressources, conditions pour monter de niveau...) sont posées dans des fichiers de constantes.
- La lecture des réglages du serveur au lancement (port, taille de la carte, équipes, vitesse) est terminée et testée.
- La base réseau du serveur (accepter plusieurs joueurs en même temps, sans tourner dans le vide) est en place en fin de semaine.

## Équipe — répartition décidée

| Personne | Sa partie du projet |
|---|---|
| Gotennku (Patrick) | Serveur — réseau |
| Dorone Atlan | Serveur — monde et règles du jeu |
| Florian Hémery | Serveur — commandes des joueurs et protocole graphique |
| Alexis (alexis) | IA — survivre et récolter |
| Nathan | IA — stratégie d'équipe |
| kyliangi (Kylian) | Interface graphique |

Côté contributions cette semaine : Alexis a fait le README et l'arborescence des dossiers.

## La suite

- Finir la poignée de main (handshake) : quand un joueur se connecte, le serveur lui dit bonjour, lit son équipe et lui répond.
- Commencer la gestion du temps des actions.
- Que les cinq autres membres commencent vraiment à coder leur partie.

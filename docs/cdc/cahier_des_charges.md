# Cahier des charges - Zappy

## C'est quoi ce projet ?

Zappy est un jeu en réseau. Plusieurs équipes vivent sur une planète (une carte) et s'affrontent. Le but de chaque équipe : faire monter ses habitants au niveau le plus élevé. La première équipe qui réunit 6 joueurs au niveau maximum gagne.

Le projet est fait de trois programmes :
- un **serveur**, le cœur du jeu, qui connaît la carte et applique les règles ;
- une **interface graphique**, pour regarder la partie en direct ;
- un **joueur automatique**, une intelligence qui joue toute seule.

## Comment ca marche ?

- Le serveur crée la carte, fait apparaître de la nourriture et des pierres, et fait respecter les règles.
- Les joueurs (automatiques) se connectent au serveur et lui envoient des ordres : avancer, tourner, regarder autour, ramasser une pierre, parler… Le serveur répond à chacun.
- Pour rester en vie, un joueur doit manger. Pour monter de niveau, il doit réunir des pierres et d'autres joueurs du même niveau sur la même case, puis lancer un rituel (l'incantation).
- L'interface graphique se connecte aussi au serveur pour montrer la partie en 2D.
- **Tout passe par le serveur** : les clients ne se parlent jamais directement.

## Comment on le lance

```
./zappy_server -p port -x largeur -y hauteur -n equipe1 equipe2 ... -c joueurs_par_equipe -f vitesse
./zappy_gui -p port -h machine
./zappy_ai -p port -n equipe -h machine
```

## Les regles

- La carte boucle sur elle-même : sortir par la droite ramène par la gauche (pareil en haut/bas).
- Chaque action prend du temps, calculé avec la vitesse `f` (par exemple avancer = 7/f secondes).
- Un joueur démarre avec 10 nourritures. 1 nourriture = 126 unités de temps de vie. Sans nourriture, il meurt.
- Pour monter de niveau, il faut le bon nombre de joueurs du même niveau **et** les bonnes pierres réunis sur une même case.
- La première équipe avec 6 joueurs au niveau 8 gagne la partie.
- Le serveur est un seul programme, un seul fil d'exécution, et n'attend jamais dans le vide (il surveille avec `poll`).

## Ce qu'on utilise

- **Serveur** : C, C++ ou Rust (ici C++).
- **Interface graphique** : C++ avec la bibliothèque SFML.
- **Joueur automatique** : langage libre (ici C++).
- **Communication** : sockets réseau (TCP).

## Ce qu'on doit livrer ?

- Les trois programmes : `zappy_server`, `zappy_gui`, `zappy_ai`.
- Un Makefile qui compile les trois.
- Le respect du protocole de communication imposé (côté joueur et côté interface graphique).
- Un code conforme au coding style C++ d'Epitech.

# Semaine 3 — semaine du 8 juin 2026 (en cours)

## Ce qui a été fait

Côté serveur :
- File d'attente des commandes par joueur : chaque joueur a sa propre file de 10 commandes maximum. Quand il envoie une commande, elle se range avec son heure prévue d'exécution. Quand l'heure arrive, le serveur l'exécute. Les joueurs ne se bloquent jamais entre eux.
- Le serveur dort exactement le temps qu'il faut avant la prochaine commande à exécuter, toujours sans tourner dans le vide.
- Petit nettoyage de style pour respecter les règles de l'école.

Côté équipe :
- Kylian a démarré l'interface graphique : lecture des arguments de démarrage et début de la gestion réseau, avec des tests.
- Alexis a posé les premières briques de l'intelligence artificielle.

## Équipe

3 personnes sur 6 sont maintenant actives : Patrick, Kylian, Alexis.
Les 3 autres n'ont toujours rien poussé.

## La suite

attaquer les commandes de déplacement du joueur (Forward, Left, Right).

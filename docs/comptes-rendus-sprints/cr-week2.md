# Semaine 2 — du 1er au 7 juin 2026

## Ce qui a été fait

### Côté serveur
- **Poignée de main de connexion terminée** : quand un joueur se connecte, le serveur lui dit bonjour (`WELCOME`), lit le nom de son équipe, vérifie si l'équipe existe et a encore une place, puis lui répond avec la taille de la carte et le nombre de places restantes. Si l'équipe est inconnue ou pleine, la connexion est refusée proprement. Le nom spécial `GRAPHIC` est reconnu pour l'interface graphique.
- **Horloge du jeu démarrée** : elle mesure le temps et calcule combien de temps prend chaque action selon la vitesse du jeu. Tout testé. La PR est ouverte, en attente de relecture.

### Côté équipe
- Tout le monde a accepté son invitation au dépôt.
- Les 30 issues sont assignées à leur auteur. Plus personne ne peut dire "j'ai pas accès" ou "je sais pas quoi faire".

## Équipe — qui a codé cette semaine

Patrick ;
Alexis : a validé la PR de la poignée de main pour faire avancer la branche principale.

## La suite

- Faire la file d'attente des commandes : un joueur peut envoyer jusqu'à 10 ordres à l'avance, le serveur les exécute dans l'ordre à l'heure prévue.
- C'est la dernière grosse brique du socle serveur. Après ça, les autres parties (IA, interface graphique) peuvent vraiment se brancher.
- Attendu cette semaine : au moins une proposition de code par personne, en priorité Florian (commandes et protocole, le plus gros morceau) et Dorone (monde et règles).

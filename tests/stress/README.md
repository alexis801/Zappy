# Tests de charge

Trois scripts qui simulent du trafic reseau contre un serveur deja lance.

## Build

```sh
make
```

## Usage

### Tout lancer

```sh
make stress
```

Demarre un serveur, execute les trois scenarios, arrete le serveur.

### Scenario par scenario

Demarrer un serveur :
```sh
./zappy_server -p 4242 -x 20 -y 20 -n red blue -c 50 -f 100
```

Connexions paralleles (64 clients en meme temps) :
```sh
python3 tests/stress/stress_connections.py --team red
```

Spam de commandes (1 client, 1000 ordres a la suite) :
```sh
python3 tests/stress/stress_spam.py --team red
```

Cycles connexion/deconnexion (100 fois) :
```sh
python3 tests/stress/stress_reconnect.py --team red
```

### Memoire

```sh
valgrind --leak-check=full ./zappy_server -p 4242 -x 20 -y 20 -n red blue -c 50 -f 100 &
make stress
```

## Glossaire

- **Serveur** : programme `zappy_server` qui gere la partie.
- **Client** : programme qui se connecte au serveur.
- **Handshake** : echange initial `WELCOME` / nom d'equipe / places libres + taille de la carte.
- **Equipe** : groupe de joueurs avec un nom.
- **Slot** : place dans une equipe.
- **Valgrind** : outil de detection de fuites memoire.

#!/usr/bin/env python3

##
## EPITECH PROJECT, 2026
## zappy
## File description:
## ai_algo - point d'entree de la strategie
##

import sys
import getopt

from ai import AI


USAGE = "USAGE: ./zappy_ai -p port -n name -h machine [--worker]"


def parse_args(argv):
    port = None
    team = None
    host = "localhost"
    is_boss = True

    try:
        opts, _ = getopt.getopt(argv, "p:n:h:", ["help", "worker"])
    except getopt.GetoptError:
        print(USAGE)
        sys.exit(84)

    for opt, arg in opts:
        if opt == "--help":
            print(USAGE)
            sys.exit(0)
        elif opt == "-p":
            try:
                port = int(arg)
            except ValueError:
                print(USAGE)
                sys.exit(84)
        elif opt == "-n":
            team = arg
        elif opt == "-h":
            host = arg
        elif opt == "--worker":
            is_boss = False

    if port is None or team is None:
        print(USAGE)
        sys.exit(84)
    return port, team, host, is_boss

def main(argv):
    port, team, host, is_boss = parse_args(argv)
    ai = AI(team, host, port, is_boss)
    try:
        ai.my_ai()
    except (ConnectionError, OSError) as exc:
        print(f"connexion impossible a {host}:{port} ({exc})", file=sys.stderr)
        sys.exit(84)


if __name__ == "__main__":
    main(sys.argv[1:])

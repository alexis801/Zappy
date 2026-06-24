##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## main
##

import sys, getopt
from strategy.Ai_Algo import AI

def parser(argv):
    port = None
    team = None
    host = "localhost"
    is_boss = True

    try:
        opts, args = getopt.getopt(argv, "p:n:h:", ["help", "worker"])
    except getopt.GetoptError:
        print("USAGE: ./zappy_ai -p port -n name -h machine [--worker]")
        sys.exit(84)

    for opt, arg in opts:
        if opt == "--help":
            print("USAGE: ./zappy_ai -p port -n name -h machine [--worker]")
            sys.exit(0)

        elif opt == "-p":
            port = int(arg)

        elif opt == "-n":
            team = arg

        elif opt == "-h":
            host = arg

        elif opt == "--worker":
            is_boss = False

    return port, team, host, is_boss

def main(argv):
    port, team, host, is_boss = parser(argv)

    print(f"Port: {port}")
    print(f"Team: {team}")
    print(f"Host: {host}")

    ai = AI(team, host, port, is_boss)
    try:
        ai.my_ai()
    except (ConnectionError, OSError) as exc:
        print(f"connexion impossible a {host}:{port} ({exc})", file=sys.stderr)
        sys.exit(84)

if __name__ == "__main__":
    main(sys.argv[1:])
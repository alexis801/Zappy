##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## main
##

import sys
import getopt
from strategy.Ai_Algo import AI


def parser(argv):
    port = 4242
    team = None
    host = "localhost"
    is_boss = True

    try:
        opts, args = getopt.getopt(argv, "p:n:h:", ["help", "worker"])
    except getopt.GetoptError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        print("USAGE: ./zappy_ai -p port -n name -h machine [--worker]")
        sys.exit(84)

    for opt, arg in opts:
        if opt == "--help":
            print("USAGE: ./zappy_ai -p port -n name -h machine [--worker]")
            sys.exit(0)

        elif opt == "-p":
            try:
                port = int(arg)
                if not 1 <= port <= 65535:
                    raise ValueError
            except ValueError:
                print("Error: port must be between 1 and 65535", file=sys.stderr)
                sys.exit(84)

        elif opt == "-n":
            team = arg

        elif opt == "-h":
            host = arg

        elif opt == "--worker":
            is_boss = False

    if not team or not team.strip():
        print("Error: team name is required (-n name)", file=sys.stderr)
        print("USAGE: ./zappy_ai [-p port] -n name [-h machine] [--worker]")
        sys.exit(84)

    if args:
        print(f"Error: unexpected arguments: {' '.join(args)}", file=sys.stderr)
        sys.exit(84)

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
##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## stress_connections
##

"""Stress test: many concurrent client connections to the Zappy server.

Opens N TCP connections in parallel, performs the handshake for each, and
verifies the server stays alive and replies correctly.
"""

import argparse
import socket
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from contextlib import contextmanager
from typing import Generator


@contextmanager
def open_client(host: str, port: int, timeout: float) -> Generator[socket.socket, None, None]:
    """Open a TCP socket to the server and yield it inside a with-block."""
    sock = socket.create_connection((host, port), timeout=timeout)
    try:
        yield sock
    finally:
        sock.close()


def receive_line(sock: socket.socket, max_bytes: int = 4096) -> str:
    """Read until a newline is found or max_bytes is reached."""
    chunks: list[bytes] = []
    total = 0
    while total < max_bytes:
        chunk = sock.recv(max_bytes - total)
        if not chunk:
            break
        chunks.append(chunk)
        total += len(chunk)
        if b"\n" in chunk:
            break
    return b"".join(chunks).decode(errors="replace")


def handshake(host: str, port: int, team: str, timeout: float) -> bool:
    """Open a connection, perform the team handshake, and close."""
    try:
        with open_client(host, port, timeout) as sock:
            welcome = receive_line(sock)
            if "WELCOME" not in welcome:
                return False
            sock.sendall(f"{team}\n".encode())
            answer = receive_line(sock)
            return bool(answer)
    except OSError:
        return False


def run(host: str, port: int, team: str, clients: int, timeout: float) -> int:
    """Run N parallel handshakes and report how many succeeded."""
    with ThreadPoolExecutor(max_workers=clients) as pool:
        futures = [
            pool.submit(handshake, host, port, team, timeout)
            for _ in range(clients)
        ]
        return sum(1 for future in as_completed(futures) if future.result())


def parse_args() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=4242)
    parser.add_argument("--team", default="default")
    parser.add_argument("--clients", type=int, default=64)
    parser.add_argument("--timeout", type=float, default=2.0)
    return parser.parse_args()


def main() -> int:
    """Entry point."""
    args = parse_args()
    success = run(args.host, args.port, args.team, args.clients, args.timeout)
    print(f"{success}/{args.clients} handshakes ok")
    return 0 if success == args.clients else 1


if __name__ == "__main__":
    sys.exit(main())

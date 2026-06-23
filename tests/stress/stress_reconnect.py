##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## stress_reconnect
##

"""Stress test: rapid connect/disconnect cycles against the Zappy server.

Reuses the same TCP slot many times in a row to make sure the server cleans
its internal state between clients and never refuses a fresh connection.
"""

import argparse
import socket
import sys
import time
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


def cycle(host: str, port: int, team: str, timeout: float) -> bool:
    """One connect / handshake / disconnect cycle."""
    try:
        with open_client(host, port, timeout) as sock:
            welcome = receive_line(sock)
            if "WELCOME" not in welcome:
                return False
            sock.sendall(f"{team}\n".encode())
            return bool(receive_line(sock))
    except OSError:
        return False


def run(host: str, port: int, team: str, cycles: int,
        delay: float, timeout: float) -> tuple[int, float]:
    """Run N cycles in sequence and return (success_count, total_time)."""
    success = 0
    start = time.perf_counter()
    for _ in range(cycles):
        if cycle(host, port, team, timeout):
            success += 1
        if delay > 0:
            time.sleep(delay)
    return success, time.perf_counter() - start


def parse_args() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=4242)
    parser.add_argument("--team", default="default")
    parser.add_argument("--cycles", type=int, default=100)
    parser.add_argument("--delay", type=float, default=0.0)
    parser.add_argument("--timeout", type=float, default=2.0)
    return parser.parse_args()


def main() -> int:
    """Entry point."""
    args = parse_args()
    success, elapsed = run(
        args.host, args.port, args.team, args.cycles,
        args.delay, args.timeout)
    print(f"{success}/{args.cycles} cycles ok in {elapsed:.2f}s")
    return 0 if success == args.cycles else 1


if __name__ == "__main__":
    sys.exit(main())

##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## stress_spam
##

"""Stress test: a single client spams commands as fast as possible.

The Zappy protocol allows up to 10 pending requests per client. This script
sends N Forward commands in a row and reports how many replies came back.
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


def consume_until(sock: socket.socket, marker: bytes) -> None:
    """Read until the given marker appears in the stream."""
    buffer = b""
    while marker not in buffer:
        chunk = sock.recv(4096)
        if not chunk:
            return
        buffer += chunk


def count_replies(data: bytes) -> tuple[int, int]:
    """Return (ok_count, ko_count) found in a raw byte stream."""
    text = data.decode(errors="replace")
    return text.count("ok\n"), text.count("ko\n")


def spam(host: str, port: int, team: str, commands: int,
         timeout: float) -> tuple[int, int, float]:
    """Send N Forward commands and read every reply back."""
    with open_client(host, port, timeout) as sock:
        consume_until(sock, b"WELCOME\n")
        sock.sendall(f"{team}\n".encode())
        consume_until(sock, b"\n")
        payload = b"Forward\n" * commands
        start = time.perf_counter()
        sock.sendall(payload)
        buffer = b""
        while buffer.count(b"\n") < commands:
            chunk = sock.recv(4096)
            if not chunk:
                break
            buffer += chunk
        elapsed = time.perf_counter() - start
        ok_count, ko_count = count_replies(buffer)
        return ok_count, ko_count, elapsed


def parse_args() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="localhost")
    parser.add_argument("--port", type=int, default=4242)
    parser.add_argument("--team", default="default")
    parser.add_argument("--commands", type=int, default=1000)
    parser.add_argument("--timeout", type=float, default=30.0)
    return parser.parse_args()


def main() -> int:
    """Entry point."""
    args = parse_args()
    ok_count, ko_count, elapsed = spam(
        args.host, args.port, args.team, args.commands, args.timeout)
    total = ok_count + ko_count
    print(f"{ok_count} ok, {ko_count} ko in {elapsed:.2f}s "
          f"({total}/{args.commands} replies)")
    return 0 if ok_count > 0 else 1


if __name__ == "__main__":
    sys.exit(main())

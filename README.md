# Unix Shell implementation in C

This repository contains a simple Unix shell implemented in C. The shell supports running system commands, handling built-in commands like `cd` and `exit`, and managing signals such as `SIGINT` for interrupting processes.

## Features

- Execute standard Unix commands.
- Built-in commands: `cd`, `exit`.
- Signal handling for `Ctrl+C` (`SIGINT`).
- Forking and process management for command execution.

## Getting Started

### Prerequisites

To compile and run this shell, you'll need a C compiler like `gcc` and a Unix-based operating system.

### Compilation

To compile the shell, run the following command:

gcc -o my_shell main.c shell.c -I.

Once compiled you can start the shell by running:
./my_shell


# Process Manager

**Language:** C

**Build environment:** Linux with `gcc` and `make`. The shell uses `fork`, `execvp`, signals, and the `/proc` filesystem, so it has to run on Linux.

A small command shell that starts programs in the background, lists those processes, and stops, continues, or kills them by pid. Process details such as state, CPU time, and memory are read from `/proc/<pid>/stat` and `/proc/<pid>/status`. Background pids are kept in a linked list.

## Breakdown

- `main.c` — the prompt loop and the commands below
- `linked_list.c`, `linked_list.h` — the list of background processes
- `Makefile` — builds the `pman` executable
- `args.c`, `demo2.c`, `inf.c` — small helper and demo programs used while testing the shell

## Usage

```bash
make
./pman
```

The prompt is `Pman: >`. Commands:

| Command | What it does |
| --- | --- |
| `bg program [args...]` | Start `program` in the background and record its pid |
| `bglist` | Print the pid and command of each background process |
| `bgstop <pid>` | Send `SIGSTOP` |
| `bgstart <pid>` | Send `SIGCONT` |
| `bgkill <pid>` | Send `SIGTERM` and drop the pid from the list |
| `pstat <pid>` | Print command name, state, user time, system time, resident memory, and context-switch counts |
| `q` | Quit |

Example:

```
Pman: > bg sleep 30
Pman: > bglist
Pman: > pstat 12345
Pman: > bgkill 12345
Pman: > q
```

```bash
make clean
```

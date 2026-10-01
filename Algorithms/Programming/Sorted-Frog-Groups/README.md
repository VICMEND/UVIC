# Sorted Frog Groups

**Language:** Java

**Build environment:** Any machine with a JDK. This program is text only, so it does not need a display. Compile from this directory with `javac`. There is no build file.

Keep a collection of named frogs in sorted order by inserting each one with a recursive binary search. Split a group into two halves, and decide whether two groups match when either half may be swapped with the other.

## Breakdown

- `Frog.java` — a frog with a name, comparable so names sort in order
- `Group.java` — the sorted collection. `addFrog` inserts with a recursive binary search, and the group can be split and compared with another group
- `Tester.java` — builds 1000 randomly named frogs, checks that the group matches a normally sorted array, then checks the split and swap comparison. This is the class with `main`.

## Usage

```bash
javac *.java
java Tester
```

The program prints whether the sorted-group checks passed, for example:

```
Groups test 1: true
Groups test 2: true
```

`true` means that check matched. The tester uses a fixed count of 1000 frogs with random three-letter names, so the printed names change between runs while the checks should still pass.

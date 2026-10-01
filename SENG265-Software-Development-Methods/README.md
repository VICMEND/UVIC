# SENG 265 — Software Development Methods

UVIC SENG 265, 2023. Four assignments that move from C string processing to Python data work, then back to C with dynamic memory, and finish with a small Python graphics generator.

Each assignment below lists the language, where it was meant to run, how the program is put together, and how to run it.

---

## Assignment 1 — Calendar parser

**Language:** C (C99)

**Build environment:** Linux with `gcc`. This was written for the course Linux lab machines.

Reads an iCalendar (`.ics`) file and prints the events that fall between a start date and an end date, including weekly repeating events.

The program walks the file line by line, keeps the fields of the current `VEVENT`, expands an `RRULE` into one occurrence per week, and prints each event that sits inside the requested range:

```
May 19, 2023
------------
10:30 AM to 11:30 AM: meeting {{Home}}
```

**Breakdown**

- `A1/event_manager.c` — argument parsing, file reading, date filtering, and formatted output
- `A1/diana-devops.ics` — sample calendar

**Usage**

```bash
cd A1
gcc -Wall -std=c99 event_manager.c -o event_manager
./event_manager --start=2023/5/1 --end=2023/5/31 --file=diana-devops.ics
```

Dates are `YYYY/M/D`. Output goes to the terminal.

This was a first contact with C. Structs and `time.h` would have made the date handling much simpler.

---

## Assignment 2 — Song CSV summary (pandas)

**Language:** Python 3, using pandas and NumPy

**Build environment:** Linux or Windows with Python 3 and pandas installed (`pip install pandas numpy`).

Loads one or more “top songs” CSV files, drops the unused columns, sorts by `popularity`, `danceability`, or `energy`, and writes the top rows to `output.csv`.

**Breakdown**

- `A2/music_manager.py` — argument parsing, pandas load/sort, and CSV write
- `A2/top_songs_1999.csv`, `top_songs_2009.csv`, `top_songs_2019.csv` — input data

**Usage**

```bash
cd A2
python music_manager.py --sortBy=danceability --display=3 --files=top_songs_2019.csv
```

Several files can be passed as a comma-separated list:

```bash
python music_manager.py --sortBy=popularity --display=10 --files=top_songs_1999.csv,top_songs_2009.csv
```

The result is `output.csv` in the same directory, with columns `artist,song,year,<sort column>`.

---

## Assignment 3 — Song CSV summary (C linked list)

**Language:** C (C99)

**Build environment:** Linux with `gcc`. Build with the makefile in `A3`.

The same song summary as assignment 2, written in C. Each row is stored in a linked-list node and inserted in sorted order. The program then writes the requested number of top songs to `output.csv`.

**Breakdown**

- `A3/music_manager.c` — argument parsing, CSV reading, and printing
- `A3/list.c`, `A3/list.h` — ordered linked list
- `A3/emalloc.c`, `A3/emalloc.h` — malloc wrapper that exits on failure
- `A3/makefile` — builds the `music_manager` executable
- `A3/top_songs_*.csv` — input data

**Usage**

```bash
cd A3
make
./music_manager --sortBy=popularity --display=10 --files=top_songs_1999.csv
```

`--sortBy` is `popularity`, `energy`, or `danceability`. Output is `output.csv`.

```bash
make clean
```

This assignment reused the argument style from assignment 1 and added dynamic memory and the supplied list structure.

---

## Assignment 4 — Random SVG art

**Language:** Python 3 (standard library only)

**Build environment:** Linux, macOS, or Windows with Python 3. No extra packages.

Writes HTML files that contain an SVG canvas of randomly colored circles, rectangles, and ellipses. Shape data is stored in named tuples.

**Breakdown**

- `A4/a43.py` — HTML/SVG writer, random shape generator, and named-tuple shape types

**Usage**

```bash
cd A4
python a43.py
```

That creates `a431.html`, `a432.html`, and `a433.html` in the same directory. Open any of them in a browser. Each run draws 1000 shapes on a 500 by 300 canvas, so the files change every time the script runs.

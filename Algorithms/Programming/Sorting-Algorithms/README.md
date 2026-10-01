# Sorting Algorithms

**Language:** Java

**Build environment:** Any desktop with a JDK that includes Swing (a standard JDK). The window needs a display. Compile from this directory with `javac`. There is no build file.

Implement merge sort, quicksort, insertion sort, and radix sort, and draw each pass so the ordering can be seen. The same program includes a pixel graph of same-colored neighbors, with flood-fill methods that recolor a connected region using depth-first and breadth-first search.

## Breakdown

- `GraphAlgorithms.java` — merge sort, quicksort, insertion sort, radix sort, and the DFS and BFS fill methods. Each sort calls `drawSequence` after a pass so the canvas updates.
- `P2Tester.java` — the Swing window. Buttons run a sort or a fill, and the canvas shows the current sequence or image. This is the class with `main`.
- `PixelGraph.java` — a graph of pixels, with edges between neighbors of the same color
- `PixelVertex.java` — one pixel node
- `PixelWriter.java` — draws a list of integers onto the canvas

## Usage

```bash
javac *.java
java P2Tester
```

A window opens. Use the controls to pick a sort and run it; the plot updates as the list is ordered. The fill controls recolor a connected region of the loaded image with DFS or BFS.

On Windows PowerShell, if `javac` is not on `PATH`, use the full path to the JDK’s `bin` directory.

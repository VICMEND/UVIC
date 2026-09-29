.phony all:
all: ACS

ACS: main.c
	gcc -o ACS main.c queue.c -pthread

.PHONY clean:
clean:
	-rm -rf *.o *.exe

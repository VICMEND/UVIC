SRC1 = diskinfo.c
SRC2 = disklist.c
SRC3 = diskget.c
SRC4 = diskput.c
SRC5 = helper.c

OUT1 = diskinfo
OUT2 = disklist
OUT3 = diskget
OUT4 = diskput


.PHONY: all clean

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

$(OUT1): $(SRC1)
	gcc -Wall -o $(OUT1) $(SRC1) $(SRC5)

$(OUT2): $(SRC2)
	gcc -Wall -o $(OUT2) $(SRC2) $(SRC5)

$(OUT3): $(SRC3)
	gcc -Wall -o $(OUT3) $(SRC3) $(SRC5)

$(OUT4): $(SRC4)
	gcc -Wall -o $(OUT4) $(SRC4) $(SRC5)

clean:
	-rm -rf $(OUT1) $(OUT2) $(OUT3) $(OUT4)

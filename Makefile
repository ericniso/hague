# Makefile for Hague
P = hague

CFLAGS_STD = -g -Wall -DDEBUG -O3 -march=native -Wno-deprecated -Wno-parentheses -Wno-format
STD_LIBS = zlib

LIBS = cmdline.o hgraph.o
CFLAGS_EXTRA =  -m64 -std=c11 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS_LIBS = `pkg-config --cflags $(STD_LIBS)`
CFLAGS_TEST =  -DTEST_EVERYTHING  `pkg-config --cflags $(DEBUG_LIBS)`
LDLIBS = `pkg-config --libs $(STD_LIBS)`
CFLAGS = $(CFLAGS_STD) $(CFLAGS_EXTRA) $(CFLAGS_LIB)

CC_FULL = $(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIB_DIR) $(CFLAGS_LIBS)

bin: $(P) $(LIBS)

$(P): $(P).c $(P).h $(LIBS)

clean:
	rm -rf $(P) *.o cmdline.*

cmdline.c cmdline.h: $(P).ggo
	gengetopt -i $< --output-dir=./

.PHONY: clean

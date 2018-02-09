# Makefile for Hague
P = hague

# Includes
INCS = -I libs -I src

# Lib linking
LINK = -lz

# C flags
CFLAGS = -g -m64 -std=c11 -Wall -Wextra

# Sources

# Graph
GRAPH_OBJDIR = build/graph
GRAPH_SRCS = $(wildcard src/graph/*.c)
GRAPH_HDRS = $(wildcard src/graph/*.h)
GRAPH_FILES = $(notdir $(GRAPH_SRCS))
GRAPH_OBJS = $(addprefix $(GRAPH_OBJDIR)/, $(GRAPH_FILES:.c=.o))

# IO
IO_OBJDIR = build/io
IO_SRCS = $(wildcard src/io/*.c)
IO_HDRS = $(wildcard src/io/*.h)
IO_FILES = $(notdir $(IO_SRCS))
IO_OBJS = $(addprefix $(IO_OBJDIR)/, $(IO_FILES:.c=.o))

# Cmdline
CMDLINE_OBJDIR = build/cmdline
CMDLINE_GEN_SRCS = src/cmdline/cmdline.c src/cmdline/cmdline.h
CMDLINE_OBJS = $(CMDLINE_OBJDIR)/cmdline.o

OBJS =  $(CMDLINE_OBJS) $(GRAPH_OBJS) $(IO_OBJS)

all: bin lib

bin: bin/hague

lib: lib/libhague.so

bin/hague: src/main/hague.c $(CMDLINE_GEN_SRCS) $(OBJS)
	mkdir -p bin
	$(CC) -o $@ $(CFLAGS) $(INCS) src/main/hague.c $(OBJS) $(LINK)

lib/libhague.so: src/graph/hgraph.c src/graph/hgraph.h
	mkdir -p lib
	$(CC) -shared -o $@ $(CFLAGS) $(INCS) -fPIC $< $(LINK)

$(IO_OBJDIR)/%.o: src/io/%.c src/io/%.h
	mkdir -p $(IO_OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCS) -c $<

$(MAP_OBJDIR)/%.o: src/map/%.c src/map/%.h
	mkdir -p $(MAP_OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCS) -c $<

$(GRAPH_OBJDIR)/%.o: src/graph/%.c src/graph/%.h
	mkdir -p $(GRAPH_OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCS) -c $<

$(CMDLINE_OBJDIR)/%.o: src/cmdline/%.c src/cmdline/%.h
	mkdir -p $(CMDLINE_OBJDIR)
	$(CC) -o $@ $(CFLAGS) $(INCS) -c $<

$(CMDLINE_GEN_SRCS): conf/$(P).ggo
	mkdir -p src/cmdline
	gengetopt -i $< --output-dir=src/cmdline/

clean:
	rm -rf bin lib build src/cmdline

.PHONY: all bin lib clean

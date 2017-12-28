# Makefile for Hague
P = hague

# Includes
INCS = -I libs

# Lib linking
LINK = -lz

# C flags
CFLAGS = -m64 -std=c11 -Wall -Wextra

# Sources

# Hague
HAGUE_OBJDIR = build/hague
HAGUE_SRCS = $(wildcard src/main/*.c)
HAGUE_HDRS = $(wildcard src/main/*.h)
HAGUE_FILES = $(notdir $(HAGUE_SRCS))
HAGUE_OBJS = $(addprefix $(HAGUE_OBJDIR)/, $(HAGUE_FILES:.c=.o))

# Graph
GRAPH_OBJDIR = build/graph
GRAPH_SRCS = $(wildcard src/graph/*.c)
GRAPH_HDRS = $(wildcard src/graph/*.h)
GRAPH_FILES = $(notdir $(GRAPH_SRCS))
GRAPH_OBJS = $(addprefix $(GRAPH_OBJDIR)/, $(GRAPH_FILES:.c=.o))

# Graph
MAP_OBJDIR = build/map
MAP_SRCS = $(wildcard src/map/*.c)
MAP_HDRS = $(wildcard src/map/*.h)
MAP_FILES = $(notdir $(MAP_SRCS))
MAP_OBJS = $(addprefix $(MAP_OBJDIR)/, $(MAP_FILES:.c=.o))

# Cmdline
CMDLINE_OBJDIR = build/cmdline
CMDLINE_GEN_SRCS = src/cmdline/cmdline.c src/cmdline/cmdline.h
CMDLINE_OBJS = $(CMDLINE_OBJDIR)/cmdline.o

OBJS =  $(CMDLINE_OBJS) $(GRAPH_OBJS) $(MAP_OBJS) $(HAGUE_OBJS)

bin: bin/hague

bin/hague: $(CMDLINE_GEN_SRCS) $(OBJS)
	mkdir -p bin
	$(CC) -o $@ $(CFLAGS) -I src/main/ -I src/graph/ -I src/cmdline/ $(INCS) $(OBJS) $(LINK)

$(HAGUE_OBJDIR)/%.o: src/main/%.c src/main/%.h
	mkdir -p $(HAGUE_OBJDIR)
	$(CC) -o $@ $(CFLAGS) -I src/main/ -I src/graph/ -I src/cmdline $(INCS) -c $<

$(MAP_OBJDIR)/%.o: src/map/%.c src/map/%.h
	mkdir -p $(MAP_OBJDIR)
	$(CC) -o $@ $(CFLAGS) -I src/map/ $(INCS) -c $<

$(GRAPH_OBJDIR)/%.o: src/graph/%.c src/graph/%.h
	mkdir -p $(GRAPH_OBJDIR)
	$(CC) -o $@ $(CFLAGS) -I src/graph/ $(INCS) -c $<

$(CMDLINE_OBJDIR)/%.o: src/cmdline/%.c src/cmdline/%.h
	mkdir -p $(CMDLINE_OBJDIR)
	$(CC) -o $@ $(CFLAGS) -I src/cmdline/ $(INCS) -c $<

$(CMDLINE_GEN_SRCS): conf/$(P).ggo
	mkdir -p src/cmdline
	gengetopt -i $< --output-dir=src/cmdline/

clean:
	rm -rf bin build src/cmdline

.PHONY: bin clean

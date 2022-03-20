# vim: set noet:

CC=g++
CFLAGS=-std=c++17 -I. -lpthread
OUTDIR=build
SOURCES=main.cpp Timer.cpp
DEPS=Timer.h
EXE=$(OUTDIR)/quicksort_multi_vec
OBJ=$(patsubst %.cpp, $(OUTDIR)/%.o, $(SOURCES))

all: build $(EXE)

# $@ is a variable for whatever is on left of rule
# $^ is first thing on right of it, I guess $> is the rest?

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTDIR)/%.o : %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

build:
	@mkdir -p build

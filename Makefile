CXX = gcc
EXE = ascii
SOURCES = $(wildcard *.c)
OBJS = $(SOURCES:.c=.o)

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	CXXFLAGS = -std=c99 -pedantic -Wall -Wextra
endif

.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete.

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
		rm $(EXE) $(OBJS)

CXX = gcc
EXE = libascii.so
SOURCES = draw.c escape.c syntax.c term.c
OBJS = $(SOURCES:.c=.o)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	CXXFLAGS = -std=c99 -pedantic -Wall -Wextra
	LFLAGS = -shared -fPIC -o
endif

.c.o:
	$(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

all: $(EXE)
	@echo Build complete.

$(EXE): $(OBJS)
	$(CXX) $(LFLAGS) $(EXE) $(OBJS) $(CXXFLAGS) -lc

clean:
		rm $(EXE) $(OBJS)

CC = gcc
CFLAGS = -Wall

debug =
ifeq ($(debug), 1)
	CFLAGS += -Wextra -DDEBUG_PRINT=1 -DDEBUG_LOG=1 -g -O0
else
	CFLAGS += -DDEBUG_PRINT=0 -DDEBUG_LOG=0 -O2
endif

SRC := $(wildcard ./*.c)

TARGET = sudoku_solver sudoku2str

.PHONY: all clean

all: $(TARGET)

sudoku_solver: sudoku_solver.o dlx.o dlx_sudoku.o

sinclude $(SRC:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\(.*\)\.o[:]*,\1.o $@:,' < $@.$$$$ > $@; \
		rm -f $@.$$$$

clean:
	-rm -f *.o $(TARGET)

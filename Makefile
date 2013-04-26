CC = gcc
CFLAGS = -Wall

debug =
ifeq ($(debug), 1)
	CFLAGS += -Wextra -DDEBUG_PRINT=1 -DDEBUG_LOG=1 -g -O0
else
	CFLAGS += -DDEBUG_PRINT=0 -DDEBUG_LOG=0 -O2
endif

SRC := $(wildcard ./*.c)

TARGET = test_sudoku
TMPTARGET = test_fun test_print_dlx_matrix test_dlx  demo dancing_links.o

.PHONY: all clean

# all: $(TARGET) $(TMPTARGET)
all: $(TARGET)

demo: demo.o dancing_links.o

test_print_dlx_matrix: test_print_dlx_matrix.o dancing_links.o

test_dlx: test_dlx.o dlx.o

test_sudoku: test_sudoku.o dlx.o dlx_sudoku.o

test_fun: test_fun.o

sinclude $(SRC:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\(.*\)\.o[:]*,\1.o $@:,' < $@.$$$$ > $@; \
		rm -f $@.$$$$

clean:
	-rm -f *.o $(TARGET) $(TMPTARGET)

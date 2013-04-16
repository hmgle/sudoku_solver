CC = gcc
CFLAGS = -Wall

debug =
ifeq ($(debug), 1)
	CFLAGS += -Wextra -DDEBUG_PRINT=1 -DDEBUG_LOG=1 -g -O0
else
	CFLAGS += -DDEBUG_PRINT=0 -DDEBUG_LOG=0 -O2
endif

SRC := $(wildcard ./*.c)

TARGET = demo dancing_links.o
TMPTARGET = test_fun test_print_dlx_matrix

.PHONY: all clean

all: $(TARGET) $(TMPTARGET)

demo: demo.o dancing_links.o

test_print_dlx_matrix: test_print_dlx_matrix.o dancing_links.o

test_fun: test_fun.o

# demo.o: demo.c debug_print.h
# 	$(CC) -c -o $@ $< $(CFLAGS)

# dancing_links.o: dancing_links.c dancing_links.h
# 	$(CC) -c -o $@ $< $(CFLAGS)

# test_fun.o: test_fun.c

sinclude $(SRC:.c=.d)

%d: %c
	@set -e; rm -f $@; \
		$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\(.*\)\.o[:]*,\1.o $@:,' < $@.$$$$ > $@; \
		rm -f $@.$$$$


clean:
	-rm -f *.o $(TARGET) $(TMPTARGET)

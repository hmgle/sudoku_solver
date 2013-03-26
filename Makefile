CC = gcc
CFLAGS = -Wall -O2 -g

debug =
ifeq ($(debug), 1)
	CFLAGS += -Wextra -DDEBUG_PRINT=1 -DDEBUG_LOG=1
else
	CFLAGS += -DDEBUG_PRINT=0 -DDEBUG_LOG=0
endif

TARGET = demo dancing_links.o
TMPTARGET = test_fun

.PHONY: all clean

all: $(TARGET) $(TMPTARGET)

demo: demo.o dancing_links.o

demo.o: demo.c debug_print.h
	$(CC) -c -o $@ $< $(CFLAGS)

dancing_links.o: dancing_links.c dancing_links.h
	$(CC) -c -o $@ $< $(CFLAGS)

test_fun: test_fun.o

test_fun.o: test_fun.c

clean:
	-rm -f *.o $(TARGET) $(TMPTARGET)

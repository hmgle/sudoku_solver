CC = gcc
CFLAGS = -Wall -O2 -g

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

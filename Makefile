CC = gcc
CFLAGS = -Wall
HEADER = a3header.h

all: a3

a3: b09a3.o b09a3F2.o
	$(CC) $(CFLAGS) -o $@ $^	

.%o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm *.o
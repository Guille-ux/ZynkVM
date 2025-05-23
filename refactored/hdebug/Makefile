CC=gcc
CFLAGS=-I../
DEPS=../core/hash.h ../core/map.h
OBJ=hash_debug.o ../core/hash.o ../core/map.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hash_debug: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o hash_debug
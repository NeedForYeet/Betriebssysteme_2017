CC = /usr/bin/gcc
CFLAGS = -Wall -g

OBJ1 = first.o
OBJ2 = second.o

all: first second

first: $(OBJ1)
	$(CC) $(CFLAGS) -o first $(OBJ1)

second: $(OBJ2)
	$(CC) $(CFLAGS) -o second $(OBJ2)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o first second firstout secondout 

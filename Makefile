CC=gcc
#CFLAGS=-W -Wall -ansi -pedantic
CFLAGS=-g3
LDFLAGS=-lpcap
MAKE=make
EXEC=kao

all: $(EXEC)
	$(MAKE) clean

kao: utils.o main.o
	$(CC) -o kao utils.o main.o $(CFLAGS) $(LDFLAGS)

utils.o: utils.c
	$(CC) -o utils.o -c utils.c $(CFLAGS)

main.o: main.c utils.h
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm -rf *.o

fclean:
	$(MAKE) clean
	rm -rf $(EXEC)

re: fclean all


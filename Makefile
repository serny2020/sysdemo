CC=gcc
CFLAGS=-g -std=gnu99 -Wall -Wextra -Werror -Wfatal-errors -pedantic
IFLAGS=-I.
LDFLAGS=
LDLIBS=
PROGRAM=shell

all: $(PROGRAM)

%.o: %.c
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

$(PROGRAM): $(PROGRAM).o
	$(CC) -o shell shell.o

.PHONY:
clean:
	rm -f *.o $(PROGRAM)
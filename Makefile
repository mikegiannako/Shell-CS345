CC = gcc
CFLAGS = -Wall -g
.PHONY: default
default: shell

shell: command.o stack.o shell.o
	$(CC) $(CFLAGS) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-del -f *.exe *.o
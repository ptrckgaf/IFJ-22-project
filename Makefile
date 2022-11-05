PROG=ifj22
SOURCE=src/*.c
CC=gcc
FLAGS=-std=c11 -Wall -Wextra -g
.PHONY: all run clean pack

all: $(PROG)

$(PROG): $(SOURCE)
	$(CC) $(FLAGS) -o $(PROG) $(SOURCE)

run: $(PROG)
	./$(PROG)



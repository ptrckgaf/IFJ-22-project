PROG=ifj22
SOURCE=src/*.c
CC=gcc
TARGET_CODE=target.out
INTERPRET=ic22int
FLAGS=-std=c11 -Wall -Wextra -g
.PHONY: all run clean pack interpret

all: $(PROG)

$(PROG): $(SOURCE)
	$(CC) $(FLAGS) -o $(PROG) $(SOURCE)

run: $(PROG)
	./$(PROG)

$(TARGET_CODE): $(PROG)
	./$(PROG) > $(TARGET_CODE)
interpret: $(TARGET_CODE)
	./$(INTERPRET) $(TARGET_CODE)



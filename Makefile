CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/network.c src/packet.c src/ui.c
OBJ = $(SRC:.c=.o)
EXEC = arpspoof

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f src/*.o $(EXEC)

.PHONY: all clean

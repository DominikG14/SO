CC = gcc
CFLAGS = -I libs

SRCDIR = src
BUILDDIR = build

SRC = $(SRCDIR)/main.c
EXEC = $(BUILDDIR)/main


all: $(EXEC)

$(EXEC): $(SRC)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)


clean:
	rm -rf $(BUILDDIR)

run: $(EXEC)
	./$(EXEC)
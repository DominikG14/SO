CC = gcc
CFLAGS = -I libs

SRCDIR = src
BUILDDIR = build

SRC_MAIN  = $(SRCDIR)/main.c
EXEC_MAIN = $(BUILDDIR)/main

SRC_CLIENT  = $(SRCDIR)/client.c
EXEC_CLIENT = $(BUILDDIR)/client


all: $(EXEC_MAIN)

$(EXEC_MAIN): $(SRC_MAIN)
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	$(CC) $(CFLAGS) $(SRC_MAIN) -o $(EXEC_MAIN)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(EXEC_CLIENT)


clean:
	rm -rf $(BUILDDIR)

run: $(EXEC_MAIN)
	./$(EXEC_MAIN)
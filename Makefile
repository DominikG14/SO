CC = gcc
CFLAGS = -I libs

SRCDIR = src
BUILDDIR = build

SRC_MAIN  = $(SRCDIR)/main.c
EXEC_MAIN = $(BUILDDIR)/main

SRC_CLIENT  = $(SRCDIR)/client.c
EXEC_CLIENT = $(BUILDDIR)/client

SRC_CASHIER  = $(SRCDIR)/cashier.c
EXEC_CASHIER = $(BUILDDIR)/cashier

SRC_LIFEGUARD  = $(SRCDIR)/lifeguard.c
EXEC_LIFEGUARD = $(BUILDDIR)/lifeguard


all: $(EXEC_MAIN)

$(EXEC_MAIN): $(SRC_MAIN)
	clear
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	$(CC) $(CFLAGS) $(SRC_MAIN) -o $(EXEC_MAIN)
	$(CC) $(CFLAGS) $(SRC_CLIENT) -o $(EXEC_CLIENT)
	$(CC) $(CFLAGS) $(SRC_CASHIER) -o $(EXEC_CASHIER)
	$(CC) $(CFLAGS) $(SRC_LIFEGUARD) -o $(EXEC_LIFEGUARD)


clean:
	rm -rf $(BUILDDIR)

run: $(EXEC_MAIN)
	./$(EXEC_MAIN)
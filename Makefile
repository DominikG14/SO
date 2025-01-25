CC = gcc
CFLAGS = -I libs

SRCDIR = src
BUILDDIR = build

SRC_MAIN  = $(SRCDIR)/main.c
EXEC_MAIN = $(BUILDDIR)/main

SRC_KLIENT  = $(SRCDIR)/klient.c
EXEC_KLIENT = $(BUILDDIR)/klient

SRC_KASJER  = $(SRCDIR)/kasjer.c
EXEC_KASJER = $(BUILDDIR)/kasjer

SRC_RATOWNIK  = $(SRCDIR)/ratownik.c
EXEC_RATOWNIK = $(BUILDDIR)/ratownik


all: $(EXEC_MAIN)

$(EXEC_MAIN): $(SRC_MAIN)
	clear
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	$(CC) $(CFLAGS) $(SRC_MAIN) -o $(EXEC_MAIN)
	$(CC) $(CFLAGS) $(SRC_KLIENT) -o $(EXEC_KLIENT)
	$(CC) $(CFLAGS) $(SRC_KASJER) -o $(EXEC_KASJER)
	# $(CC) $(CFLAGS) $(SRC_RATOWNIK) -o $(EXEC_RATOWNIK)


clean:
	rm -rf $(BUILDDIR)

run: $(EXEC_MAIN)
	./$(EXEC_MAIN)
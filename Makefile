# TODO: faire des règles génériques

CC=gcc
CFLAGS=-Wall -g

BINDIR=./bin
OBJDIR=./objets

TESTS_FOURNIS=testsfournis
NOS_TESTS=testsperso

SRCS=stdes.c $(NOS_TESTS)/*.c ./eval-libc.c
OBJS=$(SRCS:./%.c=$(OBJDIR)/%.o)
INCLUDES=-I./

EXEC= $(BINDIR)/eval-libc $(BINDIR)/testsperso_OuvrirFermer

all: $(EXEC)

### Regles pour construction des fichiers objets ###

$(OBJDIR)/stdes.o: stdes.c stdes.h
	$(CC)  $(CFLAGS) -c -o $@ $<

$(OBJDIR)/eval-libc.o: ./eval-libc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $< 

$(OBJDIR)/testsperso_OuvrirFermer.o: $(NOS_TESTS)/testsperso_OuvrirFermer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $< 

### Regles pour construction des executables ###


$(BINDIR)/eval-libc: $(OBJDIR)/stdes.o $(OBJDIR)/eval-libc.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_OuvrirFermer: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_OuvrirFermer.o 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(OBJDIR)/*.o $(EXEC) test_read_file
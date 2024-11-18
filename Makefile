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

EXEC= $(BINDIR)/eval-libc \
	$(BINDIR)/testsperso_OuvrirFermer \
	$(BINDIR)/testsperso_read1 \
	$(BINDIR)/testsperso_read2 \
	$(BINDIR)/testsperso_read3 \
	$(BINDIR)/testsperso_write \
	$(BINDIR)/testsperso_LireEcrire \
	# $(BINDIR)/test \
	# $(BINDIR)/test_format \
	# $(BINDIR)/test-rand \

all: $(EXEC)

### Regles pour construction des fichiers objets ###

$(OBJDIR)/stdes.o: stdes.c stdes.h
	$(CC)  $(CFLAGS) -c -o $@ $<

$(OBJDIR)/eval-libc.o: ./eval-libc.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $< 

$(OBJDIR)/%.o: $(TESTS_FOURNIS)/%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $< 

$(OBJDIR)/%.o: $(NOS_TESTS)/%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $< 


### Regles pour construction des executables ###

$(BINDIR)/eval-libc: $(OBJDIR)/stdes.o $(OBJDIR)/eval-libc.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/test: $(OBJDIR)/stdes.o $(OBJDIR)/test.o 
	$(CC) $(CFLAGS) -o $@ $^

# $(BINDIR)/test-rand: $(OBJDIR)/stdes.o $(OBJDIR)/test-rand.o 
# 	$(CC) $(CFLAGS) -o $@ $^

# $(BINDIR)/test-format: $(OBJDIR)/stdes.o $(OBJDIR)/test-format.o 
# 	$(CC) $(CFLAGS) -o $@ $^


$(BINDIR)/testsperso_OuvrirFermer: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_OuvrirFermer.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_read1: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_read1.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_read2: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_read2.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_read3: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_read3.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_write: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_write.o 
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/testsperso_LireEcrire: $(OBJDIR)/stdes.o $(OBJDIR)/testsperso_LireEcrire.o 
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(OBJDIR)/*.o $(EXEC)
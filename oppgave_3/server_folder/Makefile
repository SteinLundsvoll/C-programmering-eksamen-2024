# Makefile template

INCLDIR = ./include
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

_DEPS = server.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = server.o oppgave3.o server_FTP.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)
		
server: $(OBJS)
		gcc -o $@ $^ $(CFLAGS)
		
.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

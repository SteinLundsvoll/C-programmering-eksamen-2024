# Makefile template

INCLDIR = ./include
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

_DEPS = client.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = client.o client_FTP.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)
		
client: $(OBJS)
		gcc -o $@ $^ $(CFLAGS)
		
.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

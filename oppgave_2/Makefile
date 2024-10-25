# Makefile template

INCLDIR = ./include
CC = gcc
CFLAGS = -O0
CFLAGS += -I$(INCLDIR)

OBJDIR = ./obj

_DEPS = source.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o fib.o prim.o kvad.o perf.o abun.o def.o odd.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))


$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)
		
main: $(OBJS)
		gcc -o $@ $^ $(CFLAGS) -lm # -lm lagt til for å inkludere math biblioteket
		
.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

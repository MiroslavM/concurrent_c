IDIR 	= ../include
CC	= gcc
CFLAGS	= -I$(IDIR)

ODIR	= apps
LDIR 	= ../lib

LIBS	= -lm

_DEPS 	= file_server.h
DEPS 	= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ 	= file_server.o create.o  
OBJ 	= $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

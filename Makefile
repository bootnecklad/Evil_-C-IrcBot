# Makefile by Toby Thain, <toby@telegraphics.com.au>

# Tested in OS X 10.4.


CFLAGS = -O2
LDFLAGS=-laaa -lbbb

OBJ = connection.o irc.o list.o main.o

qdbot : $(OBJ)
	$(CC) -o $@ $^
	$(LDFLAGS)

clean :
	rm -f qdbot $(OBJ)

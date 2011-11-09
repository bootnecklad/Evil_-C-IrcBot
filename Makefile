# Makefile by Toby Thain, <toby@telegraphics.com.au>

# Tested in OS X 10.4.


CFLAGS = -O2 -Wall

OBJ = connection.o irc.o list.o main.o

qdbot : $(OBJ)

clean :
	rm -f qdbot $(OBJ)

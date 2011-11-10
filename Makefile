# Makefile by Toby Thain, <toby@telegraphics.com.au>

# Tested in OS X 10.4.
# Testing on Windows XP - bootnecklad

CPPFLAGS=-DWINVER=0x0501 -D_WIN32_WINNT=0x0501
CFLAGS = -O2 -Wall
LDFLAGS= -lWs2_32

OBJ = connection.o irc.o list.o main.o

qdbot : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean :
	rm -f qdbot $(OBJ)
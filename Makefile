# Macros
CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wshadow \
-fstack-protector-all
LDLIBS = -lncurses

all: typing_test debugger

# Create executables
typing_test: typing_test.o utilities.o
	$(CC) -o typing_test typing_test.o utilities.o $(LDLIBS) 

debugger: debugger.o utilities.o
	$(CC) -o debugger debugger.o utilities.o

typing_test.o: typing_test.c
	$(CC) $(CFLAGS) -c typing_test.c $(LDLIBS)

debugger.o: debugger.c utilities.h
	$(CC) $(CFLAGS) -c debugger.c

utilities.o: utilities.c
	$(CC) $(CFLAGS) -c utilities.c

clean:
	@echo "Cleaning system"
	@rm -f *.o

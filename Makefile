CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest
VPATH = lib:src:test
LIB = -lm

# Main executable construction
main: main.o user.o karnaugh.o
	$(CC) $(CFLAGS) -o main main.o user.o karnaugh.o $(LIB)

# Test unit executable construction
test_unit: test_unit.o karnaugh.o
	$(CC) $(CFLAGS) -o test_unit test_unit.o karnaugh.o $(CXXFLAGS) $(LIB)

# Object generation from source and header for MAIN
main.o: ./src/user.h ./src/user.c ./src/karnaugh.h ./src/karnaugh.c ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o main.o $(CXXFLAGS)

# Object generation from source and header for TEST_UNIT
test_unit.o: ./test/test_unit.c ./test/test_unit.h
	$(CC) $(CFLAGS) -c ./test/test_unit.c $(CXXFLAGS)

# Object generation from source and header for USER
user.o: ./src/user.h ./src/user.c
	$(CC) $(CFLAGS) -c ./src/user.c -o user.o $(CXXFLAGS)

# Object generation from source and header for KARNAUGH
karnaugh.o: ./src/karnaugh.h ./src/karnaugh.c
	$(CC) $(CFLAGS) -c ./src/karnaugh.c -o karnaugh.o $(CXXFLAGS)	

clean:
	rm -f *.o main test_unit

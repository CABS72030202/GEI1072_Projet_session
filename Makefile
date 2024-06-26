CC = gcc
CFLAGS = -g -w -Wall
CXXFLAGS = -Ilib -Isrc -Itest
VPATH = lib:src:test
LIB = -lm

# Main executable construction
main: main.o public.o bool.o calc.o user.o karnaugh.o file.o
	$(CC) $(CFLAGS) -o main main.o public.o bool.o calc.o user.o karnaugh.o file.o $(LIB)

# Test unit executable construction
test_unit: test_unit.o public.o bool.o calc.o karnaugh.o file.o user.o
	$(CC) $(CFLAGS) -o test_unit test_unit.o public.o bool.o calc.o karnaugh.o file.o user.o $(CXXFLAGS) $(LIB)

# Object generation from source and header for MAIN
main.o: ./src/public.h ./src/public.c ./src/bool.h ./src/bool.c ./src/calc.h ./src/calc.c ./src/user.h ./src/user.c ./src/karnaugh.h ./src/karnaugh.c ./src/file.h ./src/file.c ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o main.o $(CXXFLAGS)

# Object generation from source and header for TEST_UNIT
test_unit.o: ./test/test_unit.c ./test/test_unit.h
	$(CC) $(CFLAGS) -c ./test/test_unit.c $(CXXFLAGS)

# Object generation from source and header for PUBLIC
public.o: ./src/public.h ./src/public.c 
	$(CC) $(CFLAGS) -c ./src/public.c -o public.o $(CXXFLAGS)

# Object generation from source and header for BOOL
bool.o: ./src/public.h ./src/public.c ./src/bool.h ./src/bool.c ./src/calc.h ./src/calc.c 
	$(CC) $(CFLAGS) -c ./src/bool.c -o bool.o $(CXXFLAGS)

# Object generation from source and header for CALC
calc.o: ./src/public.h ./src/public.c ./src/calc.h ./src/calc.c 
	$(CC) $(CFLAGS) -c ./src/calc.c -o calc.o $(CXXFLAGS)

# Object generation from source and header for USER
user.o: ./src/public.h ./src/public.c ./src/bool.h ./src/bool.c ./src/karnaugh.h ./src/karnaugh.c ./src/user.h ./src/user.c ./src/file.h ./src/file.c 
	$(CC) $(CFLAGS) -c ./src/user.c -o user.o $(CXXFLAGS)

# Object generation from source and header for KARNAUGH
karnaugh.o: ./src/public.h ./src/public.c ./src/bool.h ./src/bool.c ./src/karnaugh.h ./src/karnaugh.c
	$(CC) $(CFLAGS) -c ./src/karnaugh.c -o karnaugh.o $(CXXFLAGS)	

# Object generation from source and header for FILE
file.o: ./src/public.h ./src/public.c ./src/bool.h ./src/bool.c ./src/file.h ./src/file.c ./src/user.h ./src/user.c
	$(CC) $(CFLAGS) -c ./src/file.c -o file.o $(CXXFLAGS)	

clean:
	rm -f *.o main test_unit

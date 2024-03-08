# Compilateur utilisé
CC = gcc

# Options de compilation
CFLAGS = -g -w -Wall

# Options supplémentaires pour la compilation
CXXFLAGS = -Ilib -Isrc -Itest

# Chemins de recherche pour les fichiers source
VPATH = lib:src:test

# Bibliothèque à lier
LIB = -lm


# Construction de l'exécutable principal 'main' à partir des fichiers objet main.o et tools.o
main: main.o tools.o
	$(CC) $(CFLAGS) -o main main.o tools.o $(LIB)

# Construction de l'exécutable de tests 'test_unit' à partir des fichiers objet test_unit.o et tools.o
test_unit: test_unit.o tools.o
	$(CC) $(CFLAGS) -o test_unit test_unit.o tools.o $(CXXFLAGS) $(LIB)

# Règles de compilation pour les fichiers objet

# Règle pour générer test_unit.o à partir du fichier source ./test/test_unit.c et son en-tête
test_unit.o: ./test/test_unit.c ./test/test_unit.h
	$(CC) $(CFLAGS) -c ./test/test_unit.c $(CXXFLAGS)

# Règle pour générer tools.o à partir du fichier source ./src/tools.c et son en-tête
tools.o: ./src/tools.h ./src/tools.c
	$(CC) $(CFLAGS) -c ./src/tools.c -o tools.o $(CXXFLAGS)

# Règle pour générer main.o à partir du fichier source ./src/main.c et les en-têtes associés
main.o: ./src/tools.h ./src/tools.c ./src/main.c
	$(CC) $(CFLAGS) -c ./src/main.c -o main.o $(CXXFLAGS)

# Cible 'clean' pour supprimer les fichiers objets et les exécutables générés
clean:
	rm -f *.o main test_unit

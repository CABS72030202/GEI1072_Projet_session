#include "../src/public.h"

const int max_vars_count = 4;

void exitError(char* error) {
    printf("\n\n\nERREUR \"%s\". FIN DU PROGRAMME\n\n\n", error);
    exit(1);
}
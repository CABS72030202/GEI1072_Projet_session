#ifndef KARNAUGH_H
#define KARNAUGH_H
#include "../src/public.h"
#include "../src/bool.h"

/*  Functions used to simplify a boolean
    expression using a Karnaugh map 
*/

typedef struct {               // Linked list of all the consecutive ones found in the Karnaugh map
    int cell_count;
    char* var_state;
    struct K_Group* prev;
    struct K_Group* next;
} K_Group;

void simplify_eq(Equation*);
int** generate_karnaugh_map(Equation*);
void calc_karnaugh_size(int[], int);                // Function that returns an array defining the required size (rows * columns) of a Karnaugh map from the number of variables in the boolean equation 

#endif
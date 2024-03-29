#ifndef KARNAUGH_H
#define KARNAUGH_H

/*  Functions used to simplify a boolean
    expression using a Karnaugh map 
*/

typedef struct {               // Linked list of all the ones found in the Karnaugh map
    int cell_count;
    char* var_state;
    struct Cell* prev;
    struct Cell* next;
} Cell;

#endif
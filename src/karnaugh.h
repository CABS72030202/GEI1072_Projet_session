#ifndef KARNAUGH_H
#define KARNAUGH_H
#include "../src/public.h"
#include "../src/bool.h"

/*  Functions used to simplify a boolean
    expression using a Karnaugh map 
*/

struct cell {               // Linked list of all the consecutive ones found in the Karnaugh map
    char* cell_exp;
    struct cell* next;
};

typedef struct cell Cell;

Equation simplified_eq(Equation*);
char* karnaugh_algorithm(Equation*);                // Function that returns a simplified boolean expression by using a Karnaugh map
int** generate_karnaugh_map(Equation*, int[]);
void calc_karnaugh_size(int[], int);                // Function that returns an array defining the required size (rows * columns) of a Karnaugh map from the number of variables in the boolean equation 
void get_fill_order(int[], int);                    // Function that fills up an array defining the order of each cell's S column index in a truth table to properly fill a Karnaugh map depending on the number of variables
char* get_cell_exp(int, int, int);                  // Function that returns the single term of the SOP of the corresponding cell in a Karnaugh map from its position and the number of variables

// Linked list manipulations
void add_back(Cell*, char*);                  // Add a Cell at the end of the linked list
void add_front(Cell**, char*);                 // Add a Cell at the beginning of the linked list
char* pop_back(Cell*);                              // Remove the last Cell from the linked list
char* pop_front(Cell**);                               // Remove the first Cell from the linked list
char* pop_at(Cell**, int);                           // Remove a Cell at a specific index from the linked list
void print_list(Cell*);                             // Print a linked list

#endif
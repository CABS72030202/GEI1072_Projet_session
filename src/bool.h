#ifndef BOOL_H
#define BOOL_H

/*  Functions used to define all forms 
    of a boolean expression  
*/

typedef struct {
    int var_count;
    int** truth_table;
    char* bool_exp;
} Equation;

// Static variable
static Equation current_eq;

// Functions
void initialize_from_TT(int, int**);
void initialize_from_BE(int, char*);
char* convert_TT_to_BE(int, int**);
int** convert_BE_to_TT(int, char*);
void print_truth_table(int, int**);
void print_truth_line(int, int, int**);
void print_truth_header(int);
void print_bool_exp(char*);

#endif
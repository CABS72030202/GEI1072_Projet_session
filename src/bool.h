#ifndef BOOL_H
#define BOOL_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*  Functions used to define and manipulate
     all forms of a boolean expression  
*/

typedef struct {
    int var_count;                              // Number of variables in the boolean equation
    int** truth_table;                          // 2D int array defining the truth table of the boolean equation
    char* bool_exp;                             // String defining the boolean expression
} Equation;

// Global variable
extern Equation current_eq;                     // Global variable that stocks the current boolean equation

// Functions
void initialize_from_TT(int, int**);            // Initialize every attributes of the global variable "current_eq" knowing the truth table
void initialize_from_BE(int, char*);            // Initialize every attributes of the global variable "current_eq" knowing the boolean expression
char* format_BE(char*, char[]);                 // Formats the boolean expression by removing unecessary characters and adding required ones 
char* convert_TT_to_BE(int, int**);             // Define the boolean expression of an equation with the associated truth table
char* sum_of_products(int, int**);
char* product_of_sums(int, int**);
int required_size(int, int**, char*);           // Calculates the number of slots required in a char array for a boolean expression of form SOP or POS
int** convert_BE_to_TT(int, char*);             // Define the truth table of an equation with the associated boolean expression
int calc_TT_line(int, int[], char*);            // Calculate the value of S from a specific boolean combination for a boolean expression
void print_truth_table(int, int**, FILE*);      // Print a truth table with its header to a specific I/O stream
void print_truth_line(int, int, int**, FILE*);  // Print a specific line of a truth table to a specific I/O stream
void print_truth_header(int, FILE*);            // Print the header of a truth table to a specific I/O stream
void print_bool_exp(char*, FILE*);              // Print a boolean expression to a specific I/O stream

#endif
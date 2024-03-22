#ifndef BOOL_H
#define BOOL_H

/*  Functions used to define and manipulate
     all forms of a boolean expression  
*/

typedef struct {
    int var_count;                      // Number of variables in the boolean equation
    int** truth_table;                  // 2D int array defining the truth table of the boolean equation
    char* bool_exp;                     // String defining the boolean expression
} Equation;

// Global variable
extern Equation current_eq;             // Global variable that stocks the current boolean equation

// Functions
void initialize_from_TT(int, int**);    // Initialize every attributes of the global variable "current_eq" knowing the truth table
void initialize_from_BE(int, char*);    // Initialize every attributes of the global variable "current_eq" knowing the boolean expression
char* format_BE(char*, char[]);         // Formats the boolean expression by removing unecessary characters and adding required ones 
char* convert_TT_to_BE(int, int**);     // Define the boolean expression of an equation with the associated truth table
char* sum_of_products(int, int**);
char* product_of_sums(int, int**);
int** convert_BE_to_TT(int, char*);     // Define the truth table of an equation with the associated boolean expression
int calc_TT_line(int, int[], char*);    // Calculate the value of S from a specific boolean combination for a boolean expression
void print_truth_table(int, int**);     // Print a truth table with its header
void print_truth_line(int, int, int**); // Print a specific line of a truth table
void print_truth_header(int);           // Print the header of a truth table
void print_bool_exp(char*);             // Print a boolean expression

#endif
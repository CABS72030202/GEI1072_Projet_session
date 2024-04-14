#ifndef CALC_H
#define CALC_H
#include "../src/public.h"

/*  Stack-based calculator to evaluate
    boolean expression of string type */

extern const int MAX_STACK_SIZE;                // Constant defining the maximum size of temporay arrays used for the stack 

char* preprocess_exp(char*);                    // Function to preprocess the boolean expression ta handle (A)(B) and A(B) forms
int eval_exp(char*);                            // Function to evaluate the expression
int perform_op(int[], int*, char[], int*);      // Function to perform an operation
int precedence(char op);                        // Function to check precedence of operators

// Boolean operations
int or(int, int);                               // +
int and(int, int);                              // .
int xor(int, int);                              // @
int not(int);                                   // '

#endif
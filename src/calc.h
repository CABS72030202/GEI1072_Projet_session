#ifndef CALC_H
#define CALC_H

/*  Stack-based calculator  */

char* preprocess_exp(char*);                      // Function to preprocess the boolean expression ta handle (A)(B) and A(B) forms
int eval_exp(char*);                              // Function to evaluate the expression
int perform_op(int[], int*, char[], int*);        // Function to perform an operation
int precedence(char op);                          // Function to check precedence of operators

// Boolean operations
int or(int, int);                               // +
int and(int, int);                              // .
int xor(int, int);                              // @
int not(int);                                   // '

#endif
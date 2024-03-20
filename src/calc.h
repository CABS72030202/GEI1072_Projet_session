#ifndef CALC_H
#define CALC_H

/*  Stack-based calculator  */

void preprocess_exp(char *expr);      // Function to preprocess the expression to handle (A)(B) form
int eval_exp(char *expr);             // Function to evaluate the expression
int precedence(char op);              // Function to check precedence of operators

#endif
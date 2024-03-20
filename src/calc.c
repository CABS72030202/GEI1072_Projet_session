#include "../src/calc.h"
#include "../src/public.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int eval_exp(char *expr) {
    // Stack to store operands (0 or 1)
    int stack[100];
    int top = -1;

    // Stack to store operators
    char opStack[100];
    int opTop = -1;

    // Loop through the expression
    while (*expr) {
        // If the current character is a digit (0 or 1), extract the operand
        if (*expr == '0' || *expr == '1') {
            int operand = *expr - '0';
            stack[++top] = operand;
            expr++;
        }
        // If the current character is an opening parenthesis
        else if (*expr == '(') {
            opStack[++opTop] = *expr;
            expr++;
        }
        // If the current character is a closing parenthesis
        else if (*expr == ')') {
            // Perform operations until an opening parenthesis is encountered
            while (opTop >= 0 && opStack[opTop] != '(') {
                perform_op(stack, &top, opStack, &opTop);
            }
            opTop--; // Pop the opening parenthesis
            expr++;
        }
        // If the current character is an operator
        else if (*expr == '+' || *expr == '.' || *expr == '\'' || *expr == '@') {
            // Perform operations until the top operator has higher precedence or the operator stack is empty
            while (opTop >= 0 && precedence(opStack[opTop]) >= precedence(*expr)) {
                perform_op(stack, &top, opStack, &opTop);
            }
            opStack[++opTop] = *expr;
            expr++;
        }
        else {
            expr++; // Ignore whitespace or invalid characters
        }
    }

    // Perform remaining operations
    while (opTop >= 0) {
        perform_op(stack, &top, opStack, &opTop);
    }

    // Return the final result
    return stack[top];
}

int perform_op(int stack[], int *top, char opStack[], int *opTop) {
    int result = 0;
    int b = stack[(*top)--];
    int a = 0;

    // For NOT operator, only one operand is needed
    if (opStack[*opTop] != '\'') {
        a = stack[(*top)--];
    }

    char op = opStack[(*opTop)--];
    switch (op) {
        case '+':
            result = or(a, b);
            break;
        case '.':
            result = and(a, b);
            break;
        case '\'':
            result = not(b);
            break;
        case '@':
            result = xor(a, b);
            break;
    }
    stack[++(*top)] = result;
    return result;
}

int precedence(char op) {
    switch (op) {
        case '+':
            return 1;
        case '.':
            return 2;
        case '\'':
            return 3;
        case '@':
            return 4;
        default:
            return 0;
    }
}

// Boolean operations
int or(int a, int b) { return a | b; }

int and(int a, int b) { return a & b; }

int xor(int a, int b) { return a ^ b; }

int not(int a) { return abs(a - 1); }
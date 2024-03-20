#include "../src/calc.h"
#include "../src/public.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void preprocess_exp(char *expr) {

    // Remove newline character from the end of the input
    if (expr[strlen(expr) - 1] == '\n') {
        expr[strlen(expr) - 1] = '\0';
    }

    char *src = expr;
    char *dest = expr;
    char prev = '\0';

    while (*src != '\0') {
        if (*src == '(' && prev == ')') {
            *dest++ = '*'; // Insert a multiplication operator between ')' and '('
            src++;
        }
        *dest++ = *src;
        prev = *src++;
    }
    *dest = '\0'; // Null-terminate the modified string
}

int eval_exp(char *expr) {
    // Stack to store operands
    int stack[100];
    int top = -1;

    // Stack to store operators
    char opStack[100];
    int opTop = -1;

    // Loop through the expression
    while (*expr) {
        // If the current character is a digit, extract the number
        if (isdigit(*expr)) {
            char *end;
            int num = strtod(expr, &end);
            stack[++top] = num;
            expr = end;
            
            // If there is an opening parenthesis right after a number, insert a multiplication operator
            if (*end == '(') {
                opStack[++opTop] = '*';
            }
        }
        // If the current character is an operator
        else if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/') {
            // Perform operations until the top operator has higher precedence or the operator stack is empty
            while (opTop >= 0 && precedence(opStack[opTop]) >= precedence(*expr)) {
                // Perform calculation
                int b = stack[top--];
                int a = stack[top--];
                char op = opStack[opTop--];
                switch (op) {
                    case '+':
                        stack[++top] = a + b;
                        break;
                    case '-':
                        stack[++top] = a - b;
                        break;
                    case '*':
                        stack[++top] = a * b;
                        break;
                    case '/':
                        stack[++top] = a / b;
                        break;
                }
            }
            opStack[++opTop] = *expr;
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
                // Perform calculation
                int b = stack[top--];
                int a = stack[top--];
                char op = opStack[opTop--];
                switch (op) {
                    case '+':
                        stack[++top] = a + b;
                        break;
                    case '-':
                        stack[++top] = a - b;
                        break;
                    case '*':
                        stack[++top] = a * b;
                        break;
                    case '/':
                        stack[++top] = a / b;
                        break;
                }
            }
            opTop--; // Pop the opening parenthesis
            expr++;
        }
        else {
            expr++; // Ignore whitespace or invalid characters
        }
    }

    // Perform remaining operations
    while (opTop >= 0) {
        // Perform calculation
        int b = stack[top--];
        int a = stack[top--];
        char op = opStack[opTop--];
        switch (op) {
            case '+':
                stack[++top] = a + b;
                break;
            case '-':
                stack[++top] = a - b;
                break;
            case '*':
                stack[++top] = a * b;
                break;
            case '/':
                stack[++top] = a / b;
                break;
        }
    }

    // Return the result
    return stack[top];
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}
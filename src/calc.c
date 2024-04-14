#include "../src/calc.h"

const int MAX_STACK_SIZE = 100;

/* 
    Function: preprocess_exp
    Description: Preprocesses the boolean expression to handle (A)(B) and A(B) forms.
    Parameters:
        - expr: String defining the boolean expression.
    Returns: The preprocessed boolean expression.
*/
char* preprocess_exp(char* expr) {
    // Allocate memory for the processed expression
    char* processed = (char*)malloc(2 * strlen(expr) * sizeof(char));
    if (processed == NULL) 
        exit_error("preprocess_exp | calc.c");

    int index = 0;  // Index to track the position in the processed expression
    int pos = 1;    // Index to track the relative position when analyzing further in the expression

    while (*expr) {
        // Copy current character to the processed expression
        processed[index++] = *expr;

        // If the current character is a digit (0 or 1) or a closing parenthesis
        if (isdigit(*expr) || *expr == ')') {
            // If the next character is a ', find the relative position of the next character or digit or opening parenthesis and copy all ' characters to the processed expression
            while (*(expr + pos) == '\'') {
                processed[index++] = '\'';
                pos++;
            }
            // If the next character is a digit (0 or 1) or an opening parenthesis, insert a dot
            if (isdigit(*(expr + pos)) || *(expr + pos) == '(')
                processed[index++] = '.';
        }
        // Move to the next character in the original expression
        expr += pos;

        // Reset the relative position
        pos = 1;
    }

    // Null-terminate the processed expression
    processed[index] = '\0';

    // Return the preprocessed expression
    return processed;
}

/* 
    Function: eval_exp
    Description: Evaluates the boolean expression.
    Parameters:
        - expr: String defining the boolean expression.
    Returns: The result of the evaluation (0 or 1).
*/
int eval_exp(char *expr) {
    expr = preprocess_exp(expr);
    // Stack to store operands (0 or 1)
    int stack[MAX_STACK_SIZE];
    int top = -1;

    // Stack to store operators
    char opStack[MAX_STACK_SIZE];
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

/* 
    Function: perform_op
    Description: Performs an operation using operands and operators.
    Parameters:
        - stack: Array representing the operand stack.
        - top: Pointer to the top of the operand stack.
        - opStack: Array representing the operator stack.
        - opTop: Pointer to the top of the operator stack.
    Returns: The result of the operation.
*/
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

/* 
    Function: precedence
    Description: Determines the precedence of an operator.
    Parameters:
        - op: Operator character.
    Returns: The precedence value of the operator.
*/
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

/* 
    Function: or
    Description: Performs the OR operation.
    Parameters:
        - a: First operand.
        - b: Second operand.
    Returns: The result of the OR operation.
*/
int or(int a, int b) { return a | b; }

/* 
    Function: and
    Description: Performs the AND operation.
    Parameters:
        - a: First operand.
        - b: Second operand.
    Returns: The result of the AND operation.
*/
int and(int a, int b) { return a & b; }

/* 
    Function: xor
    Description: Performs the XOR operation.
    Parameters:
        - a: First operand.
        - b: Second operand.
    Returns: The result of the XOR operation.
*/
int xor(int a, int b) { return a ^ b; }

/* 
    Function: not
    Description: Performs the NOT operation.
    Parameters:
        - a: Operand.
    Returns: The result of the NOT operation.
*/
int not(int a) { return abs(a - 1); }
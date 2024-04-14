#include "../src/bool.h"

Equation current_eq;

/* 
    Function: initialize_from_TT
    Description: Initializes the global variable "current_eq" based on the provided truth table.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
*/
void initialize_from_TT(int var_count, int** truth_table) {
    // Set the number of variables and truth table in the current equation
    current_eq.var_count = var_count;
    current_eq.truth_table = truth_table;
    // Convert truth table to boolean expression and set it in the current equation
    current_eq.bool_exp = convert_TT_to_BE(var_count, truth_table);
}

/* 
    Function: initialize_from_BE
    Description: Initializes the global variable "current_eq" based on the provided boolean expression.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - bool_exp: String defining the boolean expression.
*/
void initialize_from_BE(int var_count, char* bool_exp) {
    // Set the number of variables and boolean expression in the current equation
    current_eq.var_count = var_count;
    current_eq.bool_exp = bool_exp;
    // Convert boolean expression to truth table and set it in the current equation
    current_eq.truth_table = convert_BE_to_TT(var_count, bool_exp);
}

/* 
    Function: format_BE
    Description: Formats the provided boolean expression by removing unnecessary characters and adding required ones.
    Parameters:
        - old_BE: The original boolean expression.
        - valid_chars: An array of valid characters for the boolean expression.
    Returns: The formatted boolean expression.
*/
char* format_BE(char* old_BE, char valid_chars[]) {
    // Check if the string is empty
    if(strlen(old_BE) == 0)
        goto E; // Formatting error
    int is_valid = 0, parenthesis_count = 0;
    // Check the validity of characters and count parentheses
    for(int i = 0; i < strlen(old_BE); i++) {
        if(is_valid == 0 && ((int)old_BE[i] >= a_ascii && (int)old_BE[i] <= s_ascii))
            is_valid = 1;
        if(old_BE[i] == '(')
            parenthesis_count++;
        else if(old_BE[i] == ')')
            parenthesis_count--;
    }
    // Check if the expression is invalid or has incoherent parentheses
    if(!is_valid || parenthesis_count)
        goto E; // Formatting error
    // Delete spaces from the expression
    char* new_BE = delete_char(old_BE, ' ');
    // Check for invalid first or last characters
    if(is_char_valid(new_BE[strlen(new_BE) - 1], "+.(@") == 1 ||
       is_char_valid(new_BE[0], "+\'.)@") == 1)
        goto E; // Formatting error
    // Check for two consecutive operators
    for(int i = 0; new_BE[i] != '\0'; i++)
        if((new_BE[i] == '+' || new_BE[i] == '.' || new_BE[i] == '@' || new_BE[i] == '(') &&
           (new_BE[i + 1] == '+' || new_BE[i + 1] == '.' || new_BE[i + 1] == '@' || 
           new_BE[i + 1] == '\'' || new_BE[i + 1] == ')'))
            goto E; // Formatting error
    return new_BE;

E: // Formatting error
    printf("\nCONVERSION IMPOSSIBLE.\nVeuillez essayer à nouveau.\n");
    return NULL;
}

/* 
    Function: count_var_from_BE
    Description: Counts the number of variables in the provided boolean expression.
    Parameters:
        - bool_exp: String defining the boolean expression.
    Returns: The number of variables in the expression.
*/
int count_var_from_BE(char* bool_exp) {
    int var_count = 1;
    // Iterate through the expression to count unique variables
    for(int i = 0; i < strlen(bool_exp); i++)
        if ((bool_exp[i] > a_ascii && bool_exp[i] < s_ascii) &&
            bool_exp[i] > (a_ascii + var_count - 1))
                var_count = bool_exp[i] - a_ascii + 1;
    return var_count;
}

/* 
    Function: convert_TT_to_BE
    Description: Converts a truth table to a boolean expression based on the default expression type.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
    Returns: The boolean expression.
*/
char* convert_TT_to_BE(int var_count, int** truth_table) {
    switch(default_bool_exp_type) {
        case 1:
            return sum_of_products(var_count, truth_table);
            break;
        case 2:
            return product_of_sums(var_count, truth_table);
            break;
        default:
            exit_error("Undefined default_bool_exp_type | convert_TT_to_BE | bool.c");
    }
}

/* 
    Function: sum_of_products
    Description: Converts a truth table to a sum-of-products boolean expression.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
    Returns: The sum-of-products boolean expression.
*/
char* sum_of_products(int var_count, int** truth_table) {
    // Memory allocation for the boolean expression
    char* bool_exp = (char*)malloc(required_size(var_count, truth_table, "SOP") * sizeof(char));
    // Construct the boolean expression
    int index = 0, cpt = 0;
    for (int i = 0; i < pow(2, var_count); i++)
        if (truth_table[i][var_count] == 1) {
            cpt++;
            if(cpt == pow(2, var_count))    // S variable is always 1
                return "1"; 
            for (int j = 0; j < var_count; j++) {
                if (truth_table[i][j] == 0) 
                    bool_exp[index++] = tolower(a_ascii + j);
                else
                    bool_exp[index++] = toupper(a_ascii + j);
            }
            bool_exp[index++] = '+';
        }
    if(cpt == 0)    // S variable is always 0
        return "0";     
    // Replace the last '+' with null terminator
    bool_exp[index - 1] = '\0';
    // Replace lowercase letters with uppercase letters followed by '
    for(int i = 0; i < var_count; i++) {
        char old_char[] = {(a_ascii + 32) + i, '\0'};
        char new_char[] = {a_ascii + i, '\'', '\0'};
        bool_exp = replace_char(bool_exp, old_char, new_char);
    }
    return bool_exp;
}

/* 
    Function: product_of_sums
    Description: Converts a truth table to a product-of-sums boolean expression.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
    Returns: The product-of-sums boolean expression.
*/
char* product_of_sums(int var_count, int** truth_table) {
    // Memory allocation for the boolean expression
    char* bool_exp = (char*)malloc(required_size(var_count, truth_table, "POS") * sizeof(char));
    // Construct the boolean expression
    int index = 0, cpt = 0;
    for (int i = 0; i < pow(2, var_count); i++)
        if (truth_table[i][var_count] == 0) {
            cpt++;
            if(cpt == pow(2, var_count))    // S variable is always 0
                return "0";
            bool_exp[index++] = '(';
            for (int j = 0; j < var_count; j++) {
                if (truth_table[i][j] == 0) 
                    bool_exp[index++] = tolower(a_ascii + j);
                else
                    bool_exp[index++] = toupper(a_ascii + j);
                if (j < var_count - 1)
                    bool_exp[index++] = '+';
            }
            bool_exp[index++] = ')';
        }
    if(cpt == 0)    // S variable is always 1
        return "1";
    // Null terminate the string
    bool_exp[index] = '\0';
    // Replace lowercase letters with uppercase letters followed by '
    for(int i = 0; i < var_count; i++) {
        char old_char[] = {(a_ascii + 32) + i, '\0'};
        char new_char[] = {a_ascii + i, '\'', '\0'};
        bool_exp = replace_char(bool_exp, old_char, new_char);
    }
    return bool_exp;
}

/* 
    Function: required_size
    Description: Calculates the number of characters required for a boolean expression of a given type.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
        - exp_type: Type of boolean expression (SOP or POS).
    Returns: The required size of the boolean expression.
*/
int required_size(int var_count, int** truth_table, char* exp_type) {
    int size = 0;
    int op_line_size_SOP = 1; // AB+CD     -->  1 '+' per line
    int op_line_size_POS = (var_count - 1) + 2; // (A+B+C)   --> (var_count - 1) '+' and "()" per line
    switch(exp_type[0]) {
        case 'S':
            for (int i = 0; i < pow(2, var_count); i++)
                if (truth_table[i][var_count] == 1) {
                    size += op_line_size_SOP;
                    for (int j = 0; j < var_count; j++)
                        if(truth_table[i][j] == 0)
                            size += 2; // A' = 2 char
                        else if(truth_table[i][j] == 1)
                            size += 1; // A = 1 char
                }
            size--; // Account for the replacement of the last +
            break;
        case 'P':
            for (int i = 0; i < pow(2, var_count); i++)
                if (truth_table[i][var_count] == 0) {
                    size += op_line_size_POS;
                    for (int j = 0; j < var_count; j++)
                        if(truth_table[i][j] == 0)
                            size += 2; // A' = 2 char
                        else if(truth_table[i][j] == 1)
                            size += 1; // A = 1 char
                }
            break;
        default:
            exit_error("Please specify valid boolean expression type | required_size | bool.c");
    }
    return size;
}

/* 
    Function: convert_BE_to_TT
    Description: Converts a boolean expression to a truth table.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - bool_exp: String defining the boolean expression.
    Returns: The truth table corresponding to the boolean expression.
*/
int** convert_BE_to_TT(int var_count, char* bool_exp) {
    int** truth_table = (int**)malloc(pow(2, var_count) * sizeof(int*));
    // Initialize truth table rows
    for(int i = 0; i < pow(2, var_count); i++) {
        truth_table[i] = (int*)malloc((var_count + 1) * sizeof(int));
        // Initialize truth table columns
        for (int j = 0; j < var_count + 1; j++)
            // Calculate S values based on the expression
            if(j == var_count)
                truth_table[i][j] = calc_TT_line(var_count, truth_table[i], bool_exp);
            // Automatically fill the variable columns to have every possible combinaisons
            else
                truth_table[i][j] = (i >> (var_count - 1 - j)) & 1;
    }
    return truth_table;
}

/* 
    Function: convert_term_to_line
    Description: Converts a boolean expression term to a truth table row.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - term: String representing a term in the boolean expression.
    Returns: The truth table row corresponding to the term.
*/
int* convert_term_to_line(int var_count, char* term) {
    int curr_var = 0;
    // Allocate memory for the truth table row
    int* TT_line = (int*)calloc(var_count, sizeof(int)); 
    // Iterate through the term to set variable values in the row
    for(int i = 0; term[i] != '\0'; i++)
        if(isalpha(term[i])) { 
            if(i == strlen(term) - 1 || term[i + 1] != '\'') // Last character is a variable
                TT_line[curr_var] = 1; 
            curr_var++;
        }
    return TT_line;
}

/* 
    Function: calc_TT_line
    Description: Calculates the value of a truth table row for a given boolean expression.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - var_values: Array representing the values of variables in the truth table row.
        - bool_exp: String defining the boolean expression.
    Returns: The calculated value for the truth table row.
*/
int calc_TT_line(int var_count, int var_values[], char* bool_exp) {
    // Allocate memory for temporary string
    char* temp = (char*)malloc((strlen(bool_exp) + 1) * sizeof(char));
    int index = 0;
    // Substitute variable values in the expression
    for (int i = 0; bool_exp[i] != '\0'; i++) 
        if (bool_exp[i] >= a_ascii && bool_exp[i] <= (a_ascii + var_count)) 
            temp[index++] = (char)(var_values[bool_exp[i] - a_ascii] + '0'); // Convert value to character
        else 
            temp[index++] = bool_exp[i];
    temp[index] = '\0';
    // Evaluate the expression and return the result
    return eval_exp(temp);
}

/* 
    Function: print_truth_table
    Description: Prints the truth table to a specified output stream.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - truth_table: 2D array defining the truth table of the boolean equation.
        - stdio: Output stream to print the truth table.
*/
void print_truth_table(int var_count, int **truth_table, FILE *stdio) {
    // Print truth table header
    print_truth_header(var_count, stdio);
    // Print truth table rows
    for (int i = 0; i < pow(2, var_count); i++) {
        for (int j = 0; j < var_count + 1; j++) 
            fprintf(stdio, "%i\t", truth_table[i][j]);
        fprintf(stdio, "\n");
    }
}

/* 
    Function: print_truth_line
    Description: Prints a specific line of the truth table to a specified output stream.
    Parameters:
        - line: Index of the line to print.
        - last_column_index: Index of the last column in the truth table.
        - truth_table: 2D array defining the truth table of the boolean equation.
        - stdio: Output stream to print the truth table line.
*/
void print_truth_line(int line, int last_column_index, int **truth_table, FILE *stdio) {
    // Print the specified line of the truth table
    for (int i = 0; i < last_column_index + 1; i++)
        fprintf(stdio, "%i\t", truth_table[line][i]);
}

/* 
    Function: print_truth_header
    Description: Prints the header of the truth table to a specified output stream.
    Parameters:
        - var_count: Number of variables in the boolean equation.
        - stdio: Output stream to print the truth table header.
*/
void print_truth_header(int var_count, FILE *stdio) {
    int var_char = 'A';
    // Print variable labels and the result label
    fprintf(stdio, "\n");
    for (int i = 0; i < var_count + 1; i++)
        if (i == var_count)
            fprintf(stdio, "%c\n", 'S');
        else
            fprintf(stdio, "%c\t", var_char++);
}

/* 
    Function: print_bool_exp
    Description: Prints a boolean expression to a specified output stream.
    Parameters:
        - bool_exp: String defining the boolean expression.
        - stdio: Output stream to print the boolean expression.
*/
void print_bool_exp(char *bool_exp, FILE *stdio) {
    // Print the boolean expression
    fprintf(stdio, "\nS = %s\n", bool_exp);
}

/* 
    Function: compare_eq
    Description: Compares two equations to determine if they are equivalent.
    Parameters:
        - eq1: Pointer to the first equation.
        - eq2: Pointer to the second equation.
    Returns: 1 if the equations are equivalent, otherwise 0.
*/
int compare_eq(Equation* eq1, Equation* eq2) {
    if(eq1->var_count != eq2->var_count)
        return 0;
    // Update truth tables for comparison
    eq1->truth_table = convert_BE_to_TT(eq1->var_count, eq1->bool_exp);
    eq2->truth_table = convert_BE_to_TT(eq2->var_count, eq2->bool_exp);
    // Compare truth tables
    for(int i = 0; i < pow(2, eq1->var_count); i++) 
        for(int j = 0; j < eq1->var_count + 1; j++)
            if(eq1->truth_table[i][j] != eq2->truth_table[i][j])
                return 0;
    return 1;
}
#include "../src/bool.h"
 
Equation current_eq;

void initialize_from_TT(int var_count, int** truth_table) {
    current_eq.var_count = var_count;
    current_eq.truth_table = truth_table;
    current_eq.bool_exp = convert_TT_to_BE(var_count, truth_table);
}

void initialize_from_BE(int var_count, char* bool_exp) {
    current_eq.var_count = var_count;
    current_eq.truth_table = convert_BE_to_TT(var_count, bool_exp);
    current_eq.bool_exp = bool_exp;
}

char* format_BE(char* old_BE, char valid_chars[]) {
    if(strlen(old_BE) == 0)                             // Empty string
        goto E;
    int is_valid = 0, parenthesis_count = 0;
    for(int i = 0; i < strlen(old_BE); i++) {
        if(is_valid == 0 && ((int)old_BE[i] >= a_ascii && (int)old_BE[i] <= s_ascii))
            is_valid = 1;
        if(old_BE[i] == '(')
            parenthesis_count++;
        else if(old_BE[i] == ')')
            parenthesis_count--;
    }
    if(!is_valid || parenthesis_count)                  // Not any variable or incoherent parenthesis
        goto E;
    char* new_BE = delete_char(old_BE, ' ');            // Delete space
    if(is_char_valid(new_BE[strlen(new_BE) - 1], "+.(@") == 1 ||
       is_char_valid(new_BE[0], "+\'.)@") == 1)         // Invalid first or last character
        goto E;
    for (int i = 0; new_BE[i] != '\0'; i++)             // Two consecutive operators
        if ((new_BE[i] == '+' || new_BE[i] == '.' || new_BE[i] == '@' || new_BE[i] == '(') &&
            (new_BE[i + 1] == '+' || new_BE[i + 1] == '.' || new_BE[i + 1] == '@' || 
            new_BE[i + 1] == '\'' || new_BE[i + 1] == ')')) 
            goto E;
    return new_BE;

E:  // Formatting error
    printf("\nCONVERSION IMPOSSIBLE.\nVeuillez essayer à nouveau.\n");
    return NULL;
}

int count_var_from_BE(char* bool_exp) {
    int var_count = 1;
    for (int i = 0; i < strlen(bool_exp); i++)
        if ((bool_exp[i] > a_ascii && bool_exp[i] < s_ascii)   // The current character is a valid variable letter
            && bool_exp[i] > (a_ascii + var_count - 1))        // First occurence of this character
                var_count = bool_exp[i] - a_ascii + 1;
    return var_count;
}

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

char* sum_of_products(int var_count, int** truth_table) {
    // Memory allocation
    char* bool_exp = (char*)malloc(required_size(var_count, truth_table, "SOP") * sizeof(char));
    // Construct the boolean expression
    int index = 0, cpt = 0;
    for (int i = 0; i < pow(2, var_count); i++)
        if (truth_table[i][var_count] == 1) {
            cpt++;
            if(cpt == pow(2, var_count))
                return "1";
            for (int j = 0; j < var_count; j++) {
                if (truth_table[i][j] == 0) 
                    bool_exp[index++] = tolower(a_ascii + j);
                else
                    bool_exp[index++] = toupper(a_ascii + j);
            }
            bool_exp[index++] = '+';
        }
    if(cpt == 0)
        return "0";

    // Replace the last '+' with null terminator
    bool_exp[index - 1] = '\0';

    // Replace lowercase letters to its uppercase letter followed by '
    for(int i = 0; i < var_count; i++) {
        char old_char[] = {(a_ascii + 32) + i, '\0'};
        char new_char[] = {a_ascii + i, '\'', '\0'};
        bool_exp = replace_char(bool_exp, old_char, new_char); }

    return bool_exp;
}

char* product_of_sums(int var_count, int** truth_table) {
    // Memory allocation
    char* bool_exp = (char*)malloc(required_size(var_count, truth_table, "POS") * sizeof(char));
    // Construct the boolean expression
    int index = 0, cpt = 0;
    for (int i = 0; i < pow(2, var_count); i++)
        if (truth_table[i][var_count] == 0) {
            cpt++;
            if(cpt == pow(2, var_count))
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
    if(cpt == 0)
        return "1";

    // Null terminate the string
    bool_exp[index] = '\0';

    // Replace lowercase letters to its uppercase letter followed by '
    for(int i = 0; i < var_count; i++) {
        char old_char[] = {(a_ascii + 32) + i, '\0'};
        char new_char[] = {a_ascii + i, '\'', '\0'};
        bool_exp = replace_char(bool_exp, old_char, new_char); }

    return bool_exp;
}

int required_size(int var_count, int** truth_table, char* exp_type) {
    int size = 0;
    int op_line_size_SOP = 1;                         // AB+CD     -->  1 '+' per line
    int op_line_size_POS = (var_count - 1) + 2;       // (A+B+C)   --> (var_count - 1) '+' and "()" per line
    switch(exp_type[0]) {
        case 'S':
            for (int i = 0; i < pow(2, var_count); i++)
                if (truth_table[i][var_count] == 1) {
                    size += op_line_size_SOP;
                    for (int j = 0; j < var_count; j++)
                        if(truth_table[i][j] == 0)
                            size += 2;               // A' = 2 char
                        else if(truth_table[i][j] == 1)
                            size += 1;               // A = 1 char
                }
            size--;                                  // Account for the replacement of the last +
        break;
        case 'P':
            for (int i = 0; i < pow(2, var_count); i++)
                if (truth_table[i][var_count] == 0) {
                    size += op_line_size_POS;
                    for (int j = 0; j < var_count; j++)
                        if(truth_table[i][j] == 0)
                            size += 2;               // A' = 2 char
                        else if(truth_table[i][j] == 1)
                            size += 1;               // A = 1 char
                }
        break;
        default:
        exit_error("Please specify valid boolean expression type | required_size | bool.c");
    }
    return size;        
}

int** convert_BE_to_TT(int var_count, char* bool_exp) {
    int** truth_table = (int**)malloc(pow(2, var_count) * sizeof(int*));
    for(int i = 0; i < pow(2, var_count); i++) {
        truth_table[i] = (int*)malloc((var_count + 1) * sizeof(int));
        for (int j = 0; j < var_count + 1; j++)
            if(j == var_count)
                truth_table[i][j] = calc_TT_line(var_count, truth_table[i], bool_exp);
            else
                truth_table[i][j] = (i >> (var_count - 1 - j)) & 1;
    }
    return truth_table;
}

int* convert_term_to_line(int var_count, char* term) {
    int curr_var = 0;
    int* TT_line = (int*)calloc(var_count, sizeof(int)); 
    for(int i = 0; term[i] != '\0'; i++)
        if(isalpha(term[i])) { 
            if(i == strlen(term) - 1 ||             // Last character is a variable
             term[i + 1] != '\'')                   // The next character is not '          
                TT_line[curr_var] = 1; 
            curr_var++;
        }
    return TT_line;
}

int calc_TT_line(int var_count, int var_values[], char* bool_exp) {
    char* temp = (char*)malloc((strlen(bool_exp) + 1) * sizeof(char));
    int index = 0;
    for (int i = 0; bool_exp[i] != '\0'; i++) 
        if (bool_exp[i] >= a_ascii && bool_exp[i] <= (a_ascii + var_count)) 
            temp[index++] = (char)(var_values[bool_exp[i] - a_ascii] + '0');   // Convert value to character
        else 
            temp[index++] = bool_exp[i];
        temp[index] = '\0';
        return eval_exp(temp);
}

void print_truth_table(int var_count, int **truth_table, FILE *stdio) {
    print_truth_header(var_count, stdio);
    for (int i = 0; i < pow(2, var_count); i++) {
        for (int j = 0; j < var_count + 1; j++) 
            fprintf(stdio, "%i\t", truth_table[i][j]);
        fprintf(stdio, "\n");
    }
}

void print_truth_line(int line, int last_column_index, int **truth_table, FILE *stdio) {
    for (int i = 0; i < last_column_index + 1; i++)
        fprintf(stdio, "%i\t", truth_table[line][i]);
}

void print_truth_header(int var_count, FILE *stdio) {
    int var_char = 'A';
    fprintf(stdio, "\n");
    for (int i = 0; i < var_count + 1; i++)
        if (i == var_count)
            fprintf(stdio, "%c\n", 'S');
        else
            fprintf(stdio, "%c\t", var_char++);
}

void print_bool_exp(char *bool_exp, FILE *stdio) {
    fprintf(stdio, "\nS = %s\n", bool_exp);
}
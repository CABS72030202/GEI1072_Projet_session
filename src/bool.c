#include "../src/bool.h"
#include "../src/public.h"
#include <stdio.h>

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

char* convert_TT_to_BE(int var_count, int** truth_table) {

}

int** convert_BE_to_TT(int var_count, char* bool_exp) {

}

void print_truth_table(int var_count, int** truth_table) {
    print_truth_header(var_count);
    for (int i = 0; i < pow(2, var_count); i++) {
        for (int j = 0; j < var_count + 1; j++) 
            printf("%i\t", truth_table[i][j]);
        printf("\n");
    }
}

void print_truth_line(int line, int last_column_index, int** truth_table) {
    for (int i = 0; i < last_column_index + 1; i++)
        printf("%i\t", truth_table[line][i]);
}

void print_truth_header(int var_count) {
    int var_char = 65;          // ASCII value for 'A'
    printf("\n");
    for (int i = 0; i < var_count + 1; i++)
        if (i == var_count)
            printf("%c\n", 83);
        else
            printf("%c\t", var_char++);
}

void print_bool_exp(char* bool_exp) {
    printf("\n%s\n", bool_exp);
}
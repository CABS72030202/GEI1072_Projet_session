#include "../src/public.h"
#include "../src/bool.h"
#include "../src/user.h"
#include <stdio.h>
#include "math.h"
#include <ctype.h>
 
// Specific functions
void input_type_menu() {
    printf("\n--- Veuillez choisir un type de saisie parmi les suivants ---\n");
    char* choices[] = {"Table de vérité", "Expression booléenne"};
    int input = menu_selection(choices, 2);
    printf("\n--- Veuillez saisir le nombre de variables booléennes à représenter (max. %i) ---\n", max_vars_count);
    int var_count = valid_integer_input(2, max_vars_count);
    switch(input) {
        case 1:     // Truth table
        input_truth_table(var_count);
        break;
        case 2:     // Boolean expression
        input_bool_exp(var_count);
        break;
        default:
        exitError("inputTypeMenu | user.c");
    }
}

void option_menu() {
    printf("\n--- Veuillez choisir une option parmi les suivantes ---\n");
    char* choices[] = {
            "Afficher la table de vérité", 
            "Afficher l'expression actuelle", 
            "Définir le type d'affichage des expressions booléennes par défaut",
            "Déterminer une équation simplifiée équivalente", 
            "Sauvegarder l'équation dans un fichier texte", 
            "Saisir une nouvelle équation booléenne", 
            "Quitter le programme"};
    int input = menu_selection(choices, sizeof(choices) / 8);
    switch (input) {
        case 1:     // Print truth table
        print_truth_table(current_eq.var_count, current_eq.truth_table);
        break;
        case 2:     // Print boolean expression
        print_bool_exp(current_eq.bool_exp);
        break;
        case 3:     // Define default boolean expression type (SOP or POS)
        input_bool_exp_type();
        break;
        case 4:     // Simplify boolean equation
        break;
        case 5:     // Save boolean equation in a text file
        break;
        case 6:     // Define new boolean equation
        input_type_menu();
        break;
        case 7:     // Close program
        printf("\nGoodbye!\n");
        exit(0);
        break;
        default:
        exitError("optionMenu | user.c");
    }
    option_menu();
}

void input_truth_table(int var_count) {
    int** truth_table = (int**)malloc(pow(2, var_count) * sizeof(int*));
    for(int i = 0; i < pow(2, var_count); i++) {
        truth_table[i] = (int*)malloc((var_count + 1) * sizeof(int));
        for (int j = 0; j < var_count + 1; j++)
            if(j == var_count) {
                print_truth_header(var_count);
                print_truth_line(i, var_count - 1, truth_table);
                truth_table[i][j] = valid_integer_input(0, 1);
            }
            else
                truth_table[i][j] = (i >> (var_count - 1 - j)) & 1;
    }
    initialize_from_TT(var_count, truth_table);
}

void input_bool_exp(int var_count) {
    printf("\n--- Veuillez définir l'expression booléenne en utilisant uniquement les caractères suivants (le OU exclusif est représenté par '@') ---\n\t");
    for (int i = 0; i < sizeof(valid_char_operators); i++)
        printf("%c  ", valid_char_operators[i]);
    for (int i = 0; i < var_count; i++)
        printf("%c  ", a_ascii + i);
    printf("%c\n", s_ascii);
    initialize_from_BE(var_count, valid_string_input(create_valid_chars(var_count)));
}

void input_bool_exp_type() {
    char* choices[] = {"Somme de produits", "Produit de sommes"};
    printf("\n--- Veuillez définir le format par défaut pour les expressions booléennes ---\n");
    default_bool_exp_type = menu_selection(choices, 2);
    free(current_eq.bool_exp);
    current_eq.bool_exp = convert_TT_to_BE(current_eq.var_count, current_eq.truth_table);
}

// General functions
int menu_selection(char* choices[], int size) {
    int i, userInput;
    for (i = 0; i < size; ++i)
        printf("(%i) %s\n", i + 1, choices[i]);
    return valid_integer_input(1, i);
}

int valid_integer_input(int min_val, int max_val) {
    char input[128];
    int index;
    printf("==> ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &index) != 1 || !(index >= min_val && index <= max_val)) {
        printf("\nSAISIE INVALIDE.\nVeuillez essayer à nouveau.\n");
        index = valid_integer_input(min_val, max_val);
    }
    return index;
}

char* valid_string_input(char valid_chars[]) {
    char input[128];
    size_t len = 0;
    printf("==> ");
    fgets(input, sizeof(input), stdin);
    while (input[len] != '\0' && input[len] != '\n')
        len++;
    if (!is_string_valid(input, valid_chars)) {
        printf("\nSAISIE INVALIDE.\nVeuillez essayer à nouveau.\n");
        return valid_string_input(valid_chars);
    }
    char* result = (char*)malloc(len + 1);
    for (size_t i = 0; i < len; i++)
        result[i] = toupper(input[i]);
    result[len] = '\0';
    result = format_BE(result, valid_chars);
    if (result != NULL)
        return result;
    else
        return valid_string_input(valid_chars);
}

char* create_valid_chars(int var_count) {
    int total_size = strlen(valid_char_operators) + var_count + 2;
    char* valid_chars = (char*)malloc(total_size * sizeof(char));
    if (valid_chars == NULL)                        // Handle memory allocation error
        exitError("create_valid_chars | user.c");
    strcpy(valid_chars, valid_char_operators);
    valid_chars[strlen(valid_char_operators)] = 'S';
    for (int i = 0; i < var_count; i++) 
        valid_chars[strlen(valid_char_operators) + 1 + i] = a_ascii + i;
    valid_chars[total_size - 1] = '\0';             // Null-terminate the string
    return valid_chars;
}
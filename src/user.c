#include "../src/public.h"
#include "../src/bool.h"
#include "../src/user.h"
#include <stdio.h>
#include "math.h"

// Specific functions
void input_type_menu() {
    printf("\n--- Veuillez choisir un type de saisie parmi les suivants ---\n");
    char* choices[] = {"Table de vérité", "Expression booléenne"};
    int input = menu_selection(choices, 2);
    printf("\n--- Veuillez saisir le nombre de variables booléennes à représenter (max. %i) ---\n", max_vars_count);
    int var_count = valid_input(2, max_vars_count);
    switch(input) {
        case 1:     // Truth table
        input_truth_table(var_count);
        break;
        case 2:     // Boolean expression
        input_bool_exp(var_count);
        break;
        default:
        exitError("inputTypeMenu user.c");
    }
}

void option_menu() {
    printf("\n--- Veuillez choisir une option parmi les suivantes ---\n");
    char* choices[] = {
            "Afficher la table de vérité", 
            "Afficher l'expression actuelle", 
            "Déterminer une équation simplifiée équivalente", 
            "Sauvegarder l'équation dans un fichier texte", 
            "Saisir une nouvelle équation booléenne", 
            "Quitter le programme"};
    int input = menu_selection(choices, 6);
    switch (input) {
        case 1:     // Print truth table
        print_truth_table(current_eq.var_count, current_eq.truth_table);
        break;
        case 2:     // Print boolean expression
        print_bool_exp(current_eq.bool_exp);
        break;
        case 3:     // Simplify boolean equation
        break;
        case 4:     // Save boolean equation in a text file
        break;
        case 5:     // Define new boolean equation
        input_type_menu();
        break;
        case 6:     // Close program
        printf("\nGoodbye!\n");
        exit(0);
        break;
        default:
        exitError("optionMenu user.c");
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
                truth_table[i][j] = valid_input(0, 1);
            }
            else
                truth_table[i][j] = (i >> (var_count - 1 - j)) & 1;
    }
    initialize_from_TT(var_count, truth_table);
}

void input_bool_exp(int var_count) {
    
}

// General functions
int menu_selection(char* choices[], int size) {
    int i, userInput;
    for (i = 0; i < size; ++i)
        printf("(%i) %s\n", i + 1, choices[i]);
    return valid_input(1, i);
}

int valid_input(int min_val, int max_val) {
    char input[128];
    int index;
    printf("==> ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &index) != 1 || !(index >= min_val && index <= max_val)) {
        printf("\nSAISIE INVALIDE.\nVeuillez essayer à nouveau.\n");
        index = valid_input(min_val, max_val);
    }
    return index;
}
#include "../src/user.h"
#include "../src/public.h"
#include "../src/bool.h"
#include <stdio.h>

// Specific functions
void input_type_menu() {
    printf("\n--- Veuillez choisir un type de saisie parmi les suivants ---\n");
    char* choices[] = {"Table de vérité", "Expression booléenne"};
    int input = menu_selection(choices, 2);
    printf("\n--- Veuillez saisir le nombre de variables booléennes à représenter (max. %i) ---\n", max_vars_count);
    int var_count = valid_input(max_vars_count);
    switch(input) {

        case 1:     // Truth table
        int** truth_table = define_truth_table(var_count);
        break;

        case 2:     // Boolean expression
        char* bool_exp = define_bool_exp(var_count);
        break;

        default:
        exitError("inputTypeMenu() user.c");
    }
}

void option_menu() {
    printf("\n--- Veuillez choisir une option parmi les suivantes ---\n");
    char* choices[] = {"Déterminer une expression simplifiée équivalente"};
    menu_selection(choices, 1);
}

int** define_truth_table(int var_count) {
    
}

char* define_bool_exp(int var_count) {
    
}

// General functions
int menu_selection(char* choices[], int size) {
    int i, userInput;
    for (i = 0; i < size; ++i)
        printf("(%i) %s\n", i + 1, choices[i]);
    return valid_input(i);
}

int valid_input(int maxVal) {
    char input[128];
    int index;
    printf("==> ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &index) != 1 || !(index > 0 && index <= maxVal)) {
        printf("\nSAISIE INVALIDE.\nVeuillez essayer à nouveau.\n");
        index = valid_input(maxVal);
    }
    return index;
}
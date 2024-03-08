#include "../src/user.h"
#include <stdio.h>

// Specific functions
void inputTypeMenu() {
    printf("\n--- Veuillez choisir un type de saisie parmi les suivants ---\n");
    char* choices[] = {"Table de vérité", "Expression booléenne"};
    int input = menuSelection(choices, 2);
    switch(input) {

        case 1:
        break;

        case 2:
        break;

        default:
        exitError("ligne 17");
    }
}

void optionMenu() {
    printf("\n--- Veuillez choisir une option parmi les suivantes ---\n");
    char* choices[] = {"Déterminer une expression simplifiée équivalente"};
    menuSelection(choices, 1);
}

// General functions
int menuSelection(char* choices[], int size) {
    int i, userInput;
    for (i = 0; i < size; ++i)
        printf("(%i) %s\n", i + 1, choices[i]);
    return validInput(i);
}

int validInput(int maxVal) {
    char input[128];
    int index;
    printf("==> ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &index) != 1 || !(index > 0 && index <= maxVal)) {
        printf("\nSAISIE INVALIDE.\nVeuillez essayer à nouveau.\n");
        index = validInput(maxVal);
    }
    return index;
}

void exitError(char* error) {
    printf("\n\n\nERREUR \"%s\". FIN DU PROGRAMME\n\n\n", error);
    exit(1);
}
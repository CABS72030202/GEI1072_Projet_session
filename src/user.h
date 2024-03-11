#ifndef USER_H
#define USER_H

/* Functions used to interact with the user :
    - Print selection menus
    - Input validation
    - Boolean expression definition by the user
*/

// Specific functions
void input_type_menu();                 // Menu letting the user choose how the boolean equation will be initialized
void option_menu();                     // Menu letting the user choose what to do with the initialized boolean equation
void input_truth_table(int);            // Menu letting the user define the boolean equation with a truth table
void input_bool_exp(int);               // Menu letting the user define the boolean equation with an expression

// General functions
int menu_selection(char*[], int);       // Prints formatted menu choices defined in a string array and prompts the user
int valid_input(int, int);              // Prompts the user for an integer input within a specified range

#endif
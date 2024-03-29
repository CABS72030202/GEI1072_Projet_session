#ifndef USER_H
#define USER_H

/* Functions used to interact with the user :
    - Print selection menus
    - Input validation
    - Boolean expression definition by the user
*/

// Specific functions
void input_type_menu();                     // Menu letting the user choose how the boolean equation will be initialized
void option_menu();                         // Menu letting the user choose what to do next (main program loop)
void input_truth_table(int);                // Menu letting the user define the boolean equation with a truth table
void input_bool_exp(int);                   // Menu letting the user define the boolean equation with an expression
void input_bool_exp_type();                 // Menu letting the user define the default type of boolean expressions (SOP or POS) 
void save_options();                        // Menu letting the user define text file options

// Generic functions
int menu_selection(char*[], int);           // Prints formatted menu choices defined in a string array and prompts the user
int valid_integer_input(int, int);          // Prompts the user for an integer input within a specified range
char* valid_string_input(char[]);           // Prompts the user for a string input composed of specific characters
char* create_valid_chars(int);              // Creates a dynamic array defining the valid operators and the valid variables for a boolean expression 

#endif
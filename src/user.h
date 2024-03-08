#ifndef USER_H
#define USER_H

/* Functions used to interact with the user :
    - Print selection menus
    - Input validation
    - Boolean expression definition by the user
*/

// Specific functions
void input_type_menu();
void option_menu();
void input_truth_table(int);
void input_bool_exp(int);

// General functions
int menu_selection(char*[], int);
int valid_input(int, int);

#endif
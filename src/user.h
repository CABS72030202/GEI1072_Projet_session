#ifndef USER_H
#define USER_H

/* Functions used to interact with the user :
    - Print selection menus
    - Input validation
*/

// Specific functions
void inputTypeMenu();
void optionMenu();

// General functions
int menuSelection(char* choices[], int size);
int validInput(int maxVal);
void exitError();

#endif
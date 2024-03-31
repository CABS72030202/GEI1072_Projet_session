#ifndef PUBLIC_H
#define PUBLIC_H

/* Default functions and variables accessible for all files */

extern const int max_vars_count;                // Defines the maximum number of boolean variables that the program can handle
extern int default_bool_exp_type;               // Defines the default type of boolean expression used (1 = SOP ; 2 = POS)
extern const char valid_char_operators[7];      // Defines the valid characters that the user can input when defining a boolean expression
extern const int a_ascii;                       // Defines the numeral ASCII value of the capital letter 'A' (boolean variables always starts with A)
extern const int s_ascii;                       // Defines the numeral ASCII value of the capital letter 'S' (boolean variable used to reference the result of the equation)
void exit_error(char*);                         // Exit the program and print a specific message (function name | file name)
char* delete_char(char*, char);                 // Delete all instances of a specific character
char* replace_char(char*, char*, char*);        // Replace all instances of specific characters found in a string by a set of new characters
int is_string_valid(char*, char[]);             // Verifies if a string is only composed of specific characters
int is_char_valid(char, char[]);                // Verifies if a character is contained in an array
char* sub_str(int, int, char*);                 // Returns a substring created from a starting index to an ending index

#endif
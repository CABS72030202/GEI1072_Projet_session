#ifndef FILE_H
#define FILE_H

/*  Functions and variables used for
    text file management     
*/

extern char* curr_repo;                         // Variable used to define the default repository where the file will be saved
extern char* curr_file_name;                    // Variable used to define the default name of a text file
extern int curr_file_count;                     // Variable used to keep track of the number of files produced during execution

int count_files();                              // Function counting the number of files in the current repository
char* change_file_name(char*);                  // Function letting the user change the default text file name for execution duration
char* change_repo(char*);                       // Function letting the user change the default repo for generated text file for execution duration
void save_equation(Equation*);                  // Function used to save a boolean equation in a text file
char* generate_file_path();                     // Function that automatically generates a .txt file name by concatenating the curr_file_name and curr_file_count
int valid_file_path(char*);                     // Function that checks the unicity of a generated file name in the current repository

#endif
#ifndef FILE_H
#define FILE_H

/*  Functions and variables used for
    text file management     
*/

extern char* curr_repo;                         // Variable used to define the default repository where the file will be saved
extern char* curr_file_name;                    // Variable used to define the default name of a text file
extern int curr_file_count;                     // Variable used to keep track of the number of files produced during execution

void count_files();                             // Function counting the number of files in the current repository
void save_equation(Equation*);                  // Function used to save a boolean equation in a text file

#endif
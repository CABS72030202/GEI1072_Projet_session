#include "../src/file.h"

char* curr_repo = "./generated/";                   // Default repository for saving files
char* curr_file_name = "boolean_equation_";         // Default file name prefix
int curr_file_count = 0;                            // Counter for generating unique file names

// Function to count the number of files in the current repository
int count_files_in_repo() {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    if ((dir = opendir(curr_repo)) != NULL) {        // Open the directory
        while ((entry = readdir(dir)) != NULL)       // Iterate through directory entries
            if (entry->d_type == DT_REG)             // Check if it's a regular file
                count++;                             // Increment count
        closedir(dir);                               // Close the directory
    } else {
        perror("opendir");                           // Print error if directory couldn't be opened
        exit_error("count_files | file.c");          // Exit program with error message
    }
    return count;                                    // Return the count of files
}

// Function to change the default text file name for execution duration
char* change_file_name(char* new_name) {
    // Allocate memory for the new file name
    curr_file_name = (char*)malloc(strlen(new_name) * sizeof(char));
    // Convert the new name to lowercase
    for(int i = 0; new_name[i] != '\0'; i++)
        curr_file_name[i] = tolower(new_name[i]);
    return curr_file_name;                           // Return the new file name
}

// Function to change the default repository for generated text files for execution duration
char* change_repo(char* new_name) {
    // Allocate memory for the new repository name
    curr_repo = (char*)malloc(strlen(new_name) * sizeof(char));
    // Convert the new name to lowercase
    for(int i = 0; new_name[i] != '\0'; i++)
        curr_repo[i] = tolower(new_name[i]);
    DIR *dir;
    // Open the new repository directory
    if((dir = opendir(curr_repo)) == NULL) {
        printf("\nRépertoire introuvable. Veuillez réessayer.\n"); // Print error message if directory not found
        return NULL;
    }
    count_files_in_repo();                           // Update the count of files in the new repository
    return curr_repo;                                // Return the new repository name
}

// Function to save a boolean equation in a text file
void save_equation(Equation* eq, char* file_path, char* mode) {
    FILE *file;
    if ((file = fopen(file_path, mode)) == NULL)    // Open the file in specified mode
        exit_error("save_equation | file.c");       // Exit program with error message if file couldn't be opened
    // Write boolean expression and truth table to the file
    print_bool_exp(eq->bool_exp, file);
    print_truth_table(eq->var_count, eq->truth_table, file);
    fclose(file);                                   // Close the file
    printf("\nSauvegarde de l'équation S=%s dans le répertoire %s réussie.\n", eq->bool_exp, file_path); // Print success message
}

// Function to load a boolean equation from a selected text file
void load_equation(char* file_path, char* mode) {
    const int ALLOC_SIZE = 8;                       // Allocation size for dynamic memory
    FILE *file = fopen(file_path, mode);            // Open the file
    if (file == NULL) 
        exit_error("load_equation (fopen error) | file.c"); // Exit program with error message if file couldn't be opened
    char* result = (char*)malloc(ALLOC_SIZE * sizeof(char)); // Allocate memory for reading characters from file
    char curr_char;
    int eq_count = 0, char_count = 0;
    // Read characters from file until EOF
    while (curr_char != EOF) {
        curr_char = fgetc(file);
        // Count number of boolean expressions
        if(curr_char == s_ascii) {
            eq_count++;
            // Read the boolean expression until newline
            while(curr_char != '\n') {
                curr_char = fgetc(file);
                result[char_count++] = curr_char;
                // Reallocate memory if needed
                if (char_count % ALLOC_SIZE == 0)
                    result = (char*)realloc(result, (char_count + ALLOC_SIZE) * sizeof(char));
            }
        }
    }
    fclose(file);                                   // Close the file
    result[char_count++] = '\0';                    // Null-terminate the result
    result = delete_char(result, ' ');              // Delete spaces from the result
    result = delete_char(result, '\n');             // Delete newlines from the result
    char_count = strlen(result);                    // Get the length of the result
    eq_count /= 2;                                  // Divide equation count by 2
    char* eq_array[eq_count];                       // Array to store boolean expressions
    generate_eq_array(&eq_array, result);           // Generate array of boolean expressions
    char* bool_exp = choose_bool_exp(&eq_array, eq_count); // Choose boolean expression
    initialize_from_BE(count_var_from_BE(bool_exp), bool_exp); // Initialize boolean expression
    free(result);                                   // Free memory allocated for result
}

// Function to generate a file path
char* generate_file_path() {
    curr_file_count++;                              // Increment file count
    int i, new_size, j = curr_file_count;
    // Calculate the new size of file name
    for(new_size = strlen(curr_file_name); j != 0; new_size++)
        j /= 10;
    char* new_file_name = (char*)malloc(sizeof(char) * (new_size));
    // Copy the prefix of the file name
    for(i = 0; i < new_size - 1; i++)
        if(i >= strlen(curr_file_name))
            new_file_name[i] = ' ';
        else
            new_file_name[i] = curr_file_name[i];
    // Append the file count to the file name
    for(j = curr_file_count, i = 1; j != 0; i++) {
        new_file_name[new_size - i] = (char)(j % 10) + 48;
        j /= 10;
    }
    char* file_path = (char*)malloc(sizeof(char) * (strlen(curr_repo) + strlen(new_file_name) + 4));
    // Generate the file path
    sprintf(file_path, "%s%s%s", curr_repo, new_file_name, ".txt");
    // Check if file path is valid
    if(!valid_file_path(file_path))
        new_file_name = generate_file_path();
    else 
        return file_path;                           // Return the generated file path
}

// Function to generate an array containing names of all the files in the default repository
void generate_file_array(char** files) {
    DIR *dir;
    struct dirent *entry;
    int file_count = count_files_in_repo(), i = 0;
    dir = opendir(curr_repo);
    if (dir == NULL) 
        exit_error("generate_file_array (opendir error) | file.c"); // Exit program with error message if directory couldn't be opened
    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL)
        if (entry->d_type == DT_REG) {              // Check if it's a regular file
            char *dot = strrchr(entry->d_name, '.'); // Get the extension of the file
            if (dot && !strcmp(dot, ".txt")) {     // Check if it's a text file
                files[i] = (char*)malloc(strlen(entry->d_name) + strlen(curr_repo) + 1); // Allocate memory for file name
                if (files[i] == NULL)
                    exit_error("generate_file_array (allocation error) | file.c"); // Exit program with error message if memory allocation failed
                // Copy file path to the array
                sprintf(files[i], "%s%s", curr_repo, entry->d_name);
                i++;
            }
        }
    closedir(dir);                                  // Close the directory
}

// Function to generate an array containing all the boolean expressions found in a concatenated string
void generate_eq_array(char** eq_array, char* eq_str) {
    char* temp = delete_char(eq_str, '=');          // Delete '=' characters from the string
    int eq_count = strlen(eq_str) - strlen(temp);   // Count the number of equations
    int next_char = 2, prev_char = 1, curr_eq = 0;
    // Extract each boolean expression and store in the array
    while (next_char <= strlen(eq_str)) {
        if (eq_str[next_char] == '=') {
            eq_array[curr_eq++] = sub_str(prev_char, next_char, eq_str); // Extract and store the expression
            prev_char = next_char + 1;
        }
        next_char++;
    }
    eq_array[curr_eq] = sub_str(prev_char, next_char, eq_str); // Extract and store the last expression
}

// Function to check the uniqueness of a generated file name in the current repository
int valid_file_path(char* file_path) {
    FILE *file;
    if ((file = fopen(file_path, "r")) != NULL) {  // Try to open the file
        fclose(file);
        return 0;                                   // File already exists
    }
    return 1;                                       // File doesn't exist
}
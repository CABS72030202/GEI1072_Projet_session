#include "../src/public.h"
#include "../src/bool.h"
#include "../src/file.h"
#include "../src/user.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

char* curr_repo = "./generated/";
char* curr_file_name = "boolean_equation_";
int curr_file_count = 0;

int count_files_in_repo() {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    if ((dir = opendir(curr_repo)) != NULL) {
        while ((entry = readdir(dir)) != NULL) 
            if (entry->d_type == DT_REG)
                count++;
        closedir(dir);
    } else {
        perror("opendir");
        exit_error("count_files | file.c");
    }
    return count;
}

char* change_file_name(char* new_name) {
    curr_file_name = (char*)malloc(strlen(new_name) * sizeof(char));
    for(int i = 0; new_name[i] != '\0'; i++)
        curr_file_name[i] = tolower(new_name[i]);
    return curr_file_name;
}

char* change_repo(char* new_name) {
    curr_repo = (char*)malloc(strlen(new_name) * sizeof(char));
    for(int i = 0; new_name[i] != '\0'; i++)
        curr_repo[i] = tolower(new_name[i]);
    DIR *dir;
    if((dir = opendir(curr_repo)) == NULL) {
        printf("\nRépertoire introuvable. Veuillez réessayer.\n");
        return NULL;
    }
    count_files_in_repo();
    return curr_repo;
}

void save_equation(Equation* eq, char* file_path, char* mode) {
    FILE *file;
    if ((file = fopen(file_path, mode)) == NULL) 
        exit_error("save_equation | file.c");
    print_bool_exp(eq->bool_exp, file);
    print_truth_table(eq->var_count, eq->truth_table, file);
    fclose(file);
    printf("\nSauvegarde de l'équation S=%s dans le répertoire %s réussie.\n", eq->bool_exp, file_path);
}

void load_equation(char* file_path, char* mode) {
    const int ALLOC_SIZE = 8;
    FILE *file = fopen(file_path, mode);
    if (file == NULL) 
        exit_error("load_equation (fopen error) | file.c");
    char* result = (char*)malloc(ALLOC_SIZE * sizeof(char));
    char curr_char;
    int eq_count = 0, char_count = 0;
    while (curr_char != EOF) {
        curr_char = fgetc(file);
        if(curr_char == s_ascii) {
            eq_count++;
            while(curr_char != '\n') {
                curr_char = fgetc(file);
                result[char_count++] = curr_char;
                if (char_count % ALLOC_SIZE == 0)
                    result = (char*)realloc(result, ((char_count / ALLOC_SIZE) * ALLOC_SIZE) * sizeof(char));
            }
        }
    }
    fclose(file);
    result[char_count++] = '\0';
    result = delete_char(result, ' ');
    result = delete_char(result, '\n');
    char_count = strlen(result);
    eq_count /= 2;


    if (result != NULL) {
        printf("Concatenated lines starting with 'S':\n%s\nFinal size:%i\nEq_count:%i\n", result, char_count, eq_count);
    } else
        printf("No lines starting with 'S' found.\n");
    

    char* eq_array[eq_count];
    /*generate_eq_array(&eq_array, result);
    char* bool_exp = choose_bool_exp(&eq_array, eq_count);
    initialize_from_BE(count_var_from_BE(bool_exp), bool_exp);*/
    free(result);
}

char* generate_file_path() {
    curr_file_count++;
    int i, new_size, j = curr_file_count;
    for(new_size = strlen(curr_file_name); j != 0; new_size++)
        j /= 10;
    char* new_file_name = (char*)malloc(sizeof(char) * (new_size));
    for(i = 0; i < new_size - 1; i++)
        if(i >= strlen(curr_file_name))
            new_file_name[i] = ' ';
        else
            new_file_name[i] = curr_file_name[i];
    for(j = curr_file_count, i = 1; j != 0; i++) {
        new_file_name[new_size - i] = (char)(j % 10) + 48;
        j /= 10;
    }
    char* file_path = (char*)malloc(sizeof(char) * (strlen(curr_repo) + strlen(new_file_name) + 4));
    sprintf(file_path, "%s%s%s", curr_repo, new_file_name, ".txt");
    if(!valid_file_path(file_path))
        new_file_name = generate_file_path();
    else 
        return file_path;
}

void generate_file_array(char** files) {
    DIR *dir;
    struct dirent *entry;
    int file_count = count_files_in_repo(), i = 0;
    dir = opendir(curr_repo);
    if (dir == NULL) 
        exit_error("generate_file_array (opendir error) | file.c");
    while ((entry = readdir(dir)) != NULL)
        if (entry->d_type == DT_REG) {
            char *dot = strrchr(entry->d_name, '.');
            if (dot && !strcmp(dot, ".txt")) {
                files[i] = (char*)malloc(strlen(entry->d_name) + strlen(curr_repo) + 1);
                if (files[i] == NULL)
                    exit_error("generate_file_array (allocation error) | file.c");
                sprintf(files[i], "%s%s", curr_repo, entry->d_name);
                i++;
            }
        }
    closedir(dir);
}

void generate_eq_array(char** eq_array, char* eq_str) {
    
}

int valid_file_path(char* file_path) {
    FILE *file;
    if ((file = fopen(file_path, "r")) != NULL) {       // Try to open the file
        fclose(file);
        return 0;                                       // File already exists
    }
    return 1;                                           // File doesn't exist
}
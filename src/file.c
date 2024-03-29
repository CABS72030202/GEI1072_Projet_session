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

int count_files() {
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
        exitError("count_files | file.c");
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
    count_files();
    return curr_repo;
}

void save_equation(Equation* eq) {
    char* file_name = generate_file_name();
}

char* generate_file_name() {
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
    if(!valid_file_name(new_file_name))
        new_file_name = generate_file_name();
    else
        return new_file_name;
}

int valid_file_name(char* file_name) {
    return 1;
}
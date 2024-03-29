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

void count_files() {
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(curr_repo)) != NULL) {
        while ((entry = readdir(dir)) != NULL) 
            if (entry->d_type == DT_REG)
                curr_file_count++;
        closedir(dir);
    } else {
        perror("opendir");
        exitError("count_files | file.c");
    }
}

void save_equation(Equation* eq) {
    
}
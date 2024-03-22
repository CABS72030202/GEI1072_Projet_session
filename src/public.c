#include "../src/public.h"
#include <stdio.h>

const int max_vars_count = 4;
//const int default_bool_exp_type = 1;    // 1 = SOP
const int default_bool_exp_type = 2;    // 2 = POS
const char valid_char_operators[7] = {'+', '\'', '.', '(', ')', '@', ' '};
const int a_ascii = 65;
const int s_ascii = 83;

void exitError(char* error) {
    printf("\n\n\nERREUR \"%s\". FIN DU PROGRAMME\n\n\n", error);
    exit(1);
}

char* delete_char(char old_str[], char delete) {
    int old_size, new_size = 0;
    for (old_size = 1; old_str[old_size - 1] != '\0'; old_size++)
        if (old_str[old_size - 1] == delete)
            new_size++;
    if (new_size == 0)
        return old_str;
    else {
        new_size = old_size - new_size;
        char* new_str = (char*)malloc(new_size * sizeof(char));
        for (int i = 0, j = 0; i < old_size; i++)
            if (old_str[i] != delete) {
                new_str[j] = old_str[i];
                j++;
            }
        return new_str;
    }
}

char* replace_char(char* str, char* old_char, char* new_char) {
    int str_len = 0, old_len = 0, new_len = 0, count = 0, i, j, k;
    
    // Calculate lengths
    while (str[str_len] != '\0') {
        str_len++;
    }
    while (old_char[old_len] != '\0') {
        old_len++;
    }
    while (new_char[new_len] != '\0') {
        new_len++;
    }
    
    // Count occurrences
    for (i = 0; i <= str_len - old_len; i++) {
        for (j = 0; j < old_len; j++) {
            if (str[i + j] != old_char[j]) {
                break;
            }
        }
        if (j == old_len) {
            count++;
        }
    }
    
    // Calculate new length
    int new_str_len = str_len + count * (new_len - old_len);
    
    // Create new string
    char* new_str = (char*)malloc(sizeof(char) * (new_str_len + 1));
    if (new_str == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Replace characters
    for (i = 0, j = 0; i < str_len; ) {
        for (k = 0; k < old_len; k++) {
            if (str[i + k] != old_char[k]) {
                new_str[j++] = str[i++];
                break;
            }
        }
        if (k == old_len) {
            for (k = 0; k < new_len; k++) {
                new_str[j++] = new_char[k];
            }
            i += old_len;
        }
    }
    new_str[j] = '\0';
    return new_str;
}

int is_string_valid(char* str, char valid_chars[]) {
    size_t len = 0;
    while (str[len] != '\0' && str[len] != '\n')
        len++;
    for (size_t i = 0; i < len; i++) {
        int charIsValid = 0;
        for (size_t j = 0; valid_chars[j] != '\0'; j++) 
            if (toupper(str[i]) == toupper(valid_chars[j])) {
                charIsValid = 1;
                break;
            }  
        if (!charIsValid) 
            return 0;
    }
    return 1;
}

int is_char_valid(char c, char valid_chars[]) {
    for(int i = 0; i < strlen(valid_chars); i++)
        if(valid_chars[i] == c) 
            return 1;
    return 0;
}
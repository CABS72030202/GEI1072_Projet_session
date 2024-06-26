#include "../src/public.h"

const int max_vars_count = 4;
int default_bool_exp_type = 1;          // 1 = SOP (default), 2 = POS 
const char valid_char_operators[7] = {'+', '\'', '.', '(', ')', '@', ' '};
const int a_ascii = 65;
const int s_ascii = 83;

/* 
    Function: exit_error
    Description: Print a given error message and exit the program.
    Parameters:
        - error: Error message to display. Typical format should be "[function where the error occured] | [file containing the function implementation]"
    Returns: None.
*/
void exit_error(char* error) {
    printf("\n\n\nERROR OCCURED HERE : \"%s\". END OF PROGRAM.\n\n\n", error);
    exit(1);
}

char* delete_char(char* old_str, char delete) {
    int old_size, new_size = 0;
    // Calculate new size after deleting characters
    for (old_size = 1; old_str[old_size - 1] != '\0'; old_size++)
        if (old_str[old_size - 1] == delete)
            new_size++;
    // If no characters to delete, return original string
    if (new_size == 0)
        return old_str;
    else {
        new_size = old_size - new_size;
        // Allocate memory for new string
        char* new_str = (char*)malloc(new_size * sizeof(char));
        // Copy characters from old string to new string, excluding deleted characters
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
    
    // Calculate lengths of input strings
    while (str[str_len] != '\0') {
        str_len++;
    }
    while (old_char[old_len] != '\0') {
        old_len++;
    }
    while (new_char[new_len] != '\0') {
        new_len++;
    }
    
    // Count occurrences of old_char in str
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
    
    // Calculate length of new string
    int new_str_len = str_len + count * (new_len - old_len);
    
    // Allocate memory for new string
    char* new_str = (char*)malloc(sizeof(char) * (new_str_len + 1));
    if (new_str == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Replace old_char with new_char in str
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
    new_str[j] = '\0'; // Add null terminator
    return new_str;
}

int is_string_valid(char* str, char valid_chars[]) {
    size_t len = 0;
    // Calculate length of input string
    while (str[len] != '\0' && str[len] != '\n')
        len++;
    // Check if each character in string is valid
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
    // Check if input character is valid
    for(int i = 0; i < strlen(valid_chars); i++)
        if(valid_chars[i] == c) 
            return 1;
    return 0;
}

char* sub_str(int start_pos, int end_pos, char* str) {
    int length = end_pos - start_pos;
    // Allocate memory for substring
    char* sub = (char*)malloc((length + 1) * sizeof(char));     // Add 1 for the null terminator
    if (sub == NULL) 
        printf("Memory allocation failed\n");
    else {
        // Copy characters from str to sub
        for (int i = 0; i < length; i++) 
            sub[i] = str[start_pos + i];
        sub[length] = '\0';                                     // Null-terminate the substring
    }
    return sub;
}

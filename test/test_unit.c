#include "../test/test_unit.h"

const int ignore_check_format_BE = 1;
void check_format_BE(void) { if(!ignore_check_format_BE) {
    char valid_chars[] = "+'.()@ ABCDS";

    // Empty string
    TEST_CHECK(format_BE("", valid_chars) == NULL);

    // Not any variable
    TEST_CHECK(format_BE("+ . @", valid_chars) == NULL);
    TEST_CHECK(strcmp(format_BE("A", valid_chars), "A") == 0);

    // Incoherent parenthesis
    TEST_CHECK(format_BE("(A+B", valid_chars) == NULL);
    TEST_CHECK(format_BE("(A+B)+(B+C))", valid_chars) == NULL);
    TEST_CHECK(strcmp(format_BE("A+(B+C)", valid_chars), "A+(B+C)") == 0);
    TEST_CHECK(strcmp(format_BE("A.B+(C.D+A)(A+C)", valid_chars), "A.B+(C.D+A)(A+C)") == 0);

    // notalid first or last character
    TEST_CHECK(format_BE(".A+B", valid_chars) == NULL);
    TEST_CHECK(format_BE("(A+B)+", valid_chars) == NULL);
    TEST_CHECK(format_BE("'A+B", valid_chars) == NULL);
    TEST_CHECK(format_BE("(A+B)@", valid_chars) == NULL);
    TEST_CHECK(strcmp(format_BE("A'+(B+C)'", valid_chars), "A'+(B+C)'") == 0);

    // Two consecutive operators
    TEST_CHECK(format_BE("A++B", valid_chars) == NULL);
    TEST_CHECK(format_BE("(A+B+)", valid_chars) == NULL);
    TEST_CHECK(format_BE("A+'B", valid_chars) == NULL);
    TEST_CHECK(format_BE("(A+B)@(A+.C)", valid_chars) == NULL);
} else {printf("Ignored test ");}
}

const int ignore_check_delete_char = 0;
void check_delete_char(void) { if(!ignore_check_delete_char) {
    TEST_CHECK(strcmp(delete_char("A+B", '+'), "AB") == 0);
} else {printf("Ignored test ");}
}

const int ignore_check_replace_char = 0;
void check_replace_char(void) { if(!ignore_check_replace_char) {
    TEST_CHECK(strcmp(replace_char("Hello, world!", "world", "planet"), "Hello, planet!") == 0);    // strlen(old_char) <= strlen(new_char)
    TEST_CHECK(strcmp(replace_char("Hello, world!", "world", "earth"), "Hello, earth!") == 0);      // strlen(old_char) == strlen(new_char)
    TEST_CHECK(strcmp(replace_char("Hello, world!", "world", "you"), "Hello, you!") == 0);          // strlen(old_char) >= strlen(new_char)
    TEST_CHECK(strcmp(replace_char("Ab", "b", "B'"), "AB'") == 0);

    char* test = "ab";
    for(int i = 0; i < 2; i++) {
        char old_char[] = {'a' + i, '\0'};
        char new_char[] = {'A' + i, '\'', '\0'};
        test = replace_char(test, old_char, new_char); }
    TEST_CHECK(strcmp(test, "A'B'") == 0);

    test = "aBC+AbC+ABc";
    for(int i = 0; i < 3; i++) {
        char old_char[] = {'a' + i, '\0'};
        char new_char[] = {'A' + i, '\'', '\0'};
        test = replace_char(test, old_char, new_char); }
    TEST_CHECK(strcmp(test, "A'BC+AB'C+ABC'") == 0);
} else {printf("Ignored test ");}
}

const int ignore_check_is_string_valid = 0;
void check_is_string_valid(void) { if(!ignore_check_is_string_valid) {
    TEST_CHECK(is_string_valid("A", "ABC") == 1);
    TEST_CHECK(is_string_valid("B", "ABC") == 1);
    TEST_CHECK(is_string_valid("C", "ABC") == 1);
    TEST_CHECK(is_string_valid("A+B+C", "A") == 0);
} else {printf("Ignored test ");}
}

const int ignore_check_boolean_operations = 0;
void check_boolean_operations(void) { if(!ignore_check_boolean_operations) {
    TEST_CHECK(or(0,0) == 0);
    TEST_CHECK(and(0,0) == 0);
    TEST_CHECK(xor(0,0) == 0);
    TEST_CHECK(not(0) == 1);
    TEST_CHECK(or(0,1) == 1);
    TEST_CHECK(and(0,1) == 0);
    TEST_CHECK(xor(0,1) == 1);
    TEST_CHECK(or(1,0) == 1);
    TEST_CHECK(and(1,0) == 0);
    TEST_CHECK(xor(1,0) == 1);
    TEST_CHECK(not(1) == 0);
    TEST_CHECK(or(1,1) == 1);
    TEST_CHECK(and(1,1) == 1);
    TEST_CHECK(xor(1,1) == 0);
    TEST_CHECK(not(and(or(1,0), 1)) == 0);
    TEST_CHECK(and(or(and(1,0),0), 1) == 0);
} else {printf("Ignored test ");}
}

const int ignore_check_calc_TT_line = 0;
void check_calc_TT_line(void) { if(!ignore_check_calc_TT_line) {
    int values2[2] = {0,1}, values3[3] = {0,1,1};
    TEST_CHECK(calc_TT_line(2, values2, "A+B") == 1);
    TEST_CHECK(calc_TT_line(2, values2, "A'+B''") == 1);
    TEST_CHECK(calc_TT_line(2, values2, "A'B") == 1);
    TEST_CHECK(calc_TT_line(2, values2, "AB") == 0);
    TEST_CHECK(calc_TT_line(2, values2, "(A)(B)") == 0);
    TEST_CHECK(calc_TT_line(2, values2, "A@B") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "AB+BC") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "A(B+C)") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "A+B+C") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "A+B.C") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "A.B.C") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "AB+C") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "A+B+BC") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "(A+B)(B+C)") == 1);
    TEST_CHECK(calc_TT_line(3, values3, "A(B+C)B") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "A+B+(B+C)") == 1);
    values3[0] = 1; values3[1] = 1; values3[2] = 1;
    TEST_CHECK(calc_TT_line(3, values3, "A'BC+AB'C+ABC'") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "(A'+B+C)(A+B'+C)'(A+B+C')") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "AB'C'+ABC'") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "(A+B'+C')'(A+B+C')'") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "AB'C'") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "(A+B'+C')") == 1);        
    TEST_CHECK(calc_TT_line(3, values3, "A'B'C") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "ABC'") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "(ABC')") == 0);
    TEST_CHECK(calc_TT_line(3, values3, "(ABC)'") == 0);
    
} else {printf("Ignored test ");}
}

const int ignore_check_convert_term_to_line = 0;
void check_convert_term_to_line(void) { if(!ignore_check_convert_term_to_line) {
    int* test1 = convert_term_to_line(4, "ABCD");
    int result1[] = {1,1,1,1};
    for(int i = 0; i < 4; i++)
        TEST_CHECK(test1[i] == result1[i]);

    int* test2 = convert_term_to_line(4, "A'B'C'D'");
    int result2[] = {0,0,0,0};
    for(int i = 0; i < 4; i++)
        TEST_CHECK(test2[i] == result2[i]);

    int* test3 = convert_term_to_line(4, "AB'CD'");
    int result3[] = {1,0,1,0};
    for(int i = 0; i < 4; i++)
        TEST_CHECK(test3[i] == result3[i]);

    int* test4 = convert_term_to_line(3, "AB'C'");
    int result4[] = {1,0,0};
    for(int i = 0; i < 3; i++)
        TEST_CHECK(test4[i] == result4[i]);

    int* test5 = convert_term_to_line(3, "ABC'");
    int result5[] = {1,1,0};
    for(int i = 0; i < 3; i++)
        TEST_CHECK(test5[i] == result5[i]);


} else {printf("Ignored test ");}
}

const int ignore_check_eval_exp = 0;
void check_eval_exp(void) { if(!ignore_check_eval_exp) {
    TEST_CHECK(eval_exp("(0'@(1+1))+1") == 1);              // NOT and XOR
    TEST_CHECK(eval_exp("(1+(0.1))'") == 0);                // OR and AND
    TEST_CHECK(eval_exp("((1+0')@(1.0)')") == 0);           // XOR and NOT
    TEST_CHECK(eval_exp("((1+1)@(0.0))") == 1);             // XOR and AND
    TEST_CHECK(eval_exp("((1'@(1+1))@(0.1))") == 1);        // XOR, NOT, and OR
    TEST_CHECK(eval_exp("((1'@(0.0))+1)") == 1);            // NOT, AND, and OR
    TEST_CHECK(eval_exp("(1+0')@(1+0)@(1.1)") == 1);        // OR, NOT, AND, and XOR
    TEST_CHECK(eval_exp("((0.1)@(1+1))+(1.0)") == 1);       // XOR, AND, and OR
    TEST_CHECK(eval_exp("(1.0)'@(0'@(1+1))") == 1);         // NOT, AND, and XOR
    TEST_CHECK(eval_exp("(0@(1+0'))+(1.1)") == 1);          // XOR, AND, and OR
    TEST_CHECK(eval_exp("(0'@(1+0))+(1'@(0.1))") == 0);     // NOT, AND, XOR, and OR
    TEST_CHECK(eval_exp("(1+(0@(1.0)))+(1'@(1+0))") == 1);  // OR, AND, XOR, and NOT
} else {printf("Ignored test ");}
}

const int ignore_check_required_size = 0;
void check_required_size(void) { if(!ignore_check_required_size) {
    int** mat1 = convert_BE_to_TT(2, "(A@B)'");
    TEST_CHECK(required_size(2, mat1, "SOP") == 7);
    TEST_CHECK(required_size(2, mat1, "POS") == 12);
    int** mat2 = convert_BE_to_TT(3, "B'");
    TEST_CHECK(required_size(3, mat2, "SOP") == 23);
    TEST_CHECK(required_size(3, mat2, "POS") == 32);
    free(mat1);
    free(mat2);
} else {printf("Ignored test ");}
}

const int ignore_check_generate_file_array = 1;
void check_generate_file_array(void) { if(!ignore_check_generate_file_array) {
    // Manually define an array containing all the file paths in the default repository
    int file_count = 2;
    char* array1[] = {  "./generated/boolean_equation_1.txt",
                        "./generated/boolean_equation_2.txt"}; 
    char* array2[file_count]; 
    generate_file_array(&array2);
    for(int i = 0; i < file_count; i++)
        TEST_CHECK(strcmp(array2[i], array1[i]) == 0);               
} else {printf("Ignored test ");}
}

const int ignore_check_generate_eq_array = 0;
void check_generate_eq_array(void) { if(!ignore_check_generate_eq_array) {

    // Manually define the resulting array containing the separated strings
    char* str1 = "=A+B=AB'C";
    char* result1[] = {  "A+B",
                        "AB'C"};
    char* array1[2];
    generate_eq_array(&array1, str1);
    TEST_CHECK(strcmp(array1[0], result1[0]) == 0);   
    TEST_CHECK(strcmp(array1[1], result1[1]) == 0);

    // Manually define the resulting array containing the separated strings
    char* str2 = "=A";
    char* result2[] = {"A"};
    char* array2[1];
    generate_eq_array(&array2, str2);
    TEST_CHECK(strcmp(array2[0], result2[0]) == 0);   

    // Manually define the resulting array containing the separated strings
    char* str3 = "=A+B+BC=A'BC+AB'C+ABC'=A.B.C=A@B=(A'+B+C)(A+B'+C)'(A+B+C')";
    char* result3[] = { "A+B+BC",
                        "A'BC+AB'C+ABC'",
                        "A.B.C",
                        "A@B",
                        "(A'+B+C)(A+B'+C)'(A+B+C')"};
    char* array3[5];
    generate_eq_array(&array3, str3);
    for (int i = 0; i < 5; i++)
        TEST_CHECK(strcmp(array3[i], result3[i]) == 0);   

} else {printf("Ignored test ");}
}

const int ignore_check_sub_str = 0;
void check_sub_str(void) { if(!ignore_check_sub_str) {
    TEST_CHECK(strcmp(sub_str(14, 18, "first sub_str test"), "test") == 0);
    TEST_CHECK(strcmp(sub_str(0, 5, "Hello World"), "Hello") == 0);
    TEST_CHECK(strcmp(sub_str(6, 11, "Hello World"), "World") == 0);
    TEST_CHECK(strcmp(sub_str(0, 4, "OpenAI"), "Open") == 0);
    TEST_CHECK(strcmp(sub_str(3, 6, "OpenAI"), "nAI") == 0);
    TEST_CHECK(strcmp(sub_str(1, 4, "Computer"), "omp") == 0);
} else {printf("Ignored test ");}
}

const int ignore_check_count_var_from_BE = 0;
void check_count_var_from_BE(void) { if(!ignore_check_count_var_from_BE) {
    TEST_CHECK(count_var_from_BE("A+B+BC") == 3);
    TEST_CHECK(count_var_from_BE("A'BC+AB'C+ABC'") == 3);
    TEST_CHECK(count_var_from_BE("A+C+B'") == 3);
    TEST_CHECK(count_var_from_BE("A@B") == 2);
    TEST_CHECK(count_var_from_BE("(A'+B+C)(A+B'+C)'(A+B+C')") == 3);
    TEST_CHECK(count_var_from_BE("A") == 1);
} else {printf("Ignored test ");}
}

const int ignore_check_compare_eq = 0;
void check_compare_eq(void) { if(!ignore_check_compare_eq) {
    initialize_from_BE(2, "A+B");
    Equation eq1 = current_eq;
    initialize_from_BE(2, "B+A");
    Equation eq2 = current_eq;
    TEST_CHECK(compare_eq(&eq1, &eq2) == 1);

    initialize_from_BE(4, "A'B'C'D+A'B'CD+A'BC'D+A'BCD+ABC'D+ABCD+AB'C'D+AB'CD+A'BC'D'+A'BCD'+ABC'D'+ABCD'");
    eq1 = current_eq;
    initialize_from_BE(4, "B+D");
    eq2 = current_eq;
    TEST_CHECK(compare_eq(&eq1, &eq2) == 1);
} else {printf("Ignored test ");}
}

const int ignore_check_simplified_eq = 0;
void check_simplified_eq(void) { if(!ignore_check_simplified_eq) {
    /* Line groups */
    initialize_from_BE(2, "AB'+AB");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A") == 0);
    initialize_from_BE(2, "AB'+A'B+AB");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "B+A") == 0);
    initialize_from_BE(3, "AB'C'+AB'C+ABC+ABC'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A") == 0);
    initialize_from_BE(3, "A'BC'+AB'C'+AB'C+ABC'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "BC'+AB'") == 0);
    initialize_from_BE(3, "A'B'C+ABC'+ABC");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'B'C+AB") == 0);
    initialize_from_BE(4, "A'B'C'D+A'BC'D+AB'CD+AB'CD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'C'D+AB'C") == 0);
    initialize_from_BE(4, "A'B'C'D+A'BC'D+ABC'D+AB'C'D");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "C'D") == 0);
    initialize_from_BE(4, "A'BC'D'+A'BC'D+A'BCD+A'BCD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'B") == 0);
    initialize_from_BE(4, "A'B'CD+A'BC'D'+A'BC'D+A'BCD'+A'BCD+AB'C'D'+AB'CD+ABC'D'+ABCD");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "CD+A'B+AC'D'") == 0);
    /* Square groups */
    initialize_from_BE(3, "A'BC+A'BC'+ABC+ABC'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "B") == 0);
    initialize_from_BE(4, "A'BCD+A'BC'D+ABCD+ABC'D");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "BD") == 0);
    initialize_from_BE(4, "A'B'C'D'+A'B'C'D+A'BC'D'+A'BC'D+ABCD+ABCD'+AB'CD+AB'CD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'C'+AC") == 0);
    initialize_from_BE(4, "A'B'C'D'+A'B'C'D+A'B'CD'+A'BC'D'+A'BC'D+ABCD+ABCD'+AB'CD+AB'CD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'C'+A'B'D'+AC") == 0);
    initialize_from_BE(4, "A'B'C'D'+A'B'CD'+A'BC'D'+A'BCD+AB'C'D'+AB'CD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "B'D'+A'C'D'+A'BCD") == 0);
    initialize_from_BE(4, "A'B'C'D'+A'B'CD+A'BC'D+AB'CD'+AB'CD+ABCD'+ABCD");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'B'C'D'+B'CD+A'BC'D+AC") == 0);
    /* Rectangle groups */
    initialize_from_BE(4, "A'B'C'D+A'B'CD+A'BC'D+A'BCD+ABC'D+ABCD+AB'C'D+AB'CD");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "D") == 0);
    initialize_from_BE(4, "A'B'C'D'+A'B'C'D+A'B'CD+A'B'CD'+A'BC'D'+A'BC'D+A'BCD+A'BCD'+ABC'D'+ABC'D+ABCD+ABCD'+AB'C'D+AB'CD+AB'CD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "A'+B+D+C") == 0);
    initialize_from_BE(4, "A'B'C'D+A'B'CD+A'BC'D+A'BCD+ABC'D+ABCD+AB'C'D+AB'CD+A'BC'D'+A'BCD'+ABC'D'+ABCD'");
    TEST_CHECK(strcmp(simplified_eq(&current_eq).bool_exp, "D+B") == 0);
} else {printf("Ignored test ");}
}

TEST_LIST = {
    {"format_BE", check_format_BE},
    {"delete_char", check_delete_char},
    {"replace_char", check_replace_char},
    {"is_string_valid", check_is_string_valid},
    {"boolean_operations", check_boolean_operations},
    {"calc_TT_line", check_calc_TT_line},
    {"convert_term_to_line", check_convert_term_to_line},
    {"eval_exp", check_eval_exp},
    {"required_size", check_required_size},
    {"generate_file_array", check_generate_file_array},
    {"generate_eq_array", check_generate_eq_array},
    {"sub_str", check_sub_str},
    {"count_var_from_BE", check_count_var_from_BE},
    {"simplified_eq", check_simplified_eq},
    {"compare_eq", check_compare_eq},
    {0}
};
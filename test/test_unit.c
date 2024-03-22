#include "../lib/acutest.h"
#include<stdio.h>
#include "../test/test_unit.h"
#include "../src/public.h"
#include "../src/bool.h"
#include "../src/karnaugh.h"
#include "../src/calc.h"

void check_format_BE(void) 
{
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
}

void check_delete_char(void) 
{
    TEST_CHECK(strcmp(delete_char("A+B", '+'), "AB") == 0);
}

void check_replace_char(void)
{
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
}

void check_is_string_valid(void) 
{
    TEST_CHECK(is_string_valid("A", "ABC") == 1);
    TEST_CHECK(is_string_valid("B", "ABC") == 1);
    TEST_CHECK(is_string_valid("C", "ABC") == 1);
    TEST_CHECK(is_string_valid("A+B+C", "A") == 0);
}

void check_boolean_operations(void) 
{
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
}

void check_calc_TT_line(void)
{
    int values2[2] = {0,1}, values3[3] = {0,1,1};
    TEST_CHECK(calc_TT_line(2, values2, "A+B") == 1);
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
}

void check_eval_exp(void) 
{
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
}

TEST_LIST = {
    {"check_format_BE", check_format_BE},
    {"check_delete_char", check_delete_char},
    {"check_replace_char", check_replace_char},
    {"check_is_string_valid", check_is_string_valid},
    {"check_boolean_operations", check_boolean_operations},
    {"check_calc_TT_line", check_calc_TT_line},
    {"check_eval_exp", check_eval_exp},
    {0}
};
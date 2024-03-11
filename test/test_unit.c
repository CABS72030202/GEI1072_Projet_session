#include "../lib/acutest.h"
#include<stdio.h>
#include "../test/test_unit.h"
#include "../src/public.h"
#include "../src/bool.h"
#include "../src/karnaugh.h"

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

    // Invalid first or last character
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
    TEST_CHECK(inv(0) == 1);
    TEST_CHECK(or(0,1) == 1);
    TEST_CHECK(and(0,1) == 0);
    TEST_CHECK(xor(0,1) == 1);
    TEST_CHECK(or(1,0) == 1);
    TEST_CHECK(and(1,0) == 0);
    TEST_CHECK(xor(1,0) == 1);
    TEST_CHECK(inv(1) == 0);
    TEST_CHECK(or(1,1) == 1);
    TEST_CHECK(and(1,1) == 1);
    TEST_CHECK(xor(1,1) == 0);
}

TEST_LIST = {
    {"check_format_BE", check_format_BE},
    {"check_delete_char", check_delete_char},
    {"check_is_string_valid", check_is_string_valid},
    {"check_boolean_operations", check_boolean_operations},
    {0}
};
#include "../lib/acutest.h"
#include<stdio.h>
#include "../test/test_unit.h"
#include "../src/public.h"
#include "../src/bool.h"
#include "../src/karnaugh.h"

void check_format_BE(void) 
{
    char valid_chars[] = "+'.()@ ABCDS";
    //TEST_CHECK(format_BE("-", valid_chars) == NULL);
    TEST_CHECK(strcmp(format_BE("A", valid_chars), "A") == 0);
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
    TEST_CHECK(is_string_valid("ABC", "A") == 0);
}

TEST_LIST = {
    {"check_format_BE", check_format_BE},
    {"check_delete_char", check_delete_char},
    {"check_is_string_valid", check_is_string_valid},
    {0}
};
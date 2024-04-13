#ifndef TEST_H
#define TEST_H
#include "../lib/acutest.h"
#include <stdio.h>
#include "../src/user.h"

#include "../src/public.h"
extern const int ignore_check_delete_char;
void check_delete_char(void);

extern const int ignore_check_replace_char;
void check_replace_char(void);

extern const int ignore_check_is_string_valid;
void check_is_string_valid(void);

extern const int ignore_check_sub_str;
void check_sub_str(void);


#include "../src/bool.h"
extern const int ignore_check_format_BE;
void check_format_BE(void);

extern const int ignore_check_required_size;
void check_required_size(void);

extern const int ignore_check_calc_TT_line;
void check_calc_TT_line(void);

extern const int ignore_check_convert_term_to_line;
void check_convert_term_to_line(void);

extern const int ignore_check_count_var_from_BE;
void check_count_var_from_BE(void);


#include "../src/karnaugh.h"
extern const int ignore_check_simplify_eq;
void check_simplify_eq(void);


#include "../src/calc.h"
extern const int ignore_check_eval_exp;
void check_eval_exp(void);

extern const int ignore_check_boolean_operations;
void check_boolean_operations(void);


#include "../src/file.h"
extern const int ignore_check_generate_file_array;
void check_generate_file_array(void);

extern const int ignore_check_generate_eq_array;
void check_generate_eq_array(void);


#endif
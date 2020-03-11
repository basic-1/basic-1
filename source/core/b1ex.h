/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1ex.h: declarations of functions that are external to the interpreter core
 (memory, variables cache, program lines navigation, input/output, etc.)
*/


#ifndef _B1_EX_
#define _B1_EX_

#include "b1var.h"


extern B1_T_ERROR b1_ex_mem_init();
extern B1_T_ERROR b1_ex_mem_alloc(B1_T_MEMOFFSET size, B1_T_MEM_BLOCK_DESC *mem_desc, void **data);
extern B1_T_ERROR b1_ex_mem_access(const B1_T_MEM_BLOCK_DESC mem_desc, void **data);
extern B1_T_ERROR b1_ex_mem_release(const B1_T_MEM_BLOCK_DESC mem_desc);
extern B1_T_ERROR b1_ex_mem_free(const B1_T_MEM_BLOCK_DESC mem_desc);

extern B1_T_ERROR b1_ex_var_init();
extern B1_T_ERROR b1_ex_var_alloc(B1_T_IDHASH name_hash, uint8_t new_var_flags, B1_NAMED_VAR **var);
extern B1_T_ERROR b1_ex_var_free(B1_NAMED_VAR *var);

// prints one character
extern B1_T_ERROR b1_ex_io_print_char(B1_T_CHAR c);
// prints new line sequence
extern B1_T_ERROR b1_ex_io_print_newline();
// prints display margin new line sequence
extern B1_T_ERROR b1_ex_io_print_margin_newline();
// copies next input value into the specified buffer
extern B1_T_ERROR b1_ex_io_input_char(B1_T_CHAR *c);

// caches line numbers (allowing faster program navigation), the cached data can be used by b1_ex_prg_get_prog_line ant other functions
extern B1_T_ERROR b1_ex_prg_cache_curr_line_num(B1_T_LINE_NUM curr_line_num, uint8_t stmt);
// the function should set b1_int_progline and b1_int_curr_prog_line_cnt global variables according to the program line
// number requested via next_line_num argument. the argument can be either BASIC line number or one of the next constants:
// B1_T_LINE_NUM_FIRST, B1_T_LINE_NUM_NEXT. can return the next values (error codes): B1_RES_OK, B1_RES_ELINENNOTFND,
// B1_RES_EPROGUNEND, B1_RES_EENVFAT
extern B1_T_ERROR b1_ex_prg_get_prog_line(B1_T_LINE_NUM next_line_num);
// sets the NEXT statement line counter for the current FOR statement
extern B1_T_ERROR b1_ex_prg_for_go_next();
#ifdef B1_FEATURE_STMT_DATA_READ
// sets the next DATA stamtement line counter (b1_int_data_curr_line_cnt and b1_int_data_curr_line_offset),
// next_line_num can be either valid line number or B1_T_LINE_NUM_FIRST, B1_T_LINE_NUM_NEXT constants.
// possible return codes: B1_RES_OK, B1_RES_EDATAEND, B1_RES_ELINENNOTFND, B1_RES_EENVFAT, etc.
extern B1_T_ERROR b1_ex_prg_data_go_next(B1_T_LINE_NUM next_line_num);
#endif
#ifdef B1_FEATURE_RPN_CACHING
extern B1_T_ERROR b1_ex_prg_rpn_cache(B1_T_INDEX offset, B1_T_INDEX continue_offset);
extern B1_T_ERROR b1_ex_prg_rpn_get_cached(B1_T_INDEX offset, B1_T_INDEX *continue_offset);
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
#ifdef B1_FRACTIONAL_TYPE_EXISTS
extern void b1_ex_rnd_randomize();
extern float b1_ex_rnd_rand();
#endif
#endif

#endif

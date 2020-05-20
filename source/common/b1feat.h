/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 feat.h: interpreter features
*/


#ifndef _B1_FEATURES_
#define _B1_FEATURES_


// ERASE statement
#define B1_FEATURE_STMT_ERASE

// DATA, READ and RESTORE statements
#define B1_FEATURE_STMT_DATA_READ

// LEN, ASC, CHR$, STR$, VAL, IIF, IIF$ functions
#define B1_FEATURE_FUNCTIONS_STANDARD

// ABS, INT, RND, SGN functions and RANDOMIZE statement
#define B1_FEATURE_FUNCTIONS_MATH_BASIC

// ATN, COS, EXP, LOG, PI, SIN, SQR, TAN functions
#define B1_FEATURE_FUNCTIONS_MATH_EXTRA

// MID$, INSTR, LTRIM$, RTRIM$, LEFT$, RIGHT$, LSET$, RSET$, UCASE$, LCASE$ functions
#define B1_FEATURE_FUNCTIONS_STRING

// DEF statement and user defined functions
#define B1_FEATURE_FUNCTIONS_USER

// enable SINGLE type
#define B1_FEATURE_TYPE_SINGLE

// enable RPN caching
#define B1_FEATURE_RPN_CACHING

// locales support (for LCASE$, UCASE$, INSTR functions and string comparison operators)
#define B1_FEATURE_LOCALES

// type size for array subscripts (if no one macro is enabled 16 bit type is used)
//#define B1_FEATURE_SUBSCRIPT_8BIT
//#define B1_FEATURE_SUBSCRIPT_12BIT
//#define B1_FEATURE_SUBSCRIPT_16BIT
//#define B1_FEATURE_SUBSCRIPT_24BIT

// type size for pointer offsets and memory block sizes (if no one macro is define 32 bit type is used)
//#define B1_FEATURE_MEMOFFSET_16BIT
//#define B1_FEATURE_MEMOFFSET_32BIT

// constraints and memory usage
#define B1_MAX_PROGLINE_LEN 255
#define B1_MAX_STRING_LEN (B1_MAX_PROGLINE_LEN / 2)
#define B1_MAX_LINE_NUM_LEN 5
#define B1_MAX_FN_ARGS_NUM 3
#define B1_MAX_IDENTIFIER_LEN 31
#define B1_MAX_VAR_DIM_NUM 2
#define B1_DEF_SUBSCRIPT_UBOUND 10
#define B1_MAX_RPN_LEN 64
#define B1_MAX_RPN_EVAL_BUFFER_LEN 9
#define B1_MAX_RPN_BRACK_NEST_DEPTH 7

#ifdef B1_FEATURE_FUNCTIONS_USER
#define B1_MAX_UDEF_FN_RPN_LEN (768 / sizeof(B1_RPNREC))
#define B1_MAX_UDEF_FN_NUM ((uint8_t)10)
#define B1_MAX_UDEF_CALL_NEST_DEPTH 3
#endif

#endif

/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1types.h: variable data type constants, string and character macros
*/


#ifndef _B1_TYPES_
#define _B1_TYPES_

#include <ctype.h>

#include "b1feat.h"
#include "b1itypes.h"

#ifdef B1_FEATURE_UNICODE_UCS2
#include <wctype.h>
#endif


#define B1_TMP_BUF_LEN ((B1_T_INDEX)(((B1_MAX_STRING_LEN) > (B1_MAX_IDENTIFIER_LEN) ? (B1_MAX_STRING_LEN) : (B1_MAX_IDENTIFIER_LEN)) + 1))

// user data types
#ifdef B1_FEATURE_TYPE_SINGLE
// 4-byte floating-point value
#define B1_TYPE_SINGLE ((uint8_t)0x0)
#endif
// 4-byte integer value
#define B1_TYPE_INT32 ((uint8_t)0x1)
// string value
#define B1_TYPE_STRING ((uint8_t)0x2)

// internally used boolean value
#define B1_TYPE_BOOL ((uint8_t)0x3)

// special TAB value (PRINT statement TAB function)
#define B1_TYPE_TAB_FN ((uint8_t)0x1B)
// special SPC value (PRINT statement SPC function)
#define B1_TYPE_SPC_FN ((uint8_t)0x1C)
// NULL type (missing function argument or invalid type in some cases)
#define B1_TYPE_NULL ((uint8_t)0x1D)
// special type: pointer to RPN record
#define B1_TYPE_RPNREC_PTR ((uint8_t)0x1E)
// special type: any type
#define B1_TYPE_ANY ((uint8_t)0x1F)

#define B1_TYPE_MASK ((uint8_t)0x1F)
#define B1_TYPE_SHIFT ((uint8_t)0x0)

// immediate string
#define B1_TYPE_STRING_IMM_FLAG ((uint8_t)0x20)

// variable reference (no need to free)
#define B1_TYPE_REF_FLAG ((uint8_t)0x80)

#define B1_T_C_STRTERM ((B1_T_CHAR)0)
#define B1_T_C_0 ((B1_T_CHAR)'0')
#define B1_T_C_1 ((B1_T_CHAR)'1')
#define B1_T_C_2 ((B1_T_CHAR)'2')
#define B1_T_C_3 ((B1_T_CHAR)'3')
#define B1_T_C_4 ((B1_T_CHAR)'4')
#define B1_T_C_5 ((B1_T_CHAR)'5')
#define B1_T_C_6 ((B1_T_CHAR)'6')
#define B1_T_C_7 ((B1_T_CHAR)'7')
#define B1_T_C_8 ((B1_T_CHAR)'8')
#define B1_T_C_9 ((B1_T_CHAR)'9')
#define B1_T_C_PLUS ((B1_T_CHAR)'+')
#define B1_T_C_MINUS ((B1_T_CHAR)'-')
#define B1_T_C_SPACE ((B1_T_CHAR)' ')
#define B1_T_C_TAB ((B1_T_CHAR)'\t')
#define B1_T_C_DBLQUOTE ((B1_T_CHAR)'\"')
#define B1_T_C_COMMA ((B1_T_CHAR)',')
#define B1_T_C_COLON ((B1_T_CHAR)':')
#define B1_T_C_SEMICOLON ((B1_T_CHAR)';')
#define B1_T_C_CR ((B1_T_CHAR)'\r')
#define B1_T_C_LF ((B1_T_CHAR)'\n')
#define B1_T_C_APOSTROPHE ((B1_T_CHAR)'\'')
#define B1_T_C_EXCLAMATION ((B1_T_CHAR)'!')
#define B1_T_C_PERCENT ((B1_T_CHAR)'%')
#define B1_T_C_ASTERISK ((B1_T_CHAR)'*')
#define B1_T_C_SLASH ((B1_T_CHAR)'/')
#define B1_T_C_CARET ((B1_T_CHAR)'^')
#define B1_T_C_UCA ((B1_T_CHAR)'A')
#define B1_T_C_UCE ((B1_T_CHAR)'E')
#define B1_T_C_UCZ ((B1_T_CHAR)'Z')
#define B1_T_C_LCA ((B1_T_CHAR)'a')
#define B1_T_C_LCE ((B1_T_CHAR)'e')
#define B1_T_C_LCZ ((B1_T_CHAR)'z')
#define B1_T_C_OPBRACK ((B1_T_CHAR)'(')
#define B1_T_C_CLBRACK ((B1_T_CHAR)')')
#define B1_T_C_EQ ((B1_T_CHAR)'=')
#define B1_T_C_LT ((B1_T_CHAR)'<')
#define B1_T_C_GT ((B1_T_CHAR)'>')
#define B1_T_C_QUESTION ((B1_T_CHAR)'?')
#define B1_T_C_POINT ((B1_T_CHAR)'.')
#define B1_T_C_DOLLAR ((B1_T_CHAR)'$')

#define B1_T_ISCSTRTERM(C) ((B1_T_CHAR)(C) == B1_T_C_STRTERM)
#define B1_T_ISDIGIT(C) (((B1_T_CHAR)(C) >= B1_T_C_0) && ((B1_T_CHAR)(C) <= B1_T_C_9))
#define B1_T_CHAR2NUM(C) ((B1_T_CHAR)(C) - B1_T_C_0)
#define B1_T_NUM2CHAR(N) ((B1_T_CHAR)(N) + B1_T_C_0)
#define B1_T_ISPLUS(C) ((B1_T_CHAR)(C) == B1_T_C_PLUS)
#define B1_T_ISMINUS(C) ((B1_T_CHAR)(C) == B1_T_C_MINUS)
#define B1_T_ISBLANK(C) (((B1_T_CHAR)(C) == B1_T_C_SPACE) || ((B1_T_CHAR)(C) == B1_T_C_TAB))
#define B1_T_ISDBLQUOTE(C) ((B1_T_CHAR)(C) == B1_T_C_DBLQUOTE)
#define B1_T_ISCOMMA(C) ((B1_T_CHAR)(C) == B1_T_C_COMMA)
#define B1_T_ISSEMICOLON(C) ((B1_T_CHAR)(C) == B1_T_C_SEMICOLON)
#define B1_T_ISCR(C) ((B1_T_CHAR)(C) == B1_T_C_CR)
#define B1_T_ISLF(C) ((B1_T_CHAR)(C) == B1_T_C_LF)
#define B1_T_ISCRLF(C) (B1_T_ISCR(C) || B1_T_ISLF(C))
#define B1_T_ISEXPRCHAR(C) (B1_T_ISPLUS(C) || B1_T_ISMINUS(C) \
	|| ((B1_T_CHAR)(C) == B1_T_C_ASTERISK) || ((B1_T_CHAR)(C) == B1_T_C_SLASH) || ((B1_T_CHAR)(C) == B1_T_C_CARET) \
	|| ((B1_T_CHAR)(C) == B1_T_C_OPBRACK) || ((B1_T_CHAR)(C) == B1_T_C_CLBRACK) || ((B1_T_CHAR)(C) == B1_T_C_EQ) \
	|| ((B1_T_CHAR)(C) == B1_T_C_LT) || ((B1_T_CHAR)(C) == B1_T_C_GT) || B1_T_ISCOMMA(C)  || B1_T_ISSEMICOLON(C))

// the macros below should support English letters only (they are used for processing program keywords)
#define B1_T_ISUPPER(C) (((B1_T_CHAR)(C) >= B1_T_C_UCA) && ((B1_T_CHAR)(C) <= B1_T_C_UCZ))
#define B1_T_ISLOWER(C) (((B1_T_CHAR)(C) >= B1_T_C_LCA) && ((B1_T_CHAR)(C) <= B1_T_C_LCZ))
#define B1_T_ISALPHA(C) (B1_T_ISUPPER(C) || B1_T_ISLOWER(C))
#define B1_T_ALPHA2NUM(C) (B1_T_TOUPPER(C) - B1_T_C_UCA)
#define B1_T_TOUPPER(C) (B1_T_ISLOWER(C) ? ((B1_T_CHAR)(((B1_T_CHAR)(C)) - B1_T_C_LCA + B1_T_C_UCA)) : (B1_T_CHAR)(C))
#define B1_T_TOLOWER(C) (B1_T_ISUPPER(C) ? ((B1_T_CHAR)(((B1_T_CHAR)(C)) + B1_T_C_LCA - B1_T_C_UCA)) : (B1_T_CHAR)(C))

#define B1_TYPE_GET(TYPE) (((((uint8_t)(TYPE))) & B1_TYPE_MASK) >> B1_TYPE_SHIFT)
#define B1_TYPE_TEST_ANY(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_ANY)
#define B1_TYPE_TEST_RPNREC_PTR(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_RPNREC_PTR)
#define B1_TYPE_TEST_TAB_FN(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_TAB_FN)
#define B1_TYPE_TEST_SPC_FN(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_SPC_FN)
#define B1_TYPE_TEST_STRING(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_STRING)
#define B1_TYPE_TEST_INT32(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_INT32)
#ifdef B1_FEATURE_TYPE_SINGLE
#define B1_TYPE_TEST_SINGLE(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_SINGLE)
#define B1_TYPE_TEST_NUMERIC(TYPE) (B1_TYPE_TEST_SINGLE(TYPE) || B1_TYPE_TEST_INT32(TYPE))
#else
#define B1_TYPE_TEST_NUMERIC(TYPE) (B1_TYPE_TEST_INT32(TYPE))
#endif
#define B1_TYPE_TEST_BOOL(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_BOOL)
#define B1_TYPE_TEST_NULL(TYPE) (B1_TYPE_GET(TYPE) == B1_TYPE_NULL)

#define B1_TYPE_TEST_STRING_IMM(TYPE) (((uint8_t)(TYPE)) & B1_TYPE_STRING_IMM_FLAG)

#define B1_TYPE_TEST_REF(TYPE) (((uint8_t)(TYPE)) & B1_TYPE_REF_FLAG)

#define B1_TYPE_SET(TYPE, FLAGS) ((((uint8_t)(B1_TYPE_GET(TYPE))) << B1_TYPE_SHIFT) | ((uint8_t)(FLAGS)))

// max. immediate string length
#define B1_TYPE_STRING_IMM_MAX_LEN ((uint8_t)((4 / sizeof(B1_T_CHAR)) - 1))

#ifdef B1_FEATURE_TYPE_SINGLE
// in case of a fractional type absence INT and RND functions and RANDOMIZE statements are not available
#define B1_FRACTIONAL_TYPE_EXISTS
#endif


#ifdef B1_FEATURE_LOCALES
#define B1_T_TOUPPER_L(C) b1_t_toupper_l(C)
#define B1_T_TOLOWER_L(C) b1_t_tolower_l(C)
#define B1_T_STRCMP_L(BSTR1, STR2DATA, STR2LEN) b1_t_strcmp_l(BSTR1, STR2DATA, STR2LEN)
#else
#define B1_T_TOUPPER_L(C) B1_T_TOUPPER(C)
#define B1_T_TOLOWER_L(C) B1_T_TOLOWER(C)
#define B1_T_STRCMP_L(BSTR1, STR2DATA, STR2LEN) b1_t_strcmpi(BSTR1, STR2DATA, STR2LEN)
#endif


extern const B1_T_CHAR _EQ[];
extern const B1_T_CHAR _THEN[];
extern const B1_T_CHAR _GOTO[];
extern const B1_T_CHAR _GOSUB[];
extern const B1_T_CHAR _TO[];
extern const B1_T_CHAR _STEP[];
extern const B1_T_CHAR _COMMA[];
extern const B1_T_CHAR _SEMICOLON[];
extern const B1_T_CHAR _CLBRACKET[];
extern const B1_T_CHAR _PROMPT[];
extern const B1_T_CHAR _AS[];
extern const B1_T_CHAR _ON[];
extern const B1_T_CHAR _OFF[];
extern const B1_T_CHAR _BASE[];
extern const B1_T_CHAR _EXPLICIT[];
extern const B1_T_CHAR _MARGIN[];
extern const B1_T_CHAR _ZONEWIDTH[];
extern const B1_T_CHAR _INPUTECHO[];
#ifdef B1_FEATURE_TYPE_SINGLE
extern const B1_T_CHAR _SINGLE[];
#endif
extern const B1_T_CHAR _STRING[];
extern const B1_T_CHAR _INT[];

#ifdef B1_FEATURE_DEBUG
extern const B1_T_CHAR _DBG_FORVAR[];
extern const B1_T_CHAR _DBG_TYPE_OPBR[];
extern const B1_T_CHAR _DBG_TYPE_CLBR[];
extern const B1_T_CHAR _DBG_INVALID[];
extern const B1_T_CHAR _DBG_UNALLOC[];
extern const B1_T_CHAR _DBG_DELIM[];
#endif


extern B1_T_ERROR b1_t_strtoui16(const B1_T_CHAR *cs, uint16_t *value);
extern B1_T_ERROR b1_t_strtoi32(const B1_T_CHAR *cs, int32_t *value);
extern B1_T_ERROR b1_t_i32tostr(int32_t value, B1_T_CHAR *sbuf, B1_T_INDEX buflen);
#ifdef B1_FEATURE_TYPE_SINGLE
extern B1_T_ERROR b1_t_strtosingle(const B1_T_CHAR *cs, float *value);
extern B1_T_ERROR b1_t_singletostr(float value, B1_T_CHAR *sbuf, B1_T_INDEX buflen, uint8_t max_len);
#endif
extern int8_t b1_t_strcmpi(const B1_T_CHAR *s1, const B1_T_CHAR *s2data, B1_T_INDEX s2len);


// checks
#if defined(B1_FEATURE_DEBUG) && !defined(B1_FEATURE_INIT_FREE_MEMORY)
#error B1_FEATURE_DEBUG feature requires B1_FEATURE_INIT_FREE_MEMORY to be enabled
#endif

#if defined(B1_FEATURE_TYPE_SINGLE) && !defined(B1_FRACTIONAL_TYPE_EXISTS)
#error floating point type(-s) support enabled without B1_FRACTIONAL_TYPE_EXISTS definition
#endif

#if !defined(B1_FEATURE_TYPE_SINGLE) && defined(B1_FRACTIONAL_TYPE_EXISTS)
#error no one fractional type is supported, check B1_FRACTIONAL_TYPE_EXISTS definition
#endif

#if defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) && !defined(B1_FEATURE_TYPE_SINGLE)
#error math functions are not allowed without floating point type(-s) support
#endif

#if B1_MAX_STRING_LEN > B1_MAX_PROGLINE_LEN / 2
#error B1_MAX_STRING_LEN must not be greater than B1_MAX_PROGLINE_LEN / 2
#endif

#if (B1_MAX_STRING_LEN < 18) || (B1_MAX_STRING_LEN > B1_T_INDEX_MAX_VALUE)
#error B1_MAX_STRING_LEN value must be not less than 18 and not greater than B1_T_INDEX_MAX_VALUE
#endif

#if B1_MAX_STRING_LEN > B1_T_CHAR_MAX_VALUE
#error B1_MAX_STRING_LEN must not be greater than B1_T_CHAR_MAX_VALUE
#endif

#if B1_MAX_FN_ARGS_NUM < 3
#error B1_MAX_FN_ARGS_NUM must be not less than 3
#endif

#if B1_MAX_FN_ARGS_NUM > 7
#error B1_MAX_FN_ARGS_NUM must not be greater than 7
#endif

#if ((B1_T_MEMOFFSET_MIN_VALUE >> 4) > B1_T_SUBSCRIPT_MIN_VALUE) || ((B1_T_MEMOFFSET_MAX_VALUE >> 4) < B1_T_SUBSCRIPT_MAX_VALUE)
#error inappropriate B1_T_SUBSCRIPT or B1_T_MEMOFFSET type: B1_T_MEMOFFSET type should allow addressing any array element
#endif

#endif

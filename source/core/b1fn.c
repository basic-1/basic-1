/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1fn.c: built-in and user defined functions
*/


#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "b1ex.h"
#include "b1tok.h"
#include "b1fn.h"
#include "b1eval.h"
#include "b1int.h"
#include "b1err.h"


// global data
// user defined functions RPN store
#ifdef B1_FEATURE_FUNCTIONS_USER
B1_T_INDEX b1_fn_udef_fn_rpn_off = 0;
B1_RPNREC b1_fn_udef_fn_rpn[B1_MAX_UDEF_FN_RPN_LEN];
B1_UDEF_FN b1_fn_udef_fns[B1_MAX_UDEF_FN_NUM];
// user defined function call stack
B1_UDEF_CALL b1_fn_udef_call_stack[B1_MAX_UDEF_CALL_NEST_DEPTH];
#endif


#ifdef B1_FEATURE_FUNCTIONS_STANDARD
static B1_T_ERROR b1_fn_bltin_iif(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_striif(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_len(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_asc(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_chr(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_str(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_val(B1_VAR *parg1);
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
static B1_T_ERROR b1_fn_bltin_abs(B1_VAR *parg1);
#ifdef B1_FRACTIONAL_TYPE_EXISTS
static B1_T_ERROR b1_fn_bltin_int(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_rnd(B1_VAR *parg1);
#endif
static B1_T_ERROR b1_fn_bltin_sgn(B1_VAR *parg1);
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_EXTRA
static B1_T_ERROR b1_fn_bltin_atn(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_cos(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_exp(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_log(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_pi(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_sin(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_sqr(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_tan(B1_VAR *parg1);
#endif

#ifdef B1_FEATURE_FUNCTIONS_STRING
static B1_T_ERROR b1_fn_bltin_mid(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_instr(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_ltrim(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_rtrim(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_left(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_right(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_lset(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_rset(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_ucase(B1_VAR *parg1);
static B1_T_ERROR b1_fn_bltin_lcase(B1_VAR *parg1);
#endif


#if defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)
static const B1_BLTIN_FN b1_fn_bltin[B1_FN_BLTIN_COUNT] =
{
#ifdef B1_FEATURE_FUNCTIONS_STANDARD
	{{{0x621D, B1_IDENT_FLAGS_SET_FN(3, 1)}, {B1_TYPE_BOOL, B1_TYPE_ANY, B1_TYPE_ANY}, B1_TYPE_ANY}, b1_fn_bltin_iif},
	{{{0x4866, B1_IDENT_FLAGS_SET_FN(3, 1)}, {B1_TYPE_BOOL, B1_TYPE_STRING, B1_TYPE_STRING}, B1_TYPE_STRING}, b1_fn_bltin_striif},
	{{{0xDFC8, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_INT32}, b1_fn_bltin_len},
	{{{0x6D38, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_INT32}, b1_fn_bltin_asc},
	{{{0xFAE3, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_ANY}, B1_TYPE_STRING}, b1_fn_bltin_chr},
	{{{0x4BFE, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_ANY}, B1_TYPE_STRING}, b1_fn_bltin_str},
	{{{0xF7C3, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_ANY}, b1_fn_bltin_val},
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
	{{{0x290A, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_ANY}, B1_TYPE_ANY}, b1_fn_bltin_abs},
#ifdef B1_FRACTIONAL_TYPE_EXISTS
	{{{0x77AF, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_int},
	{{{0x1924, B1_IDENT_FLAGS_SET_FN(0, 1)}, {0}, B1_TYPE_SINGLE}, b1_fn_bltin_rnd},
#endif
	{{{0x834A, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_ANY}, B1_TYPE_ANY}, b1_fn_bltin_sgn},
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_EXTRA
	{{{0x71B5, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_atn},
	{{{0x6370, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_cos},
	{{{0x917C, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_exp},
	{{{0x05AF, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_log},
	{{{0xD787, B1_IDENT_FLAGS_SET_FN(0, 1)}, {0}, B1_TYPE_SINGLE}, b1_fn_bltin_pi},
	{{{0x8B0A, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_sin},
	{{{0xAE97, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_sqr},
	{{{0xF009, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_SINGLE}, B1_TYPE_SINGLE}, b1_fn_bltin_tan},
#endif

#ifdef B1_FEATURE_FUNCTIONS_STRING
	{{{0x46B6, B1_IDENT_FLAGS_SET_FN(3, 1)}, {B1_TYPE_STRING, B1_TYPE_INT32, B1_TYPE_ANY}, B1_TYPE_STRING}, b1_fn_bltin_mid},
	{{{0xFF2D, B1_IDENT_FLAGS_SET_FN(3, 1)}, {B1_TYPE_ANY, B1_TYPE_STRING, B1_TYPE_STRING}, B1_TYPE_INT32}, b1_fn_bltin_instr},
	{{{0x2C0D, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_STRING}, b1_fn_bltin_ltrim},
	{{{0x4C4E, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_STRING}, b1_fn_bltin_rtrim},
	{{{0xEAA3, B1_IDENT_FLAGS_SET_FN(2, 1)}, {B1_TYPE_STRING, B1_TYPE_INT32}, B1_TYPE_STRING}, b1_fn_bltin_left},
	{{{0x21BD, B1_IDENT_FLAGS_SET_FN(2, 1)}, {B1_TYPE_STRING, B1_TYPE_INT32}, B1_TYPE_STRING}, b1_fn_bltin_right},
	{{{0x9628, B1_IDENT_FLAGS_SET_FN(2, 1)}, {B1_TYPE_STRING, B1_TYPE_INT32}, B1_TYPE_STRING}, b1_fn_bltin_lset},
	{{{0x6D27, B1_IDENT_FLAGS_SET_FN(2, 1)}, {B1_TYPE_STRING, B1_TYPE_INT32}, B1_TYPE_STRING}, b1_fn_bltin_rset},
	{{{0x4346, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_STRING}, b1_fn_bltin_ucase},
	{{{0x444E, B1_IDENT_FLAGS_SET_FN(1, 1)}, {B1_TYPE_STRING}, B1_TYPE_STRING}, b1_fn_bltin_lcase},
#endif
};
#endif


#ifdef B1_FEATURE_FUNCTIONS_STANDARD
static B1_T_ERROR b1_fn_bltin_iif(B1_VAR *parg1)
{
	*parg1 = *(parg1 + ((*parg1).value.bval ? (uint8_t)1 : (uint8_t)2));

	if(B1_TYPE_TEST_STRING((*parg1).type))
	{
		return B1_RES_ETYPMISM;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_striif(B1_VAR *parg1)
{
	uint8_t arg_index;
	B1_T_ERROR err;

	// argument to free
	arg_index = ((*parg1).value.bval ? (uint8_t)2 : (uint8_t)1);
	err = b1_fn_bltin_len(parg1 + arg_index);
	if(err != B1_RES_OK)
	{
		return err;
	}

	// argument to return
	arg_index = (arg_index == 1) ? (uint8_t)2 : (uint8_t)1;

	*parg1 = *(parg1 + arg_index);

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_len(B1_VAR *parg1)
{
	B1_T_ERROR err;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	(*parg1).type = B1_TYPE_SET(B1_TYPE_INT32, 0);
	(*parg1).value.i32val = *b1_tmp_buf;

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_asc(B1_VAR *parg1)
{
	B1_T_ERROR err;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if(*b1_tmp_buf == 0)
	{
		return B1_RES_EINVARG;
	}

	(*parg1).type = B1_TYPE_SET(B1_TYPE_INT32, 0);
	(*parg1).value.i32val = *(b1_tmp_buf + 1);

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_chr(B1_VAR *parg1)
{
	B1_T_ERROR err;
	uint8_t type;
	B1_T_CHAR buf[2];

	type = B1_TYPE_GET((*parg1).type);
	switch(type)
	{
#ifdef B1_FEATURE_TYPE_SINGLE
		case B1_TYPE_SINGLE:
			buf[1] = (B1_T_CHAR)(*parg1).value.sval;
			break;
#endif
		case B1_TYPE_INT32:
			buf[1] = (B1_T_CHAR)(*parg1).value.i32val;
			break;
		default:
			return B1_RES_ETYPMISM;
	}

	buf[0] = 1;

	err = b1_var_str2var(buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_str(B1_VAR *parg1)
{
	B1_T_ERROR err;
	uint8_t ppz;

	// b1_var_convert uses b1_int_print_zone_width gobal variable to get max. print field width when formatting foating point values
	// set it to 12 (b1_var_convert reserves two positions for unary minus and value delimiter so max. value length is 10 charcters)
	ppz = b1_int_print_zone_width;
	b1_int_print_zone_width = 12;

	err = b1_var_convert(parg1, B1_TYPE_STRING);

	b1_int_print_zone_width = ppz;

	return err;
}

static B1_T_ERROR b1_fn_bltin_val(B1_VAR *parg1)
{
	B1_T_ERROR err;
	uint8_t type;
	const B1_T_CHAR *s;
	B1_TOKENDATA td;
	B1_T_INDEX len;

	type = B1_TYPE_GET((*parg1).type);
	switch(type)
	{
#ifdef B1_FEATURE_TYPE_SINGLE
		case B1_TYPE_SINGLE:
#endif
		case B1_TYPE_INT32:
			return B1_RES_OK;
		case B1_TYPE_STRING:
			break;
		default:
			return B1_RES_ETYPMISM;
	}

	err = b1_var_var2str(parg1, b1_tmp_buf1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	len = *b1_tmp_buf1;

	if(len == (B1_TMP_BUF_LEN - 1))
	{
		return B1_RES_EINVNUM;
	}

	*(b1_tmp_buf1 + len + 1) = 0;

	s = b1_int_progline;
	b1_int_progline = b1_tmp_buf1;
	// copy value to b1_tmp_buf
	err = b1_tok_get(1, B1_TOK_ALLOW_UNARY_OPER | B1_TOK_COPY_VALUE, &td);
	b1_int_progline = s;
	
	if(err != B1_RES_OK)
	{
		return err;
	}

	len = td.length;

	if(len == 0 || !(td.type & B1_TOKEN_TYPE_NUMERIC))
	{
		return B1_RES_EINVNUM;
	}

	s = b1_int_progline;
	b1_int_progline = b1_tmp_buf1;
	err = b1_tok_get(td.offset + len, 0, &td);
	b1_int_progline = s;

	if(err != B1_RES_OK)
	{
		return err;
	}

	if(td.length != 0)
	{
		return B1_RES_EINVNUM;
	}

	// the function converts value copied to b1_tmp_buf variable with b1_tok_get call
	return b1_eval_get_numeric_value(parg1);
}
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
static B1_T_ERROR b1_fn_bltin_abs(B1_VAR *parg1)
{
	return b1_eval_neg(parg1, (*parg1).type, 1);
}

#ifdef B1_FRACTIONAL_TYPE_EXISTS
static B1_T_ERROR b1_fn_bltin_int(B1_VAR *parg1)
{
	(*parg1).value.sval = floorf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_rnd(B1_VAR *parg1)
{
	(*parg1).type = B1_TYPE_SET(B1_TYPE_SINGLE, 0);
	(*parg1).value.sval = b1_ex_rnd_rand();
	return B1_RES_OK;
}
#endif

static B1_T_ERROR b1_fn_bltin_sgn(B1_VAR *parg1)
{
	uint8_t type;

	type = B1_TYPE_GET((*parg1).type);
	switch(type)
	{
#ifdef B1_FEATURE_TYPE_SINGLE
		case B1_TYPE_SINGLE:
			(*parg1).value.sval = ((*parg1).value.sval < 0.0f) ? -1.0f : (((*parg1).value.sval == 0.0f) ? 0.0f : 1.0f);
			break;
#endif
		case B1_TYPE_INT32:
			(*parg1).value.i32val = (int32_t)(((*parg1).value.i32val < 0) ? (int8_t)-1 : (((*parg1).value.i32val == 0) ? (int8_t)0 : (int8_t)1));
			break;
		default:
			return B1_RES_ETYPMISM;
	}

	return B1_RES_OK;
}
#endif

#ifdef B1_FEATURE_FUNCTIONS_MATH_EXTRA
static B1_T_ERROR b1_fn_bltin_atn(B1_VAR *parg1)
{
	(*parg1).value.sval = atanf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_cos(B1_VAR *parg1)
{
	(*parg1).value.sval = cosf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_exp(B1_VAR *parg1)
{
	(*parg1).value.sval = expf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_log(B1_VAR *parg1)
{
	(*parg1).value.sval = logf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_pi(B1_VAR *parg1)
{
	(*parg1).type = B1_TYPE_SET(B1_TYPE_SINGLE, 0);
	(*parg1).value.sval = (float)3.14159265358979323846;
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_sin(B1_VAR *parg1)
{
	(*parg1).value.sval = sinf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_sqr(B1_VAR *parg1)
{
	(*parg1).value.sval = sqrtf((*parg1).value.sval);
	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_tan(B1_VAR *parg1)
{
	(*parg1).value.sval = tanf((*parg1).value.sval);
	return B1_RES_OK;
}
#endif

#ifdef B1_FEATURE_FUNCTIONS_STRING
// MID$(<input_string>, <start_position>, [<length>])
static B1_T_ERROR b1_fn_bltin_mid(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX start, len;

	start = 0;

	if(B1_TYPE_GET((*(parg1 + 2)).type) != B1_TYPE_NULL)
	{
		err = b1_var_convert(parg1 + 2, B1_TYPE_INT32);
		if(err != B1_RES_OK)
		{
			return err;
		}

		if((*(parg1 + 2)).value.i32val < 0)
		{
			return B1_RES_EINVARG;
		}

		len = (B1_T_INDEX)(*(parg1 + 2)).value.i32val;

		// start here stands for <length> parameter presence
		start++;
	}

	// call b1_var_var2str after b1_var_convert function call because b1_var_convert can use b1_tmp_buf variable too
	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	// set length to the entire string length id the last parameter is not present
	if(!start)
	{
		len = *b1_tmp_buf;
	}

	if((*(parg1 + 1)).value.i32val <= 0)
	{
		return B1_RES_EINVARG;
	}
	start = (B1_T_INDEX)(*(parg1 + 1)).value.i32val;

	if(start > *b1_tmp_buf)
	{
		len = 0;
	}

	if(len == 0)
	{
		start = 1;
	}
	else
	{
		if(*b1_tmp_buf - start  + 1 < len)
		{
			len = *b1_tmp_buf - start + 1;
		}
	}

	*(b1_tmp_buf + start - 1) = len;

	return b1_var_str2var(b1_tmp_buf + start - 1, parg1);
}

// INSTR([<start_position>], <string>, <string_to_search>)
static B1_T_ERROR b1_fn_bltin_instr(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX start, len;

	start = 1;

	if(B1_TYPE_GET((*parg1).type) != B1_TYPE_NULL)
	{
		err = b1_var_convert(parg1, B1_TYPE_INT32);
		if(err != B1_RES_OK)
		{
			return err;
		}

		if((*parg1).value.i32val <= 0)
		{
			return B1_RES_EINVARG;
		}

		start = (B1_T_INDEX)(*parg1).value.i32val;
	}

	err = b1_var_var2str(parg1 + 1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	err = b1_var_var2str(parg1 + 2, b1_tmp_buf1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	(*parg1).type = B1_TYPE_SET(B1_TYPE_INT32, 0);
	(*parg1).value.i32val = 0;

	if(*b1_tmp_buf == 0)
	{
		return B1_RES_OK;
	}

	if(*b1_tmp_buf1 == 0)
	{
		(*parg1).value.i32val = start;
		return B1_RES_OK;
	}

	if(start > *b1_tmp_buf)
	{
		return B1_RES_EINVARG;
	}

	len = *b1_tmp_buf - start + 1;
	if(len < *b1_tmp_buf1)
	{
		return B1_RES_OK;
	}

	len -= *b1_tmp_buf1;
	len++;
	while(len-- != 0)
	{
#ifdef B1_FEATURE_LOCALES
		if(b1_t_strcmp_l(b1_tmp_buf1, b1_tmp_buf + start, *b1_tmp_buf1) == 0)
#else
		if(b1_t_strcmpi(b1_tmp_buf1, b1_tmp_buf + start, *b1_tmp_buf1) == 0)
#endif
		{
			(*parg1).value.i32val = start;
			return B1_RES_OK;
		}
		start++;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_ltrim(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX start;
	B1_T_CHAR c;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	start = 1;

	if(*b1_tmp_buf != 0)
	{
		for(; start <= *b1_tmp_buf; start++)
		{
			c = *(b1_tmp_buf + start);
			if(!B1_T_ISBLANK(c))
			{
				break;
			}
		}

		*(b1_tmp_buf + start - 1) = *b1_tmp_buf - start + 1;
	}

	err = b1_var_str2var(b1_tmp_buf + start - 1, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_rtrim(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len;
	B1_T_CHAR c;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	len = *b1_tmp_buf;
	for(; len != 0; len--)
	{
		c = *(b1_tmp_buf + len);
		if(!B1_T_ISBLANK(c))
		{
			break;
		}
	}

	*b1_tmp_buf = len;

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_left(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if((*(parg1 + 1)).value.i32val < 0)
	{
		return B1_RES_EINVARG;
	}

	len = (B1_T_INDEX)(*(parg1 + 1)).value.i32val;

	if(len < *b1_tmp_buf)
	{
		*b1_tmp_buf = len;
	}

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_right(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len, start;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if((*(parg1 + 1)).value.i32val < 0)
	{
		return B1_RES_EINVARG;
	}

	start = 0;
	len = (B1_T_INDEX)(*(parg1 + 1)).value.i32val;

	if(len < *b1_tmp_buf)
	{
		start = *b1_tmp_buf - len;
		*(b1_tmp_buf + start) = len;
	}

	err = b1_var_str2var(b1_tmp_buf + start, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_lset(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len, i;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if((*(parg1 + 1)).value.i32val < 0)
	{
		return B1_RES_EINVARG;
	}

	len = (B1_T_INDEX)(*(parg1 + 1)).value.i32val;

	for(i = *b1_tmp_buf; i < len; )
	{
		i++;
		*(b1_tmp_buf + i) = B1_T_C_SPACE;
	}

	*b1_tmp_buf = len;

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_rset(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len, old_len, spnum;
	B1_T_CHAR c;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if((*(parg1 + 1)).value.i32val < 0)
	{
		return B1_RES_EINVARG;
	}

	len = (B1_T_INDEX)(*(parg1 + 1)).value.i32val;
	old_len = *b1_tmp_buf;

	if(old_len < len)
	{
		spnum = len - old_len;

		while(old_len != 0)
		{
			c = *(b1_tmp_buf + old_len);
			*(b1_tmp_buf + old_len + spnum) = c;
			old_len--;
		}

		while(old_len < spnum)
		{
			old_len++;
			*(b1_tmp_buf + old_len) = B1_T_C_SPACE;
		}
	}

	*b1_tmp_buf = len;

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_ucase(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len;
	B1_T_CHAR c;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	for(len = *b1_tmp_buf; len != 0; len--)
	{
		c = *(b1_tmp_buf + len);
#ifdef B1_FEATURE_LOCALES
		*(b1_tmp_buf + len) = b1_t_toupper_l(c);
#else
		*(b1_tmp_buf + len) = B1_T_TOUPPER(c);
#endif
	}

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}

static B1_T_ERROR b1_fn_bltin_lcase(B1_VAR *parg1)
{
	B1_T_ERROR err;
	B1_T_INDEX len;
	B1_T_CHAR c;

	err = b1_var_var2str(parg1, b1_tmp_buf);
	if(err != B1_RES_OK)
	{
		return err;
	}

	for(len = *b1_tmp_buf; len != 0; len--)
	{
		c = *(b1_tmp_buf + len);
#ifdef B1_FEATURE_LOCALES
		*(b1_tmp_buf + len) = b1_t_tolower_l(c);
#else
		*(b1_tmp_buf + len) = B1_T_TOLOWER(c);
#endif
	}

	err = b1_var_str2var(b1_tmp_buf, parg1);
	if(err != B1_RES_OK)
	{
		return err;
	}

	return B1_RES_OK;
}
#endif

#if defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING) || defined(B1_FEATURE_FUNCTIONS_USER)
static B1_T_ERROR b1_fn_find(uint8_t fn_type, B1_T_IDHASH name_hash, B1_FN **fn_ptr)
{
	B1_T_ERROR err;
	B1_FN *fns;
	uint16_t size;
	B1_T_INDEX fn_num;

#if (defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)) && defined(B1_FEATURE_FUNCTIONS_USER)
	if(fn_type == 0)
	{
#endif
#if defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)
		fns = (B1_FN *)b1_fn_bltin;
		size = sizeof(B1_BLTIN_FN);
		fn_num = B1_FN_BLTIN_COUNT;
#endif
#if (defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)) && defined(B1_FEATURE_FUNCTIONS_USER)
	}
	else
	{
#endif
#ifdef B1_FEATURE_FUNCTIONS_USER
		fns = (B1_FN *)b1_fn_udef_fns;
		size = sizeof(B1_UDEF_FN);
		fn_num = B1_MAX_UDEF_FN_NUM;
#endif
#if (defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)) && defined(B1_FEATURE_FUNCTIONS_USER)
	}
#endif

	err = B1_RES_EUNKIDENT;

	for(; fn_num != 0; fn_num--)
	{
		*fn_ptr = fns;

		if(B1_IDENT_TEST_FLAGS_BUSY((*fns).id.flags))
		{
			if(name_hash == (*fns).id.name_hash)
			{
				err = B1_RES_OK;
				break;
			}
		}
		else
		{
			break;
		}

		fns = (B1_FN *)(((uint8_t *)fns) + size);
	}

	if(fn_num == 0)
	{
		*fn_ptr = NULL;
	}

	return err;
}
#endif

B1_T_ERROR b1_fn_get_params(B1_T_IDHASH name_hash, B1_FN **fn_ptr)
{
	B1_T_ERROR err;

	err = B1_RES_EUNKIDENT;

#if defined(B1_FEATURE_FUNCTIONS_STANDARD) || defined(B1_FEATURE_FUNCTIONS_MATH_BASIC) || defined(B1_FEATURE_FUNCTIONS_MATH_EXTRA) || defined(B1_FEATURE_FUNCTIONS_STRING)
	err = b1_fn_find(0, name_hash, fn_ptr);
#endif

#ifdef B1_FEATURE_FUNCTIONS_USER
	if(err == B1_RES_EUNKIDENT)
	{
		err = b1_fn_find(1, name_hash, fn_ptr);
	}
#endif

	return err;
}

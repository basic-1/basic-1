/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1types.c: interpreter string constants, variable data types, conversion
 to string and vice versa
*/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "b1types.h"
#include "b1err.h"


// global string constants
const B1_T_CHAR _EQ[] = { 1, B1_T_C_EQ };
const B1_T_CHAR _THEN[] = { 4, 'T', 'H', 'E', 'N' };
const B1_T_CHAR _GOTO[] = { 4, 'G', 'O', 'T', 'O' };
const B1_T_CHAR _GOSUB[] = { 5, 'G', 'O', 'S', 'U', 'B' };
const B1_T_CHAR _TO[] = { 2, 'T', 'O' };
const B1_T_CHAR _STEP[] = { 4, 'S', 'T', 'E', 'P' };
const B1_T_CHAR _COMMA[] = { 1, B1_T_C_COMMA };
const B1_T_CHAR _SEMICOLON[] = { 1, B1_T_C_SEMICOLON };
const B1_T_CHAR _CLBRACKET[] = { 1, B1_T_C_CLBRACK };
const B1_T_CHAR _PROMPT[] = { 2, B1_T_C_QUESTION, B1_T_C_SPACE };
#ifdef B1_FEATURE_STMT_DATA_READ
const B1_T_CHAR _DATA[] = {4, 'D', 'A', 'T', 'A' };
const B1_T_CHAR _READ[] = { 4, 'R', 'E', 'A', 'D' };
const B1_T_CHAR _RESTORE[] = { 7, 'R', 'E', 'S', 'T', 'O', 'R', 'E' };
#endif
const B1_T_CHAR _FOR[] = {3, 'F', 'O', 'R' };
const B1_T_CHAR _NEXT[] = { 4, 'N', 'E', 'X', 'T' };
#ifdef B1_FEATURE_FUNCTIONS_USER
const B1_T_CHAR _DEF[] = {3, 'D', 'E', 'F' };
#endif
const B1_T_CHAR _AS[] = { 2, 'A', 'S' };
const B1_T_CHAR _ON[] = { 2, 'O', 'N' };
const B1_T_CHAR _OFF[] = { 3, 'O', 'F', 'F' };
const B1_T_CHAR _BASE[] = { 4, 'B', 'A', 'S', 'E' };
const B1_T_CHAR _EXPLICIT[] = { 8, 'E', 'X', 'P', 'L', 'I', 'C', 'I', 'T' };
const B1_T_CHAR _MARGIN[] = { 6, 'M', 'A', 'R', 'G', 'I', 'N' };
const B1_T_CHAR _ZONEWIDTH[] = { 9, 'Z', 'O', 'N', 'E', 'W', 'I', 'D', 'T', 'H' };
const B1_T_CHAR _INPUTECHO[] = { 9, 'I', 'N', 'P', 'U', 'T', 'E', 'C', 'H', 'O' };
const B1_T_CHAR _REM[] = { 3, 'R', 'E', 'M' };
const B1_T_CHAR _OPTION[] = { 6, 'O', 'P', 'T', 'I', 'O', 'N' };
const B1_T_CHAR _ELSEIF[] = { 6, 'E', 'L', 'S', 'E', 'I', 'F' };
const B1_T_CHAR _ELSE[] = { 4, 'E', 'L', 'S', 'E' };
const B1_T_CHAR _IF[] = { 2, 'I', 'F' };
const B1_T_CHAR _RETURN[] = { 6, 'R', 'E', 'T', 'U', 'R', 'N' };
const B1_T_CHAR _PRINT[] = { 5, 'P', 'R', 'I', 'N', 'T' };
const B1_T_CHAR _INPUT[] = { 5, 'I', 'N', 'P', 'U', 'T' };
const B1_T_CHAR _DIM[] = { 3, 'D', 'I', 'M' };
#ifdef B1_FEATURE_STMT_ERASE
const B1_T_CHAR _ERASE[] = { 5, 'E', 'R', 'A', 'S', 'E' };
#endif
#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
#ifdef B1_FRACTIONAL_TYPE_EXISTS
const B1_T_CHAR _RANDOMIZE[] = { 9, 'R', 'A', 'N', 'D', 'O', 'M', 'I', 'Z', 'E' };
#endif
#endif
const B1_T_CHAR _SET[] = { 3, 'S', 'E', 'T' };
const B1_T_CHAR _END[] = { 3, 'E', 'N', 'D' };
const B1_T_CHAR _LET[] = { 3, 'L', 'E', 'T' };
#ifdef B1_FEATURE_TYPE_SINGLE
const B1_T_CHAR _SINGLE[] = { 6, 'S', 'I', 'N', 'G', 'L', 'E' };
#endif
const B1_T_CHAR _STRING[] = { 6, 'S', 'T', 'R', 'I', 'N', 'G' };
const B1_T_CHAR _INT[] = { 3, 'I', 'N', 'T' };

#ifdef B1_FEATURE_DEBUG
// string constant to designate FOR loop special variables (non-accessible directly from program)
const B1_T_CHAR _DBG_FORVAR[] = { 8, B1_T_C_LT, 'F', 'O', 'R', 'V', 'A', 'R', B1_T_C_GT };
const B1_T_CHAR _DBG_TYPE_OPBR[] = { 2, B1_T_C_SPACE, B1_T_C_OPBRACK };
const B1_T_CHAR _DBG_TYPE_CLBR[] = { 3, B1_T_C_CLBRACK, B1_T_C_COLON, B1_T_C_SPACE };
// string constant for unallocated arrays
const B1_T_CHAR _DBG_UNALLOC[] = { 9, B1_T_C_LT, 'U', 'N', 'A', 'L', 'L', 'O', 'C', B1_T_C_GT };
const B1_T_CHAR _DBG_DELIM[] = { 2, B1_T_C_COMMA, B1_T_C_SPACE };
#endif


// converts C string to uint16_t value
B1_T_ERROR b1_t_strtoui16(const B1_T_CHAR *cs, uint16_t *value)
{
	uint8_t unplus;
	uint16_t val, newval;
	B1_T_CHAR c;
	B1_T_INDEX offset;

	unplus = 1;
	val = 0;
	offset = 0;

	while(1)
	{
		c = *cs++;

		if(B1_T_ISCSTRTERM(c))
		{
			if(offset == 0 || unplus)
			{
				return B1_RES_EINVNUM;
			}

			break;
		}

		offset++;

		if(B1_T_ISPLUS(c))
		{
			if(unplus)
			{
				unplus = 0;
				continue;
			}

			return B1_RES_EINVNUM;
		}

		unplus = 0;

		if(B1_T_ISDIGIT(c))
		{
			newval = val * 10 + B1_T_CHAR2NUM(c);

			if(newval < val)
			{
				return B1_RES_ENUMOVF;
			}

			val = newval;
		}
		else
		{
			return B1_RES_EINVNUM;
		}
	}

	if(value != NULL)
	{
		*value = val;
	}

	return B1_RES_OK;
}

// not needed at the moment
// converts C string to int16_t value
/*B1_T_ERROR b1_t_strtoi16(const B1_T_CHAR *cs, int16_t *value)
{
	uint8_t neg;
	B1_T_ERROR err;
	uint16_t uval;

	uval = 0;

	neg = B1_T_ISMINUS(*cs);

	if(neg)
	{
		cs++;
	}

	err = b1_t_strtoui16(cs, &uval);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if(!(uval < (((uint16_t)INT16_MAX) + 1)))
	{
		if(!neg || !(uval == (((uint16_t)INT16_MAX) + 1)))
		{
			return B1_RES_ENUMOVF;
		}
	}

	if(value != NULL)
	{
		*value = neg ? -(int16_t)uval : (int16_t)uval;
	}

	return B1_RES_OK;
}*/

// two functions below are not needed at the moment, b1_t_strtoi32 without b1_t_strtoui32 implementation is used instead
/*
// converts C string to uint32_t value
static B1_T_ERROR b1_t_strtoui32(const B1_T_CHAR *cs, uint32_t *value)
{
	uint8_t unplus;
	uint32_t val, newval;
	B1_T_CHAR c;
	B1_T_INDEX offset;

	unplus = 1;
	val = 0;
	offset = 0;

	while(1)
	{
		c = *cs++;

		if(B1_T_ISCSTRTERM(c))
		{
			if(offset == 0 || unplus)
			{
				return B1_RES_EINVNUM;
			}

			break;
		}

		offset++;

		if(B1_T_ISPLUS(c))
		{
			if(unplus)
			{
				unplus = 0;
				continue;
			}

			return B1_RES_EINVNUM;
		}

		unplus = 0;

		if(B1_T_ISDIGIT(c))
		{
			newval = val * 10 + B1_T_CHAR2NUM(c);

			if(newval < val)
			{
				return B1_RES_ENUMOVF;
			}

			val = newval;
		}
		else
		{
			return B1_RES_EINVNUM;
		}
	}

	if(value != NULL)
	{
		*value = val;
	}

	return B1_RES_OK;
}

// converts C string to int32_t value
B1_T_ERROR b1_t_strtoi32(const B1_T_CHAR *cs, int32_t *value)
{
	uint8_t neg;
	B1_T_ERROR err;
	uint32_t uval;

	uval = 0;

	neg = B1_T_ISMINUS(*cs);

	if(neg)
	{
		cs++;
	}

	err = b1_t_strtoui32(cs, &uval);
	if(err != B1_RES_OK)
	{
		return err;
	}

	if(!(uval < (((uint32_t)INT32_MAX) + 1)))
	{
		if(!neg || !(uval == (((uint32_t)INT32_MAX) + 1)))
		{
			return B1_RES_ENUMOVF;
		}
	}

	if(value != NULL)
	{
		*value = neg ? -(int32_t)uval : (int32_t)uval;
	}

	return B1_RES_OK;
}
*/

// converts C string to int32_t value
B1_T_ERROR b1_t_strtoi32(const B1_T_CHAR *cs, int32_t *value)
{
	B1_T_CHAR c;
	int8_t neg, n;
	int32_t val, mval;

	val = 0;
	neg = 0;

	while(1)
	{
		c = *cs++;

		if(B1_T_ISCSTRTERM(c))
		{
			break;
		}

		if(neg == 0)
		{
			if(B1_T_ISMINUS(c))
			{
				neg++;
				continue;
			}

			neg--;

			if(B1_T_ISPLUS(c))
			{
				continue;
			}
		}

		if(B1_T_ISDIGIT(c))
		{
			n = B1_T_CHAR2NUM(c);

			if(neg == 1)
			{
				mval = INT32_MIN;

				if(val < INT32_MIN / 10)
				{
					return B1_RES_ENUMOVF;
				}
			}
			else
			{
				mval = INT32_MAX;

				if(val > INT32_MAX / 10)
				{
					return B1_RES_ENUMOVF;
				}
			}

			val *= 10;

			mval -= val;

			if(neg == 1)
			{
				n = -n;

				if(mval > n)
				{
					return B1_RES_ENUMOVF;
				}
			}
			else
			{
				if(mval < n)
				{
					return B1_RES_ENUMOVF;
				}
			}

			val += n;

			continue;
		}

		return B1_RES_EINVNUM;
	}

	if(neg == 0)
	{
		return B1_RES_EINVNUM;
	}

	if(value != NULL)
	{
		*value = val;
	}

	return B1_RES_OK;
}

B1_T_ERROR b1_t_i32tostr(int32_t value, B1_T_CHAR *sbuf, B1_T_INDEX buflen)
{
#ifdef B1_FEATURE_TYPE_SINGLE
#ifdef B1_FEATURE_UNICODE_UCS2
	char tmpbuf[50];
	B1_T_INDEX len, i;
#endif

	if(buflen < 13)
	{
		return B1_RES_EBUFSMALL;
	}

#ifdef B1_FEATURE_UNICODE_UCS2
	len = (B1_T_CHAR)sprintf(tmpbuf, "%ld", (long int)value);
	*sbuf = len;
	for(i = 0; i <= len; i++)
	{
		*(sbuf + 1 + i) = (uint8_t)tmpbuf[i];
	}
#else
	*sbuf = (B1_T_CHAR)sprintf((char *)(sbuf + 1), "%ld", (long int)value);
#endif

	return B1_RES_OK;
#else
	B1_T_INDEX start, end;
	uint8_t n, dnum;
	B1_T_CHAR c;

	dnum = 0;

	if(value < 0)
	{
		if(buflen < 3)
		{
			return B1_RES_EBUFSMALL;
		}

		value = -value;

		sbuf[1] = B1_T_C_MINUS;
		dnum++;
	}

	end = dnum;
	end++;

	start = end;

	for(; ; end++)
	{
		if(end == buflen)
		{
			return B1_RES_EBUFSMALL;
		}

		n = ((uint32_t)value) % 10;
		value = ((uint32_t)value) / 10;

		sbuf[end] = B1_T_NUM2CHAR(n);

		if(value == 0)
		{
			break;
		}
	}

	sbuf[0] = end;

	for(; start < end; start++, end--)
	{
		c = sbuf[start];
		sbuf[start] = sbuf[end];
		sbuf[end] = c;
	}

	return B1_RES_OK;
#endif
}

#ifdef B1_FEATURE_TYPE_SINGLE
// temporary implementation: just a replace to atof
B1_T_ERROR b1_t_strtosingle(const B1_T_CHAR *cs, float *value)
{
#ifdef B1_FEATURE_UNICODE_UCS2
	char tmpbuf[50];
	B1_T_INDEX i;

	for(i = 0; i < 49; i++)
	{
		if(cs[i] == 0)
		{
			break;
		}

		tmpbuf[i] = (uint8_t)cs[i];
	}

	tmpbuf[i] = 0;

	*value = (float)atof(tmpbuf);
#else
	*value = (float)atof((const char *)cs);
#endif

	return B1_RES_OK;
}

// trims trailing zeroes and point
static uint8_t b1_t_single_trim_zeroes(B1_T_CHAR *s)
{
	B1_T_INDEX start, i, len;
	B1_T_CHAR c;

	i = *s;
	len = i;
	start = B1_T_ISMINUS(*(s + 1)) ? (B1_T_INDEX)2 : (B1_T_INDEX)1;

	for(; i >= start; i--)
	{
		c = *(s + i);
		
		if(c == B1_T_C_0)
		{
			len--;
			continue;
		}

		if(c == B1_T_C_POINT)
		{
			if(i == start)
			{
				*(s + i) = B1_T_C_0;
			}
			else
			{
				len--;
			}
		}

		break;
	}

	*s = len;

	return (c == B1_T_C_POINT) && (i == start);
}

static int8_t b1_t_single_round(B1_T_CHAR *s, B1_T_INDEX extra_digits)
{
	B1_T_INDEX i, out, start;
	uint8_t carry;
	B1_T_CHAR c;

	start = B1_T_ISMINUS(*(s + 1)) ? (uint8_t)2 : (uint8_t)1;

	// copy and round
	for(carry = 0, i = *s, out = i; i >= start; i--)
	{
		c = *(s + i);

		if(c != B1_T_C_POINT)
		{
			if(extra_digits)
			{
				extra_digits--;

				if(!extra_digits && c >= B1_T_C_5)
				{
					carry++;
				}

				continue;
			}

			if(carry)
			{
				if(c == B1_T_C_9)
				{
					c = B1_T_C_0;
				}
				else
				{
					c++;
					carry--;
				}
			}
		}

		*(s + out--) = c;
	}

	if(carry)
	{
		*(s + out--) = B1_T_C_1;
	}

	// set new value length and shift the string to the left
	out++;
	i = *s;
	*s = i + start - out;
	for(; out <= i; out++, start++)
	{
		*(s + start) = *(s + out);
	}

	return carry;
}

// converts floating point values to exp. form, supports large values without fractional part and values less than 1
// (doesn't work for values with point in th middle of the significand)
static void b1_t_single_to_exp_form(B1_T_CHAR *s, int8_t e, uint8_t max_len)
{
	B1_T_INDEX start, end, cend;

	start = B1_T_ISMINUS(*(s + 1)) ? (B1_T_INDEX)2 : (B1_T_INDEX)1;

	// copy significand with rounding
	// cend - the ending position of significand part that should be copied
	cend = start + max_len - 1 - 1;
	if(e != 0)
	{
		// min. exp. length
		cend -= 2;
		// abs(E) > 9
		if(e > 9 || e < -9)
		{
			cend--;
		}
		// negative exp.
		if(e < 0)
		{
			cend--;
		}
	}

	end = *s;

	if(cend > end)
	{
		cend = start;
	}

	if(cend < start)
	{
		cend = start;
	}

	e += b1_t_single_round(s, end - cend);
	
	b1_t_single_trim_zeroes(s);
	
	end = *s;

	// insert point
	if(end != start)
	{
		// shift fractional part to the right
		for(cend = end; cend > start; cend--)
		{
			*(s + cend + 1) = *(s + cend);
		}

		// correct string size and insert point
		end++;
		*s = end;
		*(s + start + 1) = B1_T_C_POINT;
	}

	// write exponent
	if(e != 0)
	{
		// put 'E' letter
		*(s + ++end) = B1_T_C_UCE;
		
		if(e < 0)
		{
			*(s + ++end) = B1_T_C_MINUS;

			e = -e;
		}

		if(e >= 10)
		{
			*(s + ++end) = B1_T_NUM2CHAR((uint8_t)e / (uint8_t)10);

			e = (uint8_t)e % (uint8_t)10;
		}

		*(s + ++end) = B1_T_NUM2CHAR(e);
	}

	// write string length
	*s = end;
}

B1_T_ERROR b1_t_singletostr(float value, B1_T_CHAR *sbuf, B1_T_INDEX buflen, uint8_t max_len)
{
	B1_T_INDEX start, end, n;
	uint8_t neg, use_exp, lt1;
	int8_t e;
#ifdef B1_FEATURE_UNICODE_UCS2
	char tmpbuf[50];
#endif

	// negative value
	neg = 0;
	// use exponential form
	use_exp = 0;
	// less than 1 value
	lt1 = 0;

	// <string length><unary minus>0.<up to 47 characters long significand><C string terminator>
	// 0              1            234                                   5051
	if(buflen < 52)
	{
		return B1_RES_EBUFSMALL;
	}

	// check for negative value
#ifdef signbit
	if(signbit(value))
#else
	// IEEE 754 single precision fp value
	if(*((int32_t *)&value) < 0)
#endif
	{
		neg++;
		value = -value;
	}

	// process zero values separately
	if(value == 0.0f)
	{
		*sbuf = neg + 1;
		*(sbuf + 1) = neg ? B1_T_C_MINUS : B1_T_C_0;
		*(sbuf + 2) = B1_T_C_0;
		return B1_RES_OK;
	}

	if(value < 1.0f)
	{
		lt1++;
	}

#ifdef B1_FEATURE_UNICODE_UCS2
	n = (B1_T_INDEX)sprintf(tmpbuf, lt1 ? "%.47f" : "%.9f", value);
	for(start = 0; start <= n; start++)
	{
		*(sbuf + neg + 1 + start) = (uint8_t)tmpbuf[start];
	}
#else
	n = (B1_T_INDEX)sprintf((char *)(sbuf + neg + 1), lt1 ? "%.47f" : "%.9f", value);
#endif

	if(neg)
	{
		*(sbuf + 1) = B1_T_C_MINUS;
		n++;
	}

	*(sbuf) = n;


	// find significand (from start to end), exponent (e)
	start = neg + 1;
	end = n;
	if(lt1)
	{
		// integer part length is not needed for less than 1 values
		// take into account leading zero and point
		start += 2;
		// calculate e and start
		e = (int8_t)start;
		for(; *(sbuf + start) == B1_T_C_0; start++);
		e -= start;
		e--;

		if(e < -3 && max_len + e < 3)
		{
			use_exp++;
		}
		else
		{
			// digits to leave
			n = max_len - 1;
			
			if(n <= -(e + 1))
			{
				// value length exceeds max_len, copy at least one digit of significand
				n = -e;
			}

			b1_t_single_round(sbuf, 47 - n);
			b1_t_single_trim_zeroes(sbuf);
		}
	}
	else
	{
		// find integer part length, exponent is not needed here
		for(n = 0; *(sbuf + start + n) != B1_T_C_POINT; n++);

		if(n > max_len)
		{
			use_exp++;
		}
		else
		{
			// fractional part length
			e = max_len - n;
			// reserve one place for point
			if(e != 0)
			{
				e--;
			}

			// round value
			n += b1_t_single_round(sbuf, 9 - e);
			if(n > max_len)
			{
				use_exp++;
			}
			else
			{
				b1_t_single_trim_zeroes(sbuf);
			}
		}
	}

	if(use_exp)
	{
		if(!lt1)
		{
			// calculate new value length and exponent
			end = neg + n;
			e = end - start;
		}

		// move significand at the beginning of the string
		for(n = start - neg - 1; start <= end; start++)
		{
			*(sbuf + start - n) = *(sbuf + start);
		}
		*sbuf = end - n;

		b1_t_single_to_exp_form(sbuf, e, max_len);
	}

	if(*(sbuf + neg + 1) == B1_T_C_0)
	{
		// remove first zero if he value is less than 1
		end = *sbuf;
		*sbuf = end - 1;

		for(start = neg + 1; start < end; start++)
		{
			*(sbuf + start) = *(sbuf + start + 1);
		}
	}

	return B1_RES_OK;
}
#endif

int8_t b1_t_strcmpi(const B1_T_CHAR *s1, const B1_T_CHAR *s2data, B1_T_INDEX s2len)
{
	B1_T_INDEX s1len, i;
	B1_T_CHAR c1, c2;
	
	i = 0;
	s1len = *s1++;

	while(1)
	{
		if(i == s1len && i == s2len)
		{
			break;
		}

		if(i == s1len)
		{
			return -1;
		}

		if(i == s2len)
		{
			return 1;
		}

		c1 = B1_T_TOUPPER(*(s1 + i));
		c2 = B1_T_TOUPPER(*(s2data + i));

		if(c1 > c2)
		{
			return 1;
		}
		
		if(c1 < c2)
		{
			return -1;
		}

		i++;
	}

	return 0;
}

/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 exio.c: input/output functions (reading from stdin, writing to stdout)
*/


#include <stdio.h>

#include "b1err.h"


B1_T_ERROR b1_ex_io_print_char(B1_T_CHAR c)
{
	putchar(c);
	return B1_RES_OK;
}

B1_T_ERROR b1_ex_io_print_newline()
{
	// putchar translates '\n' to "\r\n" by itself for text output
	putchar('\n');
	return B1_RES_OK;
}

B1_T_ERROR b1_ex_io_print_margin_newline()
{
	// putchar translates '\n' to "\r\n" by itself for text output
	putchar('\n');
	return B1_RES_OK;
}

B1_T_ERROR b1_ex_io_input_char(B1_T_CHAR *c)
{
	int ic;

	ic = getchar();

	if(ic == EOF)
	{
		if(feof(stdin))
		{
			return B1_RES_EEOF;
		}

		return B1_RES_EENVFAT;
	}

	*c = (B1_T_CHAR)ic;

	return B1_RES_OK;
}

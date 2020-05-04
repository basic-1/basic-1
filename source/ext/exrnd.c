/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 exrndsmp.c: random values generator
*/


// include "b1types.h" for B1_FRACTIONAL_TYPE_EXISTS macro
#include "b1types.h"


#ifdef B1_FEATURE_FUNCTIONS_MATH_BASIC
#ifdef B1_FRACTIONAL_TYPE_EXISTS
#include <stdlib.h>
#include <time.h>


float b1_ex_rnd_rand()
{
	int r = rand();
	if(r == RAND_MAX)
	{
		r--;
	}
	return ((float)r) / (float)(RAND_MAX);
}

void b1_ex_rnd_randomize(uint8_t init)
{
	srand(init ?
		(unsigned int)0 :
		((unsigned int)time(NULL) * rand())
		);
}
#endif
#endif

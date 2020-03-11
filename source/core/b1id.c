/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1id.c: hashes for identifiers (variable and function names)
*/


#include "b1id.h"
#include "b1types.h"


// calculates BASIC identifier hash (for 16 bit B1_T_IDHASH type)
B1_T_IDHASH b1_id_calc_hash(const B1_T_CHAR *data, B1_T_INDEX data_size)
{
	uint8_t b;
	B1_T_IDHASH hash;
	B1_T_INDEX i;

	hash = 0xFFFF;

	do
	{
		for(i = 0; i < data_size; i++)
		{
			b = *(((const uint8_t *)data) + i);

			if(B1_T_ISALPHA(b))
			{
				b = B1_T_TOLOWER(b);
			}
			else
			if(B1_T_ISCSTRTERM(b))
			{
				continue;
			}

			hash += b;
			b ^= (uint8_t)(hash >> 8);
			hash <<= 5;
			hash += b;
		}

	} while((uint8_t)(hash >> 8) == 0);

	return hash;
}

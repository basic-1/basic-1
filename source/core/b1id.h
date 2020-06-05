/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1id.c: definitions and types for identifiers (variable and function names)
*/


#ifndef _B1_IDENT_
#define _B1_IDENT_

#include <stdint.h>

#include "b1feat.h"
#include "b1itypes.h"


#define B1_IDENT_FLAGS_BUSY ((uint8_t)0x1)
#define B1_IDENT_FLAGS_IDTYPE_FN ((uint8_t)0x2)
#define B1_IDENT_FLAGS_ARGNUM_MASK ((uint8_t)0x1C)
#define B1_IDENT_FLAGS_ARGNUM_SHIFT ((uint8_t)0x2)

#define B1_IDENT_GET_FLAGS_ARGNUM(FLAGS) ((((uint8_t)(FLAGS)) & B1_IDENT_FLAGS_ARGNUM_MASK) >> B1_IDENT_FLAGS_ARGNUM_SHIFT)

#define B1_IDENT_TEST_FLAGS_BUSY(FLAGS) (((uint8_t)(FLAGS)) & B1_IDENT_FLAGS_BUSY)
#define B1_IDENT_TEST_FLAGS_IDTYPE_VAR(FLAGS) (!B1_IDENT_TEST_FLAGS_IDTYPE_FN(FLAGS))
#define B1_IDENT_TEST_FLAGS_IDTYPE_FN(FLAGS) (((uint8_t)(FLAGS)) & B1_IDENT_FLAGS_IDTYPE_FN)

#define B1_IDENT_FLAGS_SET_VAR(ARGNUM) ((B1_IDENT_FLAGS_BUSY) | ((((uint8_t)(ARGNUM)) << B1_IDENT_FLAGS_ARGNUM_SHIFT) & B1_IDENT_FLAGS_ARGNUM_MASK))
#define B1_IDENT_FLAGS_SET_VAR_FREE ((uint8_t)0x0)

#define B1_IDENT_FLAGS_FN_BLTIN ((uint8_t)0x20)

#define B1_IDENT_TEST_FLAGS_FN_BLTIN(FLAGS) (((uint8_t)(FLAGS)) & B1_IDENT_FLAGS_FN_BLTIN)

#define B1_IDENT_FLAGS_SET_FN(ARGNUM, BLTIN) ((B1_IDENT_FLAGS_BUSY) | ((((uint8_t)(ARGNUM)) << B1_IDENT_FLAGS_ARGNUM_SHIFT) & B1_IDENT_FLAGS_ARGNUM_MASK) | ((BLTIN) ? B1_IDENT_FLAGS_FN_BLTIN : (uint8_t)0))


typedef struct
{
	uint16_t name_hash;
	uint8_t flags;
#ifdef B1_FEATURE_DEBUG
	B1_T_CHAR name[B1_MAX_IDENTIFIER_LEN + 1];
#endif
} B1_ID;


extern B1_T_IDHASH b1_id_calc_hash(const B1_T_CHAR *data, B1_T_INDEX data_size);

#endif

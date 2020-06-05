/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 b1dbg.c: functions for debugging
*/

#include "b1feat.h"

#ifndef _B1_DEBUG_
#define _B1_DEBUG_

#include "b1var.h"


#ifdef B1_FEATURE_DEBUG
extern B1_T_ERROR b1_dbg_get_var_dump(const B1_NAMED_VAR *var, B1_T_CHAR *sbuf, B1_T_INDEX buflen);
#endif

#endif

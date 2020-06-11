/*
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 int.h: interpreter constants and types
*/


#ifndef _B1_INT_
#define _B1_INT_

#include <stdint.h>

#include "b1feat.h"
#include "b1itypes.h"
#include "b1var.h"


#define B1_MAX_STMT_NEST_DEPTH ((B1_T_INDEX)10)

#define B1_DEF_PRINT_MARGIN 80
#define B1_DEF_PRINT_ZONE_WIDTH 10

#define B1_INT_STATE_IF ((uint8_t)0x80)
#define B1_INT_STATE_GOSUB ((uint8_t)0x40)
#define B1_INT_STATE_FOR ((uint8_t)0x20)
#define B1_INT_STATE_ON ((uint8_t)0x10)

#define B1_INT_STATE_IF_EXEC ((uint8_t)0x1)
#define B1_INT_STATE_IF_NEXT ((uint8_t)0x2)
#define B1_INT_STATE_IF_SKIP ((uint8_t)0x4)

#define B1_INT_STATE_FOR_NEXT_CHECKED ((uint8_t)0x1)
#define B1_INT_STATE_FOR_NEG_STEP ((uint8_t)0x2)

#define B1_INT_STATE_ON_POS_MASK ((uint8_t)0x7)

#define B1_INT_STATE_ON_SET(POS) (B1_INT_STATE_ON | B1_INT_STATE_ON_POS_GET(POS))
#define B1_INT_STATE_ON_POS_GET(STATE) (((uint8_t)STATE) & B1_INT_STATE_ON_POS_MASK)


// call stack structure (for IF, GOSUB and FOR statements)
typedef struct
{
	uint8_t state;
	B1_NAMED_VAR *var;
	B1_T_PROG_LINE_CNT ret_line_cnt;
} B1_INT_STMT_STK_REC;


extern B1_T_CHAR b1_tmp_buf[B1_TMP_BUF_LEN];
extern B1_T_CHAR b1_tmp_buf1[B1_TMP_BUF_LEN];

extern const B1_T_CHAR *b1_int_progline;
extern B1_T_PROG_LINE_CNT b1_int_curr_prog_line_cnt;
extern B1_T_INDEX b1_int_curr_prog_line_offset;

extern uint8_t b1_int_opt_base_val;
extern uint8_t b1_int_opt_explicit_val;

extern uint8_t b1_int_print_zone_width;
extern uint8_t b1_int_print_curr_pos;

extern uint8_t b1_int_input_echo;

#ifdef B1_FEATURE_STMT_DATA_READ
// DATA statement counters
extern uint8_t b1_int_data_state;
extern B1_T_PROG_LINE_CNT b1_int_data_curr_line_cnt;
extern B1_T_INDEX b1_int_data_curr_line_offset;
#endif


extern B1_T_ERROR b1_int_reset();
extern B1_T_ERROR b1_int_get_type_by_type_spec(B1_T_CHAR type_spec_char, uint8_t expl_type, uint8_t *res_type);
extern B1_T_ERROR b1_int_prerun();
extern B1_T_ERROR b1_int_run();


#endif

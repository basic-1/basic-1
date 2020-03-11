# BASIC1
*just one more BASIC interpreter*

# Features
- supports classic BASIC dialect with various extensions
- simple line-by-line interpreter
- configurable language features/extensions
- interpreter core written in C
- embeddable: memory functions, input/output and caching detached from core
- licensed under MIT license

# Statements
- DATA
- DEF
- DIM
- ELSE
- ELSEIF ... THEN
- ERASE
- FOR ... TO ... [STEP]
- GOTO
- GOSUB
- IF ... THEN
- INPUT
- LET
- NEXT
- ON ... GOTO
- ON ... GOSUB
- OPTION BASE | EXPLICIT
- PRINT
- RANDOMIZE
- READ
- REM
- RESTORE
- RETURN
- SET MARGIN | ZONEWIDTH | INPUTECHO

# Data types
- SINGLE (32-bit floating point)
- INT (32-bit integer)
- STRING

# More
- optional line numbers
- optional LET statement
- case-insensitive statement, variable and function names
- one- and two-dimensional arrays
- relational operators can be used with string type
- automatic numeric to string conversion
- functions: ABS, INT, RND, SGN, LEN, ASC, CHR$, STR$, VAL
- more functions: IIF, IIF$, ATN, COS, EXP, LOG, PI, SIN, SQR, TAN
- more functions: MID$, INSTR, LTRIM$, RTRIM$, LEFT$, RIGHT$, LSET$, RSET$, UCASE$, LCASE$

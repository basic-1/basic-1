# BASIC1 language syntax (not finished yet)  
  
## Introduction  
  
BASIC1 language program is a sequence of text strings (program lines). The interpreter starts executing program from the first line consecutively. Every program line consists of line number, a single BASIC language statement and `EOL` sequence. Line number or statement or both can be absent.  
  
Line number is a number in the range [1 .. 65535]  
  
Statement is one of the next BASIC1 language staetments: `DATA`, `DEF`, `DIM`, `ELSE`, `ELSEIF`, `ERASE`, `FOR`, `GOTO`, `GOSUB`, `IF`, `INPUT`, `LET`, `NEXT`, `ON`, `OPTION`, `PRINT`, `RANDOMIZE`, `READ`, `REM`, `RESTORE`, `RETURN`, `SET` and implicit assignment statement (with omitted `LET` keyword). Every statement should start from statement keyword except for the implicit assignment.  
  
**Examples of program lines:**  
`10 REM RANDOMIZE statement`  
`20 RANDOMIZE`  
  
`REM FOR statement with omitted line number`  
`FOR I = 0 TO 10`  
  
`REM Program line without statement`  
`100`  
  
`REM Implicit assignment statement`  
`50 A = A + 1`  
  
## Comments  
  
There are two types of comments supported by the interpreter: full-line comments and end-of-line comments. BASIC full-line comment starts from `REM` keyword so the interpreter ignores the rest of the line. Once `REM` keyword is a BASIC language statement it can be preceeded by line number. End-of-line comment starts from `'` character (apostrophe). It can be placed in any position of any program line and makes the interpreter ignoring the rest of the line.
  
**Examples of comments:**  
`10 REM This is a full-line comment`  
`REM Another full-line comment`  
`30 A = 20 'assign the value 20 to the variable A`  
  
## Constants  
  
Constant is an element of every BASIC program representing a number or text string. String constants are embraced in double quote characters. All double quote characters withing string constants have to be doubled for the interpreter to distinguish them from the embracing characters. Numeric constants can be written in normal or exponential form. Numeric constants allow type specifying character addition at the end. See **Data type specifiers** chapter for details.  
  
**Examples of constants:**  
`"this is a string constant"`  
`"string constant with "" character"`  
`0`  
`-1`  
`+10.1`  
  
## Identifiers  
  
Identifier is a program element representing function or variable. Identifier name must start from a Latin letter, can consist of Lating letters and digits, can end with a type specifier and must not be longer than 31 character. Type specifier character is mandatory for names of identifiers representing string variables or functions returing string values. String type specifier is `$` character. Numeric type specifiers are optional. Identifier names are case-insensitive so identifiers `var1`, `Var1` and `VAR1` all refer to the same function or variable.  
  
**Examples of identifiers:**  
`a` - can be a numeric variable name or a function returning numeric value  
`s$`, `s1$`, `text$` - string variables or functions names  
  
## Data types  
  
Data types supported by the interpreter:  
- `STRING` - used for storing textual data  
- `SINGLE` - single precision floating point (32-bit)  
- `INT` - 32-bit integer type  
  
Every constant, variable, function or function argument is processed according to its data type. Default numeric data type is `SINGLE`. String constants are enclosed in double quotes and string variables names, names of functions returning string values or names of function string arguments must end with `$` character. BASIC1 interpreter automatically converts operands to the common data type when evaluating every operator. The common data type is selected according to data types priority: `STRING` data type has the highest priority and `INT` - the lowest. When assigning values to variables they are converted to variable data type if possibly.  
  
**Examples of data types:**  
`s$ = "text"` - assign string constant to the `s$` string variable  
`s = "text"` - not correct, `s` cannot be a string variable name because of `$` type specifier absence  
`var = 0.5` - assign numeric value to the `var` numeric variable  
`var$ = 0.5` - numeric value will be converted to string before assignment  
`var$ = 0.5 + "text"` - numeric value will be converted to string before addition operator evaluation  
  
## Data type specifiers  
  
Data type specifiers can be used to define types of constants, variables, functions arguments and values returning with functions. Data type specifier has to be the last character of an identifier or constant. BASIC1 language supports the next data type specifiers:  
- `$` (dollar sign) - used to define string identifier  
- `!` (exclamation mark) - single precision floating point identifier or constant  
- `%` (percent) - integer identifier or constant  
  
String data type specifier cannot be used with constants.  Identifiers with the same names differ in data type specifiers only are different identifiers. The interpreter assigns default numeric data type (`SINGLE`) to identifiers or numeric constants without data type specifiers. `DIM` statement can declare variables of any numeric data type without data type specifiers in their names.  

**Examples of data type specifiers usage:**  
`a! = 10% + 0.5` - 10.5 floating point value is assigned to `a!` variable  
`a% = 10% + 0.5` - 11 integer value is assigned to `a%` variable (10.5 rounded to 11 because `a%` variable has integer data type specifier)  
`a = 10.0%` - an error will be reported because 10.0 is not a valid integer number  
`a! = 10 / 3` - 3.33333 value is assigned to `a!`  
`a% = 10 / 3` - 3 value is assigned to `a%` because of rounding  
`a! = 10% / 3` - 3.33333 value is assigned to `a!`  
`a! = 10% / 3%` - 3 value is assigned to `a!` because of integer division  
  
## Operators and expressions  
  
Operators are characters indicating arithmetic and other operations performed on constants and variables in expressions. Every operator requires one or two values (operands) to perform operation on. Operators that require one operand are called unary operators and operators with two operands - binary operators. An expression is a mathematical expression consisting of constants, variables, function calls and operators. Parentheses can be used in expressions to change operators evaluation order.  
  
### Unary operators  
  
- `+` - unary plus operator, does nothing, can be used with numeric operands only  
- `-` - unary minus operator, performs arithmetic negation, can be used with numeric operands only  
  
### Binary operators  
  
- `+` - arithmetic addition for numeric operands and string concatenation for strings  
- `-` - arithmetic subtraction, numeric operands only  
- `*` - arithmetic multiplication  
- `/` - arithmetic division  
- `^` - calculates power of a number (exponentiation)  
- `=` - assignment operator, can be used with numerics and strings  
- `>` - "greater than" comparison operator, can be used with numerics and strings  
- `<` - "less than" comparison operator, can be used with numerics and strings  
- `>=` - "greater than or equal" comparison operator, can be used with numerics and strings  
- `<=` - "less than or equal" comparison operator, can be used with numerics and strings  
- `=` - "equal" comparison operator, can be used with numerics and strings  
- `<>` - "not equal" comparison operator, can be used with numerics and strings  
  
**Examples of expressions:**  
`A = 10` 'the simplest expresssion assigning numeric value to `A` variable  
`A = 5 + 10 * 2` 'after the expression evaluation 25 numeric value is assigned to `A`  
`A = (5 + 10) * 2` '30 numeric value is assigned to `A`  
`IF A > 10 THEN GOTO 100` 'here `A > 10` is a logical expression  
`FOR I = 0 TO A + 5 STEP S - 1` 'three expressions here: `I = 0`, `A + 5` and `S - 1`  
`S$ = S$ + 10` 'the expression concatenates two strings: value of `S$` variable and string representation of 10 and assigns the result to the same `S$` variable  
  
## Operator precedence  
  
Operator precedence determines the order of operators evaluation in one expression. An operator with higher precedence will be evaluated before an operator with lower precedence. Operators evaluation order can be changed using parentheses.  
  
### Operator precedence order  
  
- unary `+` and unary `-` (the highest order of precedence)  
- `^`  
- `*` and `/`  
- `+` and `-`  
- `>`, `<`, `>=`, `<=`, `=` and `<>` comparison operators  
- `=` (assignment operator, the lowest order of precedence)  
  
**Examples:**  
`A = A + -A` 'operators evaluation order: negation (unary `-`), addition (`+`), assignment (`=`)  
`A = A + B * C` 'order: multiplication (`*`), addition (`+`), assignment (`=`)  
`A = (A + B) * C` 'order: addition (`+`), multiplication (`*`), assignment (`=`)  
`IF A + 1 > B * 2 THEN GOTO 100` 'order: multiplication (`*`), addition (`+`), greater than comparison (`>`)  
  
## Functions  
  
Function is a named block of code that can be reused multiple times calling it by the name in expressions. Usually functions take one or more arguments and return some value. A function call in expression consists of the function name and the function arguments list enclosed in parentheses. Arguments must be delimited from each other with commas. Some functions allows omitting arguments.  
  
**Examples of function calls:**  
`A = SIN(X)` - calling `SIN` function accepting one argument  
`A = PI` - calling `PI` function without arguments  
`I = INSTR(, S1$, S2$)` - calling `INSTR` function with the first argument omitted  
`I = SOMEFN()` - calling `SOMEFN` function with a single argument omitted  
  
There are two types of functions in BASIC1: built-in functions and user-defined functions. Built-in functions are provided by the language itself and can be used without any additional steps such as definition. User-defined functions have to be defined using special `DEF` statement before using them in expressions.  
  
### Built-in functions  
  
`ASC(<string>)` - returns integer code of the first character of a specified text string  
`LEN(<string>)` - returns number of characters in a string  
`CHR$(<numeric>)` - returns string consisting of a single character corresponding to integer code specified as function argument  
`IIF(<logical>, <numeric1>, <numeric2>)` - takes three arguments: a logical expression, and two numeric expressions; evaluates the logical expression and if the result of the expression is `TRUE` the function returns result of evaluation of the first numeric expression and result of the second numeric exprssion is returned otherwise  
`IIF$(<logical>, <string1>, <string2>)` - takes three arguments: a logical expression, and two string expressions, is similar to `IIF` function but works with string arguments and returns string value  
`STR$(<numeric>)` - converts numeric value to string  
`VAL(<string>)` - converts textual representation of a number to numeric value if possibly  
`ABS(<numeric>)` - returns the absolute value of a numeric value  
`INT(<numeric>)` - returns rounded integer value of a numeric argument value  
`RND`- returns a random numeric value between 0 and 1  
`SGN(<numeric>)` - returns a numeric value indicating the sign of a specified number (-1 if the input number is negative, 0 if it is equal to 0 and 1 if the value is positive)  
`ATN(<numeric>)` - returns the arctangent (in radians) of its argument  
`COS(<numeric>)` - returns the cosine of an angle specified in radians  
`EXP(<numeric>)` - returns `e` (the base of natural logorithms) raised to a specified power  
`LOG(<numeric>)` - returns the natural logorithm of a specified value  
`PI` - returns the value of `pi` constant  
`SIN(<numeric>)` - returns the sine of an angle specified in radians  
`SQR(<numeric>)` - returns the square root of the specified number  
`TAN(<numeric>)` - returns the tangent of an angle specified in radians  
`MID$(<string>, <numeric1>, [<numeric2>])` - returns the substring of a string specified with the first argument, one-based starting position is specified with the second argument and the third stands for substring length. The last parameter is optional and if it's absent the function returns all the characters to the right of the starting position  
`INSTR([<numeric>], <string1>, <string2>)` - returns one-based position of a string provided with the third argument in a string specified with the second argument. The first argument stands for a position to start search from (if it is omitted search starts from the beginning of the string). If the string is not found zero value is returned  
`LTRIM$(<string>)` - trims leading blank (space and TAB) characters  
`RTRIM$(<string>)` - trims trailing blank characters  
`LEFT$(<string>, <numeric>)` - returns the leftmost part of a string specified with the first argument, the substring length is specified with the second argument  
`RIGHT$(<string>, <numeric>)` - returns the rightmost part of a string specified with the first argument, the substring length is specified with the second argument  
`LSET$(<string>, <numeric>)` - returns the string specifed with the first argument left justified to a length provided with the second argument  
`RSET$(<string>, <numeric>)` - returns the string specifed with the first argument right justified to a length provided with the second argument  
`LCASE$(<string>)` - converts all string letters to lower case  
`RCASE$(<string>)` - converts all string letters to upper case  
  
**Examples:**  
`COS2PI = COS(2 * PI)` 'cosine of `2pi`  
`POS = INSTR(, "BASIC1", "BASIC")` 'look for "BASIC" in "BASIC1" string  
`S$ = LEFT("BASIC1", 5)` 'get first five characters of "BASIC1" string  
`S$ = MID$("BASIC1", 1, 5)` 'the same as previous  
`MIN = IIF(A > B, B, A)` 'get the minimum of two values  
  
## Statements  
  

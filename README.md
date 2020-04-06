# BASIC1  
  
*just one more BASIC interpreter*  
  
# Brief  
  
- supports classic BASIC dialect with various extensions  
- simple line-by-line interpreter  
- configurable language features/extensions  
- interpreter core written in C  
- embeddable: memory functions, input/output and caching detached from core  
- licensed under MIT license  
  
# Data types  
  
- STRING  
- SINGLE (32-bit floating point)  
- INT (32-bit integer)  
  
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
  
# More features  
  
- optional line numbers  
- optional LET statement  
- case-insensitive statement, variable and function names  
- one- and two-dimensional arrays  
- relational operators can be used with strings  
- automatic numeric to string conversion  
- functions: ABS, INT, RND, SGN, LEN, ASC, CHR$, STR$, VAL  
- more functions: IIF, IIF$, ATN, COS, EXP, LOG, PI, SIN, SQR, TAN  
- more functions: MID$, INSTR, LTRIM$, RTRIM$, LEFT$, RIGHT$, LSET$, RSET$, UCASE$, LCASE$  
  
# Building and usage  
  
## Building  
  
Use CMake 3.0 tools to build the project with MinGW-W64 or MSVC compilers. To build **b1i** simple command-line interpreter go to `./source/b1i` directory and run corresponding batch file depending on your compiler: `win_x86_mingw_rel.bat` for MinGW-W64, `win_x86_msvc10_rel.bat` for MSVC10, etc.  
  
**Important note:** the batch files mentioned above run `win_x86_mingw_env.bat` and `win_x86_msvc10_env.bat` files if they exist to set compiler-specific environment variables(PATH, INCLUDE, LIB, etc.). Create them if necessary before building the project.  
  
## Usage  
  
**b1i** executable file name of the interpreter for Windows is `b1i.exe`. After successful project building the file can be found in `./bin` directory. The command-line interpreter usage syntax: `b1i.exe [options] <filename>` where `<filename>` is relative or absolute path of a BASIC program file. Possible options: `-d` - print not only error code but textual description too, `-e` - enable input echo (redirects all the text receiving with INPUT statement to output stream), `-t` - print program execution time.  
Examples:  
`b1i.exe sample1.bsc`  
`b1i.exe -e sample2.bsc <input.txt`  
  
# Project directories structure  
  
- `./bin` - binary modules go here  
- `./build` - a directory for generated project files, objects and other build stuff  
- `./distr` - distributives (only zip archives are available at the moment)  
- `./docs` - documentation directory  
- `./samples` - some basic samples  
- `./source` - source directory  
- `./source/b1i` - source files of a simple command-line interpreter  
- `./source/common` - common source files  
- `./source/core` - interpreter core source files  
- `./source/ext` - a separate directory for environment-specific source files  
- `./source/setup` - setup source files (making zip archives only at the moment)  
- `./LICENSE` - MIT license text file  
- `./README.md` - this file  
  
# BASIC1  
  
*just one more BASIC interpreter*  
  
# Brief  
  
- supports classic BASIC dialect with various extensions  
- simple line-by-line interpreter  
- configurable language features/extensions  
- interpreter core written in C  
- embeddable: memory functions, input/output and caching detached from core  
- command-line interpreter built for Windows x86, Windows x64, Linux i386, Linux amd64, Linux armhf  
- optional Unicode support
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
- FOR ... TO ... \[STEP\]  
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
- WHILE ... WEND  
  
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
  
Use CMake 3.1 tools to build **b1i** or **b1iu** simple command-line interpreter. **b1iu** interpreter uses Unicode for characters representation. Use one of the next C/C++ toolchains: MinGW-W64 (x86 and x64) under Windows, MSVC 2010 (x86 and x64) under Windows, MSVC 2019 (x86 and x64) under Windows, gcc/g++ (x86, x64 and armhf) under Linux. Other compilers/toolchains are probably compatible too.  
  
To build the interpreter under Windows go to `./source/b1i` directory and run corresponding batch file depending on your compiler and target platform: `b1i_win_x86_mingw_rel.bat` and `b1i_win_x64_mingw_rel.bat` for MinGW-W64, `b1i_win_x86_msvc10_rel.bat` and `b1i_win_x64_msvc10_rel.bat` for MSVC10, etc. Under Linux the procedure is the same but shell script names are `b1i_lnx_x86_gcc_rel.sh`, `b1i_lnx_x64_gcc_rel.sh` and `b1i_lnx_armhf_gcc_rel.sh`. Use scripts with `b1iu_` prefix to build Unicode version of the interpreter.  
  
**Important note:** The batch files mentioned above run corresponding `<platform_toolchain_name>_env.bat` files if they exist to set compiler-specific environment variables(PATH, INCLUDE, LIB, etc.). So the file names are: `win_x86_mingw_env.bat`, `win_x64_mingw_env.bat`, `win_x86_msvc10_env.bat`, etc. Create them if necessary before building the project.  
  
## Usage  
  
Executable file names of the interpreter are `b1i.exe` and `b1iu.exe` for Windows and `b1i` and `b1iu` for Linux. After successful project building the file can be found in `./bin` directory. The command-line interpreter usage syntax: `b1i [options] <filename>` where `<filename>` is relative or absolute path of a BASIC program file.  
  
Possible options:  
- `-a` - print variables values on error (for debugging)  
- `-d` - print not only error code but textual description too  
- `-e` - enable input echo (redirects all the text receiving with INPUT statement to output stream)  
- `-l <local_name>` - set `<local_name>` locale for the program (the locale should be available on your system)  
- `-t` - print program execution time  
- `-v` - just print interpreter version  
  
Examples:  
`./b1i sample1.bsc`  
`./b1i -e sample2.bsc <input.txt`  
`./b1iu 12.bsc`  
`./b1iu -a failed.bsc`  
  
# Project directories structure  
  
- `./bin` - binary modules go here  
- `./build` - a directory for generated project files, objects and other build stuff  
- `./distr` - distributives (zip archives for Windows, deb packages for Linux)  
- `./docs` - documentation directory  
- `./samples` - some basic samples  
- `./source` - source directory  
- `./source/b1i` - source files of a simple command-line interpreter  
- `./source/common` - common source files  
- `./source/core` - interpreter core source files  
- `./source/ext` - a separate directory for environment-specific source files  
- `./source/setup` - setup source files (making zip archives for Windows and deb packages for Linux)  
- `./LICENSE` - MIT license text file  
- `./README.md` - this file  
  
# More documents  
  
[BASIC1 language reference](./docs/reference.md)  
[BASIC1 interpreter limitations](./docs/limits.md)  
[BASIC1 interpreter core embedding](./docs/embedding.md)  
[Change log](./docs/changelog)  
  
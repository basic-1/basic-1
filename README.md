# BASIC1  
  
*just one more BASIC interpreter*  
  
![b1ig program on Windows](https://github.com/basic-1/basic-1/common/docs/images/b1win.png "b1ig program on Windows")  
  
# Brief  
  
- supports classic BASIC dialect with various extensions  
- simple line-by-line interpreter  
- configurable language features/extensions  
- interpreter core written in C  
- embeddable: memory functions, input/output and caching detached from core  
- command-line interpreter built for Windows x86, Windows x64, Linux i386, Linux amd64, Linux armhf  
- interpreter with graphical IDE and debugger built for various platforms (uses wxWidgets GUI library)  
- optional Unicode support  
- licensed under MIT license  
  
# Data types  
  
- STRING  
- SINGLE (32-bit floating-point)  
- DOUBLE (64-bit floating-point)  
- INT (32-bit integer)  
- INT16 (16-bit integer)  
- WORD (16-bit unsigned integer)  
- BYTE (8-bit unsigned integer)  
  
# Statements  
  
- BREAK  
- CONTINUE  
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
- STOP  
- WHILE ... WEND  
  
# More features  
  
- optional line numbers  
- optional LET statement  
- case-insensitive statement, variable and function names  
- one-, two- and three-dimensional arrays  
- relational operators can be used with strings  
- automatic numeric to string conversion  
- functions: ABS, INT, RND, SGN, LEN, ASC, CHR$, STR$, VAL  
- more functions: IIF, IIF$, ATN, COS, EXP, LOG, PI, SIN, SQR, TAN  
- more functions: MID$, INSTR, LTRIM$, RTRIM$, LEFT$, RIGHT$, LSET$, RSET$, UCASE$, LCASE$  
  
# Building and usage  
  
## Building  
  
Use CMake 3.1 tools to build **b1i** or **b1iu** simple command-line interpreter or **b1ig** interpreter with graphical IDE. **b1iu** interpreter uses Unicode for characters representation. wxWidgets GUI library is needed to build **b1ig** interpreter with graphical IDE. Use one of the next C/C++ toolchains: MinGW-W64 (x86 and x64) under Windows, MSVC 2010 (x86 and x64) under Windows, MSVC 2019 (x86 and x64) under Windows, gcc/g++ (x86, x64 and armhf) under Linux. Other compilers/toolchains are probably compatible too.  
  
First get the interpreter source files copy to your local computer from [GitHub](https://github.com/basic-1/basic-1)  
  
Commands to get the source files:  
`git clone https://github.com/basic-1/basic-1`  
`cd basic-1`  
`git submodule update --init`  
  
To build the interpreter under Windows go to `./b1i/build` or `./b1ig/build` directory and run corresponding batch file depending on your compiler and target platform: `b1i_win_x86_mingw_rel.bat` and `b1i_win_x64_mingw_rel.bat` for MinGW-W64, `b1i_win_x86_msvc10_rel.bat` and `b1i_win_x64_msvc10_rel.bat` for MSVC10, etc. Under Linux the procedure is the same but shell script names are `b1i_lnx_x86_gcc_rel.sh`, `b1i_lnx_x64_gcc_rel.sh` and `b1i_lnx_armhf_gcc_rel.sh`. Use scripts with `b1iu_` prefix to build Unicode version of the interpreter.  
  
**Important note:** The batch files mentioned above run corresponding `<platform_toolchain_name>_env` scripts from `./env` directory if they exist to set compiler-specific environment variables(PATH, INCLUDE, LIB, etc.). So the file names are: `win_x86_mingw_env.bat`, `win_x64_mingw_env.bat`, `win_x86_msvc10_env.bat`, `lnx_x64_gcc_env.sh`, etc. Create them if necessary before building the project. CMake build and setup scripts for **b1ig** interpreter use `wx-config` utility to locate wxWidgets libraries and compiler settings. Specify the utility executable file name (with path if needed) by setting `WX_CONFIG` environment variable before running **b1ig** build or setup scripts (the mentioned above `<platform_toolchain_name>_env` scripts can be used for this purpose, the samples can be found in `./env` directory: they have `sample_` prefix).  
  
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
  
- `./b1core` - a directory of [BASIC1 interpreter core](https://github.com/basic-1/b1core) used as submodule  
- `./b1i` - source files of a simple command-line interpreter  
- `./b1i/build` - build scripts of **b1i** and **b1iu** command-line interpreters  
- `./b1i/source` - source files of **b1i** and **b1iu** command-line interpreters  
- `./b1ig/build` - build scripts of **b1ig** interpreters with GUI  
- `./b1ig/source` - build scripts of **b1ig** interpreters with GUI  
- `./bin` - binary modules go here  
- `./build` - a directory for generated project files, objects and other build stuff  
- `./common` - common source files and scripts  
- `./distr` - distributives (zip archives for Windows, deb packages for Linux)  
- `./env` - setting up environment scripts (used when compiling interpreter and building setup)  
- `./setup` - setup source files (making zip archives for Windows and deb packages for Linux)  
- `./LICENSE` - MIT license text file  
- `./README.md` - this file  
  
# More documents  
  
[BASIC1 language reference](https://github.com/basic-1/b1core/docs/reference.md)  
[BASIC1 interpreter limitations](https://github.com/basic-1/b1core/docs/limits.md)  
[BASIC1 interpreter core embedding](https://github.com/basic-1/b1core/docs/embedding.md)  
[Change log](https://github.com/basic-1/basic-1/common/docs/changelog)  
  
![b1ig program on Ubuntu 18.04](https://github.com/basic-1/basic-1/common/docs/images/b1ub1804.png "b1ig program on Ubuntu 18.04")  
![b1ig program on Kubuntu 20.10](https://github.com/basic-1/basic-1/common/docs/images/b1kub2010.png "b1ig program on Kubuntu 20.10")  
  
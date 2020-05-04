# Embedding BASIC1 interpreter  
  
## Preamble  
  
BASIC1 interpreter core can be embedded into existing application or an alternative command-line or GUI shell can be written for the interpreter instead of the existing **b1i** command-line one. The interpreter core is written in C language and does not use standard memory allocation, input/output or other environment-specific functions. The interpreter hosting application has to provide such functions to the core as well as some other functions allowing navigating through program lines, caching line numbers and expressions data, etc.  
  
## Typical embedding scenario  
  
1) add core source files to hosting application project;  
2) implement all external functions necessary for the core to act;  
3) implement BASIC program loading mechanism;  
4) call the interpreter initialization and run functions.  
  
## External functions needed for the interpreter core  
  
The functions have `b1_ex_` prefix and must be implemented for external linkage with C language (use `extern` keyword if they are written in C or `extern "C"` if they are written in C++).  
  
### Memory management functions  
  
`extern B1_T_ERROR b1_ex_mem_init();`  
`b1_ex_mem_init` function is called on interpreter initialization or reset (`b1_int_reset` function does this).  
  
`extern B1_T_ERROR b1_ex_mem_alloc(B1_T_MEMOFFSET size, B1_T_MEM_BLOCK_DESC *mem_desc, void **data);`  
`b1_ex_mem_alloc` function has to allocate `size` bytes of memory and return the memory block descriptor in `mem_desc` parameter and pointer to already allocated memory in `data` parameter. Any memory block is accessible for reading and writing after `b1_ex_mem_alloc` and `b1_ex_mem_access` calls and `b1_ex_mem_release` call makes the pointer returned in `data` parameter invalid. To get a new valid pointer to the same memory block the interpreter calls `b1_ex_mem_access` again. The interpreter does not use two or more memory block pointers at the same time.  
  
`extern B1_T_ERROR b1_ex_mem_access(const B1_T_MEM_BLOCK_DESC mem_desc, void **data);`  
`b1_ex_mem_access` function has to make the memory block identified with `mem_desc` parameter accessible and return the pointer to the block in `data` parameter.  
  
`extern B1_T_ERROR b1_ex_mem_release(const B1_T_MEM_BLOCK_DESC mem_desc);`  
`b1_ex_mem_release` function is called by the interpreter when it finishes working with the memory block and memory manager can move the changes made by the interpreter to some another memory location if needed. The function must not free the memory block.  
  
`extern B1_T_ERROR b1_ex_mem_free(const B1_T_MEM_BLOCK_DESC mem_desc);`  
`b1_ex_mem_free` function must free the memory block identified with `mem_desc` parameter.  
  
Possible return codes for the functions: `B1_RES_OK` (success), `B1_RES_ENOMEM` (not enough memory).  
  
The simplest implementation of the functions can be found in `./source/ext/exmem.cpp` file (using standard C++ `new` and `delete` operators).  
  
### Input/output functions  
  
`extern B1_T_ERROR b1_ex_io_print_char(B1_T_CHAR c);`  
The function must send a single character to the application textual output device (display, file, memory, etc.).  
  
`extern B1_T_ERROR b1_ex_io_print_newline();`  
The function has to write a new line sequence to the output device (moving printing caret on new line).  
  
`extern B1_T_ERROR b1_ex_io_print_margin_newline();`  
The function is called when the interpreter reaches output device right margin so it should either send new line sequence to the device or do nothing (depending on the device type).  
  
`extern B1_T_ERROR b1_ex_io_input_char(B1_T_CHAR *c);`  
`b1_ex_io_input_char` function has to read the next character from the hosting application textual input device.  
  
Possible return codes for the functions: `B1_RES_OK` (success), `B1_RES_EEOF` (no more data, can be returned by `b1_ex_io_input_char`), `B1_RES_EENVFAT` (fatal I/O error).  
  
The simplest functions implementation: `./source/ext/exio.c` (sdandard C input/output).  
  
to be continued...
#[[
 BASIC1 interpreter
 Copyright (c) 2021 Nikolay Pletnev
 MIT license

 CMakeLists.txt: CMake file for BASIC1 interpreter with wxWidgets-based IDE
]]


set(CMAKE_CXX_STANDARD 11)

# set project (target) name, B1I_TARGET variable should be set before running cmake
set(B1_PROJECT_NAME $ENV{B1I_TARGET})

if(UNIX)
 set(B1_WX_CONFIG_PLATFORM_SPECIFIC /bin/sh)
else()
 set(B1_ADD_EXEC_PLATFORM_SPECIFIC WIN32)
endif()

execute_process(COMMAND ${B1_WX_CONFIG_PLATFORM_SPECIFIC} $ENV{B1_WX_CONFIG} --cppflags OUTPUT_VARIABLE B1_WX_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${B1_WX_CXX_FLAGS} B1_WX_CXX_FLAGS)
execute_process(COMMAND ${B1_WX_CONFIG_PLATFORM_SPECIFIC} $ENV{B1_WX_CONFIG} --cflags OUTPUT_VARIABLE B1_WX_C_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${B1_WX_C_FLAGS} B1_WX_C_FLAGS)
execute_process(COMMAND ${B1_WX_CONFIG_PLATFORM_SPECIFIC} $ENV{B1_WX_CONFIG} --libs core,base,aui,stc OUTPUT_VARIABLE B1_WX_LINK_OPTIONS OUTPUT_STRIP_TRAILING_WHITESPACE)
string(STRIP ${B1_WX_LINK_OPTIONS} B1_WX_LINK_OPTIONS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${B1_WX_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${B1_WX_C_FLAGS}")
link_libraries(${B1_WX_LINK_OPTIONS})

project(${B1_PROJECT_NAME} LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$\{ORIGIN\}/../lib/b1i")

set(B1_CORE_SRC_DIR ../core)
set(B1_EXT_SRC_DIR ../ext)

include_directories($ENV{WXWIN}/include)
include_directories(${B1_CORE_SRC_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${B1_PROJECT_NAME} ${B1_ADD_EXEC_PLATFORM_SPECIFIC} B1igApp.cpp
	B1igFrame.cpp
	B1Interpreter.cpp
	errors.cpp
	exprg.cpp
	${B1_CORE_SRC_DIR}/b1.c
	${B1_CORE_SRC_DIR}/b1types.c
	${B1_CORE_SRC_DIR}/b1id.c
	${B1_CORE_SRC_DIR}/b1tok.c
	${B1_CORE_SRC_DIR}/b1rpn.c
	${B1_CORE_SRC_DIR}/b1var.c
	${B1_CORE_SRC_DIR}/b1fn.c
	${B1_CORE_SRC_DIR}/b1eval.c
	${B1_CORE_SRC_DIR}/b1int.c
	${B1_CORE_SRC_DIR}/b1dbg.c
	${B1_EXT_SRC_DIR}/exmem.cpp
	${B1_EXT_SRC_DIR}/exvar.cpp
	${B1_EXT_SRC_DIR}/exufn.cpp
	${B1_EXT_SRC_DIR}/exrnd.c
	${B1_EXT_SRC_DIR}/ext.cpp
        resource.rc)

# link with math. libary
if(UNIX)
 target_link_libraries(${B1_PROJECT_NAME} m)
endif()

#[[
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 CMakeLists.txt: CMake file for command line interpreter
]]


if(UNIX)
 set(CMAKE_CXX_STANDARD 11)
endif()

# set project (target) name, B1I_TARGET variable should be set before running cmake
set(B1_PROJECT_NAME $ENV{B1I_TARGET})

project(${B1_PROJECT_NAME} LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.1)

set(B1_CORE_SRC_DIR ../core)
set(B1_EXT_SRC_DIR ../ext)

add_definitions(-DB1_PROJECT_NAME="${B1_PROJECT_NAME}")

# enable B1_FEATURE_UNICODE_UCS2 feature if the target is b1iu
string(LENGTH "$ENV{B1I_TARGET}" STR_LEN)
math(EXPR STR_LEN "${STR_LEN} - 1")
string(SUBSTRING "$ENV{B1I_TARGET}" ${STR_LEN} -1 LAST_CHAR)
if("${LAST_CHAR}" STREQUAL "u")
 add_definitions(-DB1_FEATURE_UNICODE_UCS2)
endif()

include_directories(${B1_CORE_SRC_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${B1_PROJECT_NAME} b1i.c
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
	${B1_EXT_SRC_DIR}/exio.c
	${B1_EXT_SRC_DIR}/exprg.cpp
	${B1_EXT_SRC_DIR}/exrnd.c
	${B1_EXT_SRC_DIR}/ext.cpp)

# link with math. libary
if(UNIX)
 target_link_libraries(${B1_PROJECT_NAME} m)
endif()

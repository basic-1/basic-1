#[[
 BASIC1 interpreter
 Copyright (c) 2020 Nikolay Pletnev
 MIT license

 CMakeLists.txt: CMake file for command line interpreter
]]


# set project (target) name
set(B1_PROJECT_NAME b1i)

project(${B1_PROJECT_NAME} LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.0)

set(B1_CORE_SRC_DIR ../core)
set(B1_EXT_SRC_DIR ../ext)

add_definitions(-DB1_PROJECT_NAME="${B1_PROJECT_NAME}")

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
	${B1_EXT_SRC_DIR}/exmem.cpp
	${B1_EXT_SRC_DIR}/exvar.cpp
	${B1_EXT_SRC_DIR}/exio.c
	${B1_EXT_SRC_DIR}/exprg.cpp
	${B1_EXT_SRC_DIR}/exrnd.c)

# link with math. libary
if(UNIX)
 target_link_libraries(${B1_PROJECT_NAME} m)
endif()

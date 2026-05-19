cmake_minimum_required(VERSION 3.5)

# Minimal Linux toolchain helper: prefer gcc and set sane flags
if(NOT CMAKE_C_COMPILER)
	find_program(GCC_EXECUTABLE NAMES gcc cc)
	if(GCC_EXECUTABLE)
		set(CMAKE_C_COMPILER ${GCC_EXECUTABLE} CACHE STRING "C compiler" FORCE)
	endif()
endif()

set(_pearl_common_cflags "-std=c11 -O2 -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_pearl_common_cflags}" CACHE STRING "C flags" FORCE)

cmake_minimum_required(VERSION 3.5)

# MinGW toolchain hint: try common MinGW compilers, fall back to gcc if present
if(NOT CMAKE_C_COMPILER)
	find_program(MINGW_GCC_EXECUTABLE NAMES x86_64-w64-mingw32-gcc i686-w64-mingw32-gcc gcc)
	if(MINGW_GCC_EXECUTABLE)
		set(CMAKE_C_COMPILER ${MINGW_GCC_EXECUTABLE} CACHE STRING "C compiler" FORCE)
	endif()
endif()

set(_pearl_common_cflags "-std=c11 -O2 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_pearl_common_cflags}" CACHE STRING "C flags" FORCE)

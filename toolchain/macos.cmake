cmake_minimum_required(VERSION 3.5)

# macOS toolchain: prefer system clang, set conservative C flags
set(CMAKE_SYSTEM_NAME Darwin)
if(NOT CMAKE_C_COMPILER)
  find_program(CLANG_EXECUTABLE NAMES clang clang-14 clang-13)
  if(CLANG_EXECUTABLE)
    set(CMAKE_C_COMPILER ${CLANG_EXECUTABLE} CACHE STRING "C compiler" FORCE)
  endif()
endif()

set(_pearl_common_cflags "-std=c11 -O2 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_pearl_common_cflags}" CACHE STRING "C flags" FORCE)

# Provide an easy variable to override for cross-compilation users
if(NOT DEFINED PEARLCD_ENABLE_SANITIZERS)
  set(PEARLCD_ENABLE_SANITIZERS OFF CACHE BOOL "Enable ASAN/UBSAN when building locally")
endif()
if(PEARLCD_ENABLE_SANITIZERS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer" CACHE STRING "C flags" FORCE)
endif()

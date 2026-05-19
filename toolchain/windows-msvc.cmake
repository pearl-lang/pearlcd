cmake_minimum_required(VERSION 3.5)

# MSVC toolchain hints. Do not force MSVC — let the generator select the compiler.
# Provide some default flags and defines that make builds quieter on Windows.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D_CRT_SECURE_NO_WARNINGS /Ox" CACHE STRING "C flags" FORCE)

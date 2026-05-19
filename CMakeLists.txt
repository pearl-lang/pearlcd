cmake_minimum_required(VERSION 3.20)

project(pearlcd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(PEARLCD_PARSER
	${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/parser.y
	${CMAKE_CURRENT_BINARY_DIR}/parser.tab.c
	DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.h
)

flex_target(PEARLCD_LEXER
	${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c
)

add_flex_bison_dependency(PEARLCD_LEXER PEARLCD_PARSER)

add_executable(pearlcd
	src/frontend/ast.c
	src/frontend/semantic.c
	src/main.c
	src/backend/ir.c
	src/backend/codegen_c.c
	${BISON_PEARLCD_PARSER_OUTPUTS}
	${FLEX_PEARLCD_LEXER_OUTPUTS}
)

target_include_directories(pearlcd PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}
)


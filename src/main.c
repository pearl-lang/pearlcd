#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "ir.h"
#include "codegen_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin;
int yylex(void);

static void print_usage(const char *program_name) {
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  %s --lex [file]\n", program_name);
	fprintf(stderr, "  %s --parse [file]\n", program_name);
	fprintf(stderr, "  %s --check [file]\n", program_name);
	fprintf(stderr, "  %s --lower [file]\n", program_name);
	fprintf(stderr, "  %s [file]\n", program_name);
	fprintf(stderr, "\n");
	fprintf(stderr, "If no file is given, input is read from standard input.\n");
}

int pearl_run_lexer(FILE *input, const char *source_name) {
	yyin = input;

	if (source_name != NULL) {
		printf("== lexing %s ==\n", source_name);
	}

	int token; while ((token = yylex()) != 0); return 0;
}

static int run_lex_mode(const char *path) {
	FILE *input = stdin;

	if (path != NULL && strcmp(path, "-") != 0) {
		input = fopen(path, "rb");

		if (input == NULL) {
			perror(path);
			return 1;
		}
	}

	pearl_lexer_set_trace(1);
	pearl_run_lexer(input, path == NULL ? "<stdin>" : path);
	pearl_lexer_set_trace(0);

	if (input != stdin) {
		fclose(input);
	}

	return 0;
}

static int run_parse_mode(const char *path) {
	FILE *input = stdin;
	PearlAstNode *ast = NULL;

	if (path != NULL && strcmp(path, "-") != 0) {
		input = fopen(path, "rb");

		if (input == NULL) {
			perror(path);
			return 1;
		}
	}

	if (pearl_parse_file(input, path == NULL ? "<stdin>" : path, &ast) != 0) {
		if (input != stdin) {
			fclose(input);
		}

		return 1;
	}

	pearl_ast_print(ast, 0);
	pearl_ast_free(ast);

	if (input != stdin) {
		fclose(input);
	}

	return 0;
}

static int run_check_mode(const char *path) {
	FILE *input = stdin;
	PearlAstNode *ast = NULL;
	int result = 0;

	if (path != NULL && strcmp(path, "-") != 0) {
		input = fopen(path, "rb");

		if (input == NULL) {
			perror(path);
			return 1;
		}
	}

	result = pearl_parse_file(input, path == NULL ? "<stdin>" : path, &ast);

	if (result == 0) {
		result = pearl_check_ast(stderr, path == NULL ? "<stdin>" : path, ast);
	}

	pearl_ast_free(ast);

	if (input != stdin) {
		fclose(input);
	}

	return result;
}

static int run_lower_mode(const char *path) {
	FILE *input = stdin;
	PearlAstNode *ast = NULL;
	int result = 0;

	if (path != NULL && strcmp(path, "-") != 0) {
		input = fopen(path, "rb");

		if (input == NULL) {
			perror(path);
			return 1;
		}
	}

	result = pearl_parse_file(input, path == NULL ? "<stdin>" : path, &ast);

	if (result == 0) {
		result = pearl_lower_ast(ast, stdout);
	}

	pearl_ast_free(ast);

	if (input != stdin) {
		fclose(input);
	}

	return result;
}

static int run_codegen_mode(const char *path) {
	FILE *input = stdin;
	PearlAstNode *ast = NULL;
	int result = 0;

	if (path != NULL && strcmp(path, "-") != 0) {
		input = fopen(path, "rb");

		if (input == NULL) {
			perror(path);
			return 1;
		}
	}

	result = pearl_parse_file(input, path == NULL ? "<stdin>" : path, &ast);

	if (result == 0) {
		result = pearl_codegen_c(ast, stdout);
	}

	pearl_ast_free(ast);

	if (input != stdin) {
		fclose(input);
	}

	return result;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		print_usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		print_usage(argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--lex") == 0) {
		if (argc > 3) {
			fprintf(stderr, "Too many arguments.\n");
			print_usage(argv[0]);
			return 1;
		}

		return run_lex_mode(argc == 3 ? argv[2] : NULL);
	}

	if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--parse") == 0) {
		if (argc > 3) {
			fprintf(stderr, "Too many arguments.\n");
			print_usage(argv[0]);
			return 1;
		}

		return run_parse_mode(argc == 3 ? argv[2] : NULL);
	}

	if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--check") == 0) {
		if (argc > 3) {
			fprintf(stderr, "Too many arguments.\n");
			print_usage(argv[0]);
			return 1;
		}

		return run_check_mode(argc == 3 ? argv[2] : NULL);
	}

	if (strcmp(argv[1], "-L") == 0 || strcmp(argv[1], "--lower") == 0) {
		if (argc > 3) {
			fprintf(stderr, "Too many arguments.\n");
			print_usage(argv[0]);
			return 1;
		}

		return run_lower_mode(argc == 3 ? argv[2] : NULL);
	}

	if (strcmp(argv[1], "-g") == 0 || strcmp(argv[1], "--codegen") == 0) {
		if (argc > 3) {
			fprintf(stderr, "Too many arguments.\n");
			print_usage(argv[0]);
			return 1;
		}

		return run_codegen_mode(argc == 3 ? argv[2] : NULL);
	}

	if (argv[1][0] == '-') {
		fprintf(stderr, "Unknown option: %s\n", argv[1]);
		print_usage(argv[0]);
		return 1;
	}

	if (argc > 2) {
		fprintf(stderr, "Too many arguments.\n");
		print_usage(argv[0]);
		return 1;
	}

	return run_lex_mode(argv[1]);
}

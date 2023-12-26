#include <stdio.h>

#include "token.h"
#include "lexer.h"

#include "ast.h"
#include "parser.h"

#include "macros.h"

int main(int argc, const char* argv[]) {
	UNUSED_PARAMETER(argc);
	UNUSED_PARAMETER(argv);

	FILE* input = fopen("input.txt", "r");
	if(input != NULL) {
		FunctionDeclaration* function = parse_function_declaration(input);

		printf("%p\n", (void*)function);

		print_function_declaration(stdout, function, 0);

		fclose(input);
	}

	return 0;
}


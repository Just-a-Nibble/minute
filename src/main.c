#include <stdio.h>

#include "ast.h"
#include "parser.h"

#include "macros.h"

int main(int argc, const char* argv[]) {
	UNUSED_PARAMETER(argc);
	UNUSED_PARAMETER(argv);

	FILE* input = fopen("input.txt", "r");
	if(input != NULL) {
		Program* program = parse_program(input);
		printf("%p\n", (void*)program);

		print_program(stdout, program, 0);

		fclose(input);
	}

	return 0;
}


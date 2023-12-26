#include <stdio.h>

#include "value.h"
#include "ast.h"
#include "parser.h"
#include "interpreter.h"

#include "macros.h"

int main(int argc, const char* argv[]) {
	UNUSED_PARAMETER(argc);
	UNUSED_PARAMETER(argv);

	FILE* input = fopen("input.txt", "r");
	if(input != NULL) {
		Program* program = parse_program(input);

		Value value = eval_function_call_from_name("main", program, NULL, 0);
		switch(value.type) {
			case VAL_INTEGER:
				printf("%i\n", value.data.integer);
				break;
			case VAL_FLOATING:
				printf("%f\n", value.data.floating);
				break;
			case VAL_STRING:
				printf("%s\n", value.data.string);
				break;
		}

		fclose(input);
	}

	return 0;
}


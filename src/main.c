#include <stdio.h>

#include "lexer.h"

#include "macros.h"

int main(int argc, const char* argv[]) {
	UNUSED_PARAMETER(argc);
	UNUSED_PARAMETER(argv);

	FILE* input = fopen("input.txt", "r");
	if(input != NULL) {
		Token token;

		do {
			token = next_token(input);

			printf("%s\n", token_type_to_string(token.type));

			destroy_token(&token);
		} while (token.type != TOK_EOF);

		fclose(input);
	}

	return 0;
}


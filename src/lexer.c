#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>

#include "token.h"

#include "macros.h"

static const struct {
	const char* string;
	Keyword keyword;
} keywords[] = {
	{ "defunc", KEY_DEFUNC },
	{ "var", KEY_VAR },
	{ "if", KEY_IF }
};

static const struct {
	int character;
	Punctuator punctuator;
} punctuators[] = {
	{ '(', PUNC_OPEN_PAREN },
	{ ')', PUNC_CLOSE_PAREN }
};

static Token _lex_keyword_or_identifier(FILE* file) {
	char* string = NULL;
	size_t string_length = 0;

	for(;;) {
		const int ch = fgetc(file);

		if(!isalnum(ch) && ch != '_') {
			ungetc(ch, file);
			break;
		}

		string = realloc(string, string_length + 2);
		if(string == NULL) {
			fputs("[ERROR] Out of Memory.\n", stderr);
			exit(1);
		}

		string[string_length++] = (char)ch;
	}

	assert(string != NULL);
	string[string_length] = '\0';

	for(size_t i = 0; i < ARRAY_LENGTH(keywords); i++) {
		if(strcmp(string, keywords[i].string) == 0) {
			free(string);

			return (Token) {
				.type = TOK_KEYWORD,
				.data.keyword = keywords[i].keyword
			};
		}
	}

	return (Token) {
		.type = TOK_IDENTIFIER,
		.data.identifier = string
	};
}

static Token _lex_numeric_literal(FILE* file) {
	int integer_literal = 0;

	/* TODO: Support hexidecimal numbers */
	/* TODO: Support binary numbers */
	/* TODO: Support floating point */
	/* TODO: Support octal numbers */

	for(;;) {
		const int ch = fgetc(file);

		if(!isdigit(ch)) {
			ungetc(ch, file);
			break;
		}

		integer_literal = (integer_literal * 10) + (ch - '0');
	}

	return (Token) {
		.type = TOK_INTEGER_LITERAL,

		.data.integer_literal = integer_literal
	};
}

static Token _lex_string_literal(FILE* file) {
	char* string = NULL;
	size_t string_length = 0;

	/* TODO: Support character escaping */

	for(;;) {
		const int ch = fgetc(file);

		if(ch == '\"') {
			break;
		}

		if(!isgraph(ch) && ch != '_') {
			if(ch == EOF) {
				fputs("[ERROR] Unexpected End of File When Parsing String Literal.\n", stderr);
			} else if(ch == '\n') {
				fputs("[ERROR] Unexpected Newline When Parsing String Literal.\n", stderr);
			} else if(ch == '\r') {
				fputs("[ERROR] Unexpected Carriage Return When Parsing String Literal.\n", stderr);
			} else if(ch == '\r') {
				fputs("[ERROR] Unexpected Tab When Parsing String Literal.\n", stderr);
			} else {
				fputs("[ERROR] Unexpected Character When Parsing String Literal.\n", stderr);
			}

			exit(1);
		}

		string = realloc(string, string_length + 2);
		if(string == NULL) {
			fputs("[ERROR] Out of Memory.\n", stderr);
			exit(1);
		}

		string[string_length++] = (char)ch;
	}

	assert(string != NULL);
	string[string_length] = '\0';

	return (Token) {
		.type = TOK_STRING_LITERAL,
		.data.string_literal = string
	};
}

Token next_token(FILE* file) {
	assert(file != NULL);

	for(;;) {
		const int ch = fgetc(file);

		if(feof(file)) {
			return (Token) { .type = TOK_EOF };
		}

		if(isspace(ch)) {
			continue;
		}

		if(isalpha(ch) || ch == '_') {
			ungetc(ch, file);

			return _lex_keyword_or_identifier(file);
		}

		if(isdigit(ch)) {
			ungetc(ch, file);

			return _lex_numeric_literal(file);
		}

		{
			for(size_t i = 0; i < ARRAY_LENGTH(punctuators); i++) {
				if(punctuators[i].character == ch) {
					return (Token) {
						.type = TOK_PUNCTUATOR,
						.data.punctuator = punctuators[i].punctuator
					};
				}
			}
		}

		if(ch == '\"') {
			/* Not backtracking because we don't need it */

			return _lex_string_literal(file);
		}

		fprintf(stderr, "[ERROR] Unexpected Character '%c' (%i).\n",
			(char)ch, ch
		);
		exit(1);
	}
}

Token expect_token(FILE* file, TokenType type) {
	Token token = next_token(file);

	if(token.type != type) {
		fprintf(stderr, "[ERROR] Unexpected Token; Expected '%s', But Got '%s'.\n",
			token_type_to_string(type),
			token_type_to_string(token.type)
		);
		exit(1);
	}

	return token;
}

Token expect_keyword(FILE* file, Keyword keyword) {
	Token token = expect_token(file, TOK_KEYWORD);

	if(token.data.keyword != keyword) {
		fprintf(stderr, "[ERROR] Unexpected Keyword; Expected '%s', But Got '%s'.\n",
			keyword_to_string(keyword),
			keyword_to_string(token.data.keyword)
		);
		exit(1);
	}

	return token;
}

Token expect_punctuator(FILE* file, Punctuator punctuator) {
	Token token = expect_token(file, TOK_PUNCTUATOR);

	if(token.data.punctuator != punctuator) {
		fprintf(stderr, "[ERROR] Unexpected Punctuator; Expected '%s', But Got '%s'.\n",
			punctuator_to_string(punctuator),
			punctuator_to_string(token.data.punctuator)
		);
		exit(1);
	}

	return token;
}


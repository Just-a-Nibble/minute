#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>

#include "macros.h"

void destroy_token(Token* token) {
	assert(token != NULL);

	switch(token->type) {
		case TOK_IDENTIFIER:
			free(token->data.identifier);
#ifndef NDEBUG
			token->data.identifier = NULL;
#endif
			break;
		case TOK_KEYWORD: break;
		case TOK_PUNCTUATOR: break;
		case TOK_INTEGER_LITERAL: break;
		case TOK_FLOAT_LITERAL: break;
		case TOK_STRING_LITERAL:
			free(token->data.string_literal);
#ifndef NDEBUG
			token->data.string_literal = NULL;
#endif
			break;
		case TOK_EOF: break;
	}
}

const char* token_type_to_string(TokenType type) {
	switch(type) {
		case TOK_IDENTIFIER:
			return "Identifier";
		case TOK_KEYWORD:
			return "Keyword";
		case TOK_PUNCTUATOR:
			return "Punctuator";
		case TOK_INTEGER_LITERAL:
			return "Integer Literal";
		case TOK_FLOAT_LITERAL:
			return "Float Literal";
		case TOK_STRING_LITERAL:
			return "String Literal";
		case TOK_EOF:
			return "End of File";
	}

	UNREACHABLE();
}

const char* keyword_to_string(Keyword keyword) {
	switch(keyword) {
		case KEY_DEFUNC:
			return "defunc";
		case KEY_VAR:
			return "VAR";
		case KEY_IF:
			return "if";
	}

	UNREACHABLE();
}

const char* punctuator_to_string(Punctuator punctuator) {
	switch(punctuator) {
		case PUNC_OPEN_PAREN:
			return "(";
		case PUNC_CLOSE_PAREN:
			return ")";
	}

	UNREACHABLE();
}


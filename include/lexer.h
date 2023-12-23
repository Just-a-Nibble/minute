#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>

typedef enum {
	TOK_IDENTIFIER,
	TOK_KEYWORD,
	TOK_PUNCTUATOR,
	TOK_INTEGER_LITERAL,
	TOK_FLOAT_LITERAL,
	TOK_STRING_LITERAL,
	TOK_EOF
} TokenType;

typedef char* Identifier;

typedef enum {
	KEY_DEFUNC,
	KEY_VAR,
	KEY_IF
} Keyword;

typedef enum {
	PUNC_OPEN_PAREN,
	PUNC_CLOSE_PAREN
} Punctuator;

typedef int IntegerLiteral;

typedef float FloatLiteral;

typedef char* StringLiteral;

typedef struct {
	TokenType type;

	union {
		Identifier identifier;
		Keyword keyword;
		Punctuator punctuator;
		IntegerLiteral integer_literal;
		FloatLiteral float_literal;
		StringLiteral string_literal;
	} data;
} Token;

Token next_token(FILE* file);

Token expect_token(FILE* file, TokenType type);
Token expect_keyword(FILE* file, Keyword keyword);
Token expect_punctuator(FILE* file, Punctuator punctuator);

void destroy_token(Token* token);

const char* token_type_to_string(TokenType type);
const char* keyword_to_string(Keyword keyword);
const char* punctuator_to_string(Punctuator punctuator);

#endif /* _LEXER_H */


#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "lexer.h"

#include "macros.h"

static Expression* _allocate_expression(void) {
	Expression* expression = malloc(sizeof(Expression));
	if(expression == NULL) {
		fputs("[ERROR] Out of Memory.\n", stderr);
		exit(1);
	}

	return expression;
}

static Expression* _parse_call_expression(FILE* file, Identifier identifier) {
	Expression** arguments = NULL;
	size_t argument_count = 0;

	for(;;) {
		const long int location = ftell(file);

		const Token token = next_token(file);

		if(token.type == TOK_PUNCTUATOR && token.data.punctuator == PUNC_CLOSE_PAREN) {
			break;
		}

		fseek(file, location, SEEK_SET);

		arguments = realloc(arguments, (argument_count + 1) * sizeof(Expression*));
		if(arguments == NULL) {
			fputs("[ERROR] Out of Memory.\n", stderr);
			exit(1);
		}

		arguments[argument_count++] = parse_expression(file);
	}

	Expression* expression = _allocate_expression();
	expression->type = EXPR_CALL;
	expression->data.call.name = identifier;
	expression->data.call.arguments = arguments;
	expression->data.call.argument_count = argument_count;

	return expression;
}

static Expression* _parse_if_else_expression(FILE* file) {
	Expression* expression = _allocate_expression();
	expression->type = EXPR_IF_ELSE;

	expression->data.if_else.condition  = parse_expression(file);
	expression->data.if_else.true_body  = parse_expression(file);
	expression->data.if_else.false_body = parse_expression(file);

	expect_punctuator(file, PUNC_CLOSE_PAREN);

	return expression;
}

static Expression* _parse_variable_declaration_expression(FILE* file) {
	Expression* expression = _allocate_expression();
	expression->type = EXPR_VARIABLE_DECLARATION;

	expect_punctuator(file, PUNC_OPEN_PAREN);

	expression->data.variable_declaration.name  = expect_token(file, TOK_IDENTIFIER).data.identifier;
	expression->data.variable_declaration.value = parse_expression(file);

	expect_punctuator(file, PUNC_CLOSE_PAREN);

	expression->data.variable_declaration.expression = parse_expression(file);

	expect_punctuator(file, PUNC_CLOSE_PAREN);

	return expression;
}

Expression* parse_expression(FILE* file) {
	Expression* expression = NULL;

	const Token token = next_token(file);

	switch(token.type) {
		/* Literals */
		case TOK_INTEGER_LITERAL:
			expression = _allocate_expression();
			expression->type = EXPR_INTEGER_LITERAL;
			expression->data.integer_literal = token.data.integer_literal;
			return expression;
		case TOK_FLOAT_LITERAL:
			expression = _allocate_expression();
			expression->type = EXPR_FLOAT_LITERAL;
			expression->data.float_literal = token.data.float_literal;
			return expression;
		case TOK_STRING_LITERAL:
			expression = _allocate_expression();
			expression->type = EXPR_STRING_LITERAL;
			expression->data.string_literal = token.data.string_literal;
			return expression;

		/* Variable Get */
		case TOK_IDENTIFIER:
			expression = _allocate_expression();
			expression->type = EXPR_GET;
			expression->data.get = token.data.identifier;
			return expression;

		/* Invalid Tokens */
		case TOK_KEYWORD:
			fprintf(stderr, "[ERROR] Unexpected Keyword '%s' When Parsing Expression.\n",
				keyword_to_string(token.data.keyword)
			);
			exit(1);
		case TOK_EOF:
			fputs("[ERROR] Unexpected End of File When Parsing Expression.\n", stderr);
			exit(1);

		/* Most Common 'Happy' Path (Function Calls and Most Generics) */
		case TOK_PUNCTUATOR: {
			const Token token2 = next_token(file);

			/* TODO: Use '+', '-', '*', '/' instead of add, sub, mult, div */

			if(token2.type == TOK_IDENTIFIER) {
				return _parse_call_expression(file, token2.data.identifier);
			} else if(token2.type == TOK_KEYWORD) {
				switch(token2.data.keyword) {
					case KEY_DEFUNC:
						fprintf(stderr, "[ERROR] Unexpected Keyword '%s' When Parsing Call-Like Expression.\n",
							keyword_to_string(token2.data.keyword)
						);
						exit(1);

					case KEY_VAR:
						return _parse_variable_declaration_expression(file);
					case KEY_IF:
						return _parse_if_else_expression(file);
				}
			} else {
				fprintf(stderr, "[ERROR] Unexpected %s Token When Parsing Call-Like Expression.\n",
					token_type_to_string(token2.type)
				);
				exit(1);
			}
		} break;
	}

	UNREACHABLE();
}

FunctionDeclaration* parse_function_declaration(FILE* file) {
	expect_punctuator(file, PUNC_OPEN_PAREN);

	expect_keyword(file, KEY_DEFUNC);

	Identifier identifier = expect_token(file, TOK_IDENTIFIER).data.identifier;

	expect_punctuator(file, PUNC_OPEN_PAREN);

	Identifier* arguments = NULL;
	size_t argument_count = 0;

	for(;;) {
		const Token token = next_token(file);

		if(token.type == TOK_PUNCTUATOR && token.data.punctuator == PUNC_CLOSE_PAREN) {
			break;
		}

		if(token.type != TOK_IDENTIFIER) {
			fprintf(stderr, "[ERROR] Unexpected %s Token When Parsing Function Declaration.\n",
				token_type_to_string(token.type)
			);
			exit(1);
		}

		arguments = realloc(arguments, (argument_count + 1) * sizeof(Identifier));
		if(arguments == NULL) {
			fputs("[ERROR] Out of Memory.\n", stderr);
			exit(1);
		}

		arguments[argument_count++] = token.data.identifier;
	}

	Expression* body = parse_expression(file);

	expect_punctuator(file, PUNC_CLOSE_PAREN);

	FunctionDeclaration* declaration = malloc(sizeof(FunctionDeclaration));
	if(declaration == NULL) {
		fputs("[ERROR] Out of Memory.\n", stderr);
		exit(1);
	}

	declaration->name = identifier;
	declaration->arguments = arguments;
	declaration->argument_count = argument_count;
	declaration->body = body;

	return declaration;
}


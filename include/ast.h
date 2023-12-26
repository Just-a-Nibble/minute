#ifndef _AST_H
#define _AST_H

#include <stdio.h>
#include <stddef.h>

#include "token.h"

typedef enum {
	EXPR_INTEGER_LITERAL,
	EXPR_FLOAT_LITERAL,
	EXPR_STRING_LITERAL,

	EXPR_GET,
	EXPR_CALL,

	EXPR_IF_ELSE,

	EXPR_VARIABLE_DECLARATION
} ExpressionType;

typedef struct Expression Expression;
struct Expression {
	ExpressionType type;

	union {
		IntegerLiteral integer_literal;
		FloatLiteral float_literal;
		StringLiteral string_literal;

		Identifier get;

		struct {
			Identifier name;

			Expression** arguments;
			size_t argument_count;
		} call;


		struct {
			Expression* condition;

			Expression* true_body;
			Expression* false_body;
		} if_else;

		struct {
			Identifier name;
			Expression* value;

			Expression* expression;
		} variable_declaration;
	} data;
};

typedef struct {
	Identifier name;

	Identifier* arguments;
	size_t argument_count;

	Expression* body;
} FunctionDeclaration;

void print_expression(FILE* output, const Expression* expression, unsigned int tab_depth);
void print_function_declaration(FILE* output, const FunctionDeclaration* declaration, unsigned int tab_depth);

#endif /* _AST_H */


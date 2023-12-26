#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "macros.h"

static void _tab_padding(FILE* output, unsigned int tab_depth) {
	for(unsigned int i = 0; i < tab_depth; i++) {
		fputc('\t', output);
	}
}

void print_expression(FILE* output, const Expression* expression, unsigned int tab_depth) {
	switch(expression->type) {
		case EXPR_INTEGER_LITERAL:
			_tab_padding(output, tab_depth);
			fprintf(output, "Integer Literal %i\n", expression->data.integer_literal);
			break;
		case EXPR_FLOAT_LITERAL:
			_tab_padding(output, tab_depth);
			fprintf(output, "Float Literal %f\n", expression->data.float_literal);
			break;
		case EXPR_STRING_LITERAL:
			_tab_padding(output, tab_depth);
			fprintf(output, "String Literal \"%s\"\n", expression->data.string_literal);
			break;

		case EXPR_GET:
			_tab_padding(output, tab_depth);
			fprintf(output, "Get %s\n", expression->data.get);
			break;

		case EXPR_CALL:
			_tab_padding(output, tab_depth);
			fprintf(output, "Call %s\n", expression->data.call.name);

			for(size_t i = 0; i < expression->data.call.argument_count; i++) {
				print_expression(output, expression->data.call.arguments[i], tab_depth + 1);
			}

			break;

		case EXPR_IF_ELSE:
			_tab_padding(output, tab_depth);
			fprintf(output, "If\n");
			print_expression(output, expression->data.if_else.condition, tab_depth + 1);

			_tab_padding(output, tab_depth);
			fprintf(output, "Then\n");
			print_expression(output, expression->data.if_else.true_body, tab_depth + 1);

			_tab_padding(output, tab_depth);
			fprintf(output, "Else\n");
			print_expression(output, expression->data.if_else.false_body, tab_depth + 1);
			break;

		case EXPR_VARIABLE_DECLARATION:
			_tab_padding(output, tab_depth);
			fprintf(output, "Var %s\n", expression->data.variable_declaration.name);
			print_expression(output, expression->data.variable_declaration.value, tab_depth + 1);

			_tab_padding(output, tab_depth);
			fprintf(output, "For\n");
			print_expression(output, expression->data.variable_declaration.expression, tab_depth + 1);
			break;

		default: UNREACHABLE();
	}
}

void print_function_declaration(FILE* output, const FunctionDeclaration* declaration, unsigned int tab_depth) {
	_tab_padding(output, tab_depth);
	fprintf(output, "Define Function '%s'\n", declaration->name);

	for(size_t i = 0; i <declaration->argument_count; i++) {
		_tab_padding(output, tab_depth + 2);
		fprintf(output, "%s\n", declaration->arguments[i]);
	}

	print_expression(output, declaration->body, tab_depth + 1);
}


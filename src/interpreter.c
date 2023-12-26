#include "interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "ast.h"
#include "value.h"

#include "native_functions.h"

#include "macros.h"

static Value scope_find(ScopeNode* node, const String name) {
	while(node != NULL) {
		if(strcmp(node->name, name) == 0) {
			return node->value;
		}

		node = node->prev;
	}

	fprintf(stderr, "[ERROR] Variable '%s' Not Defined.\n",
		name
	);
	exit(1);
}

static bool coerce_bool(Value value) {
	switch(value.type) {
		case VAL_INTEGER:
			return value.data.integer != 0;
		case VAL_FLOATING:
			return value.data.floating != (Float)0.0;
		case VAL_STRING:
			return value.data.string != NULL && value.data.string[0] != '\0';
	}

	UNREACHABLE();
}

Value eval_expression(
	const Expression* expression,
	const Program* program,
	ScopeNode* scope
) {
	switch(expression->type) {
		case EXPR_INTEGER_LITERAL:
			return (Value) {
				.type = VAL_INTEGER,
				.data.integer = expression->data.integer_literal
			};
		case EXPR_FLOAT_LITERAL:
			return (Value) {
				.type = VAL_FLOATING,
				.data.floating = expression->data.float_literal
			};
		case EXPR_STRING_LITERAL:
			return (Value) {
				.type = VAL_STRING,
				.data.string = expression->data.string_literal
			};

		case EXPR_GET:
			return scope_find(scope, expression->data.get);

		case EXPR_CALL: {
			Value* arguments = malloc(sizeof(Value) * expression->data.call.argument_count);
			CHECK_OUT_OF_MEMORY(arguments);

			for(size_t i = 0; i < expression->data.call.argument_count; i++) {
				arguments[i] = eval_expression(expression->data.call.arguments[i], program, scope);
			}

			Value value = eval_function_call_from_name(
				expression->data.call.name,
				program,
				arguments, expression->data.call.argument_count
			);

			free(arguments);

			return value;
		} break;

		case EXPR_IF_ELSE:
			return
				coerce_bool(eval_expression(expression->data.if_else.condition, program, scope))
					? eval_expression(expression->data.if_else.true_body, program, scope)
					: eval_expression(expression->data.if_else.false_body, program, scope);

		case EXPR_VARIABLE_DECLARATION: {
			/* TODO: Could probably do something like github.com/TrevorMartin27/BrainStack */
			ScopeNode* node = malloc(sizeof(node));
			CHECK_OUT_OF_MEMORY(node);

			node->name = expression->data.variable_declaration.name;
			node->value = eval_expression(expression->data.variable_declaration.value, program, scope);
			node->prev = scope;

			Value value = eval_expression(expression->data.variable_declaration.expression, program, node);

			free(node);

			return value;
		} break;
	}

	UNREACHABLE();
}

Value eval_function_call_from_name(
	String name,
	const Program* program,
	Value* arguments,
	size_t argument_count
) {
	for(size_t i = 0; i < program->function_count; i++) {
		if(strcmp(name, program->functions[i]->name) == 0) {
			return eval_function_call(program->functions[i], program, arguments, argument_count);
		}
	}

	NativeFunction native_function = get_native_function(name);
	if(native_function != NULL) {
		return native_function(program, arguments, argument_count);
	}

	fprintf(stderr, "[ERROR] Function '%s' Not Defined.\n",
		name
	);
	exit(1);
}

Value eval_function_call(
	const FunctionDeclaration* function,
	const Program* program,
	Value* arguments,
	size_t argument_count
) {
	if(function->argument_count != argument_count) {
		fprintf(stderr, "[ERROR] Function Argument Count Mismatch; Expected %zu, Not %zu.\n",
			function->argument_count,
			argument_count
		);
		exit(1);
	}

	ScopeNode* scope = NULL;

	for(size_t i = 0; i < argument_count; i++) {
		ScopeNode* new_scope = malloc(sizeof(ScopeNode));
		CHECK_OUT_OF_MEMORY(new_scope);

		new_scope->name = function->arguments[i];
		new_scope->value = arguments[i];
		new_scope->prev = scope;

		scope = new_scope;
	}

	Value value = eval_expression(function->body, program, scope);

	while(scope != NULL) {
		ScopeNode* next = scope->prev;

		free(scope);

		scope = next;
	}

	return value;
}


#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "ast.h"
#include "value.h"

typedef struct ScopeNode ScopeNode;
struct ScopeNode {
	String name;
	Value value;

	ScopeNode* prev;
};

Value eval_expression(
	const Expression* expression,
	const Program* program,
	ScopeNode* scope
);

Value eval_function_call_from_name(
	String name,
	const Program* program,
	Value* arguments, size_t argument_count
);

Value eval_function_call(
	const FunctionDeclaration* function,
	const Program* program,
	Value* arguments, size_t argument_count
);

#endif /* _INTERPRETER_H */


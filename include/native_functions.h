#ifndef _NATIVE_FUNCTIONS_H
#define _NATIVE_FUNCTIONS_H

#include "ast.h"
#include "value.h"

typedef Value (*NativeFunction)(
	const Program* program,
	const Value* arguments, size_t argument_count
);

NativeFunction get_native_function(const String name);

Value native_add(const Program* program, const Value* arguments, size_t argument_count);

#endif /* _NATIVE_FUNCTIONS_H */


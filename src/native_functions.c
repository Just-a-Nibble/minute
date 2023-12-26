#include "native_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "macros.h"

static const struct {
	const String name;
	NativeFunction function;
} native_functions[] = {
	{ "add", &native_add }
};

NativeFunction get_native_function(const String name) {
	for(size_t i = 0; i < ARRAY_LENGTH(native_functions); i++) {
		if(strcmp(native_functions[i].name, name) == 0) {
			return native_functions[i].function;
		}
	}

	return NULL;
}

Value native_add(const Program* program, const Value* arguments, size_t argument_count) {
	UNUSED_PARAMETER(program);

	if(argument_count < 1) {
		fputs("[ERROR] Native function 'add' requires at least 1 argument.\n", stderr);
		exit(1);
	}

	Value value = arguments[0];
	for(size_t i = 1; i < argument_count; i++) {
		switch(arguments[i].type) {
			case VAL_INTEGER:
				if(value.type == VAL_INTEGER) {
					value.data.integer += arguments[i].data.integer;
				} else if(value.type == VAL_FLOATING) {
					value.data.floating += (Float)arguments[i].data.integer;
				} else {
					UNREACHABLE();
				}
				break;

			case VAL_FLOATING:
				if(value.type == VAL_INTEGER) {
					value.type = VAL_FLOATING;
					value.data.floating = (Float)value.data.integer;

					value.data.floating += arguments[i].data.floating;
				} else if(value.type == VAL_FLOATING) {
					value.data.floating += arguments[i].data.floating;
				} else {
					UNREACHABLE();
				}
				break;

			case VAL_STRING:
				fputs("[ERROR] Cannot perform native 'add' on string. Do you mean str_concat?.\n", stderr);
				exit(1);
		}
	}

	return value;
}


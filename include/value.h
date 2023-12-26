#ifndef _VALUE_H
#define _VALUE_H

#include "types.h"

typedef enum {
	VAL_INTEGER,
	VAL_FLOATING,
	VAL_STRING
} ValueType;

typedef struct {
	ValueType type;

	union {
		Integer integer;
		Float floating;
		String string;
	} data;
} Value;

#endif /* _VALUE_H */


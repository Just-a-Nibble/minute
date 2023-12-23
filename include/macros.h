#ifndef _MACROS_H
#define _MACROS_H

#define UNUSED_PARAMETER( V) (void) ( V)

#define ARRAY_LENGTH( A) (sizeof( A) / sizeof(( A)[0]))

#define UNREACHABLE() \
	fputs("[ERROR] Reached Unreachable.\n", stderr); \
	exit(1);

#endif /* _MACROS_H */


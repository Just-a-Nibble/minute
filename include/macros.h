#ifndef _MACROS_H
#define _MACROS_H

#define UNUSED_PARAMETER( V) (void) ( V)

#define ARRAY_LENGTH( A) (sizeof( A) / sizeof(( A)[0]))

#define OUT_OF_MEMORY() \
	fputs("[ERROR] Out of Memory.\n", stderr); \
	exit(1);

#define CHECK_OUT_OF_MEMORY( P) \
	if(( P) == NULL) { \
		OUT_OF_MEMORY(); \
	}

#define TODO() \
	fputs("[ERROR] Unimplemented Code Route.\n", stderr); \
	exit(1);

#define UNREACHABLE() \
	fputs("[ERROR] Reached Unreachable.\n", stderr); \
	exit(1);

#endif /* _MACROS_H */


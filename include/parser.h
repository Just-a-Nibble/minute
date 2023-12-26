#ifndef _PARSER_H
#define _PARSER_H

#include <stdbool.h>

#include "ast.h"

Expression* parse_expression(FILE* file, bool internal);

FunctionDeclaration* parse_function_declaration(FILE* file, bool internal);

Program* parse_program(FILE* file);

#endif /* _PARSER_H */


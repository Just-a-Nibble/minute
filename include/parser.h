#ifndef _PARSER_H
#define _PARSER_H

#include "ast.h"

Expression* parse_expression(FILE* file);

FunctionDeclaration* parse_function_declaration(FILE* file);

#endif /* _PARSER_H */


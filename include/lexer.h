#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>

#include "token.h"

Token next_token(FILE* file);

Token expect_token(FILE* file, TokenType type);
Token expect_keyword(FILE* file, Keyword keyword);
Token expect_punctuator(FILE* file, Punctuator punctuator);

#endif /* _LEXER_H */


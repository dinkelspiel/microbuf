#include "file.h"
#include "stdio.h"
#include "types.h"

#ifndef LEXER_H

Token *lex(FileContents *raw_file);

char *token_to_string(Token *token);
char *token_type_to_string(TokenType type);
void print_token(Token *token);

#endif
#define LEXER_H
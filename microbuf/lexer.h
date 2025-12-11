#include "file.h"
#include "stdio.h"
#include "types.h"

#ifndef LEXER_H

Token *lex(FileContents *raw_file);

char *token_to_string(Token *token);
void print_token(Token *token);

#endif
#define LEXER_H
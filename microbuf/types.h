#ifndef TYPES_H

typedef enum {
  TOKEN_IDENTIFIER = 0,
  TOKEN_LEFT_BRACE = 1,
  TOKEN_RIGHT_BRACE = 2,
  TOKEN_EQUALS = 3,
  TOKEN_INT = 4,
  TOKEN_SEMI_COLON = 5,
  TOKEN_QUESTIONMARK = 6,
  TOKEN_EOF = 7
} TokenType;

typedef struct Token {
  TokenType type;
  char *lexeme;
  int row;
  int col;
  struct Token *next;
} Token;

typedef struct {
  Token *token_start;
  Token *token_end;
  int row;
  int col;
  char *acc;
} LexerContext;

#endif
#define TYPES_H
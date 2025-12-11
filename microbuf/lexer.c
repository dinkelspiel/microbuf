#include "lexer.h"
#include "string.h"
#include "utils.h"

void add_token(LexerContext *ctx, TokenType type, char *lexeme) {
  Token *token = malloc(sizeof(Token));
  token->type = type;
  token->lexeme = lexeme;
  token->row = ctx->row;

  int col = ctx->col;
  int lexeme_length = strlen(lexeme);
  if (lexeme_length > 1) {
    col -= lexeme_length;
  }
  token->col = col;

  if (ctx->token_end != NULL) {
    ctx->token_end->next = token;
    ctx->token_end = token;
  } else {
    ctx->token_end = token;
    ctx->token_start = token;
  }
}

void default_acc(LexerContext *ctx) {
  ctx->acc = malloc(1);
  ctx->acc[0] = '\0';
}

void pop_acc(LexerContext *ctx) {
  if (strcmp(ctx->acc, "") == 0)
    return;

  if (is_string_numeric(ctx->acc))
    add_token(ctx, TOKEN_INT, ctx->acc);
  else
    add_token(ctx, TOKEN_IDENTIFIER, ctx->acc);

  default_acc(ctx);
}

Token *lex(FileContents *raw_file) {
  LexerContext *ctx = malloc(sizeof(LexerContext));
  ctx->row = 1;
  ctx->col = 0;
  ctx->token_start = NULL;
  ctx->token_end = NULL;
  ctx->acc = NULL;
  default_acc(ctx);

  for (int i = 0; i < raw_file->length; i++) {
    ctx->col++;
    if (raw_file->contents[i] == '\n') {
      ctx->row++;
      ctx->col = 0;
      continue;
    }
    switch (raw_file->contents[i]) {
    case ' ':
      pop_acc(ctx);
      continue;
    case '{':
      pop_acc(ctx);
      add_token(ctx, TOKEN_LEFT_BRACE, "{");
      break;
    case '}':
      pop_acc(ctx);
      add_token(ctx, TOKEN_RIGHT_BRACE, "}");
      break;
    case '=':
      pop_acc(ctx);
      add_token(ctx, TOKEN_EQUALS, "=");
      break;
    case ';':
      pop_acc(ctx);
      add_token(ctx, TOKEN_SEMI_COLON, ";");
      break;
    case '?':
      pop_acc(ctx);
      add_token(ctx, TOKEN_QUESTIONMARK, "?");
      break;
    default:
      int acc_length = strlen(ctx->acc);
      char *tmp = realloc(ctx->acc, acc_length + 2);
      if (!tmp) {
        printf("failed to allocate accumulator in lexer");
        return 0;
      }

      ctx->acc = tmp;
      ctx->acc[acc_length] = raw_file->contents[i];
      ctx->acc[acc_length + 1] = '\0';
      break;
    }
  }

  Token *start = ctx->token_start;
  free(ctx);
  return start;
}

char *token_to_string_map[] = {"identifier",   "left_brace", "right_brace",
                               "equals",       "int",        "semi_colon",
                               "questionmark", "eof"};

char *token_to_string(Token *token) { return token_to_string_map[token->type]; }
char *token_type_to_string(TokenType type) { return token_to_string_map[type]; }

void print_token(Token *token) {
  int map_length = 8;
  int max_value_length = 0;

  for (int i = 0; i < map_length; i++) {
    int value_length = strlen(token_to_string_map[i]);
    if (value_length > max_value_length)
      max_value_length = value_length;
  }

  char row_col[8];
  sprintf(row_col, "%d:%d", token->row, token->col);
  printf("%s %s %s\n", pad_right(row_col, 7, ' '),
         pad_right(token_to_string_map[token->type], max_value_length + 1, ' '),
         token->lexeme);
}
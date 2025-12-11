#include "lexer.h"
#include "stdio.h"
#include "stdlib.h"
#include "types.h"
#include "utils.h"
#include <stdarg.h>

typedef struct {
  char *source;
  Token *current_token;
} ParserContext;

void error(ParserContext *ctx, char *error) {
  char *error_line = get_string_by_index_within_delimeters(
      ctx->source, '\n', ctx->current_token->row);

  // printf("=====\n");
  // print_token(ctx->current_token);

  char row[4];
  snprintf(row, sizeof(row), "%d", ctx->current_token->row);

  char *arrow = "^";

  char *empty_line_start_padding = pad_right("", 2, ' ');
  char *number_line_start_padding = pad_right(row, 2, ' ');
  char *positioned_error =
      pad_left(arrow, ctx->current_token->col + strlen(arrow) - 1, ' ');

  printf("error: %s\n", error);
  printf("%s │\n", empty_line_start_padding);
  printf("%s │ %s\n", number_line_start_padding, error_line);
  printf("%s │ %s\n\n", empty_line_start_padding, positioned_error);

  free(empty_line_start_padding);
  // free(number_line_start_padding);
  // free(positioned_error);
  free(error_line);
}

void errorf(ParserContext *ctx, const char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  int needed = vsnprintf(NULL, 0, fmt, ap);
  va_end(ap);

  if (needed < 0) {
    error(ctx, "internal formatting error");
    return;
  }

  char *buf = malloc((size_t)needed + 1);
  if (!buf) {
    error(ctx, "out of memory");
    return;
  }

  va_start(ap, fmt);
  vsnprintf(buf, (size_t)needed + 1, fmt, ap);
  va_end(ap);

  error(ctx, buf);

  free(buf);
}

void parse(Token *token, char *source) {
  ParserContext *ctx = malloc(sizeof(ParserContext));
  ctx->current_token = token;
  ctx->source = source;

  while (ctx->current_token != NULL) {
    if (ctx->current_token->type != TOKEN_IDENTIFIER) {
      errorf(ctx, "unexpected token, expected identifier, found %s",
             token_to_string(ctx->current_token));
    }

    ctx->current_token = ctx->current_token->next;
  }
}
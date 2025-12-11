#include "parser.h"
#include "lexer.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "types.h"
#include "utils.h"

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

void advance(ParserContext *ctx) {
  ctx->current_token = ctx->current_token->next;
}

void assert(ParserContext *ctx, TokenType type) {
  if (ctx->current_token->type != type) {
    errorf(ctx, "unexpected token, expected %s, found %s",
           token_type_to_string(type), token_to_string(ctx->current_token));
    exit(1);
    return;
  }
}

void assert_advance(ParserContext *ctx, TokenType type) {
  assert(ctx, type);
  advance(ctx);
}

void parse_message(ParserContext *ctx) {
  assert(ctx, TOKEN_IDENTIFIER);

  AstNode *node = malloc(sizeof(AstNode));
  node->next = NULL;
  node->kind = AST_MESSAGE;
  node->type.t_message = (AstMessage){.name = ctx->current_token->lexeme,
                                      .item_start = NULL,
                                      .item_end = NULL,
                                      .item_length = 0};

  advance(ctx);
  assert_advance(ctx, TOKEN_LEFT_BRACE);

  parse_message_item(ctx, node);

  if (ctx->ast_end == NULL) {
    ctx->ast_start = node;
    ctx->ast_end = node;
  } else {
    ctx->ast_end->next = node;
    ctx->ast_end = node;
  }
}

void parse_message_item(ParserContext *ctx, AstNode *node) {
  assert(ctx, TOKEN_IDENTIFIER);
  AstMessageItem *item = malloc(sizeof(AstMessageItem));
  item->type = ctx->current_token->lexeme;
  item->next = NULL;
  advance(ctx);

  if (ctx->current_token->type == TOKEN_QUESTIONMARK) {
    item->optional = true;
    advance(ctx);
  } else {
    item->optional = false;
  }

  assert(ctx, TOKEN_IDENTIFIER);
  item->name = ctx->current_token->lexeme;
  advance(ctx);

  assert_advance(ctx, TOKEN_EQUALS);

  assert(ctx, TOKEN_INT);
  item->position = atoi(ctx->current_token->lexeme);
  advance(ctx);

  assert_advance(ctx, TOKEN_SEMI_COLON);

  if (node->type.t_message.item_end == NULL) {
    node->type.t_message.item_start = item;
    node->type.t_message.item_end = item;
  } else {
    node->type.t_message.item_end->next = item;
    node->type.t_message.item_end = item;
  }

  if (ctx->current_token->type == TOKEN_RIGHT_BRACE) {
    advance(ctx);
  } else {
    parse_message_item(ctx, node);
  }
}

void print_ast_message(AstMessageItem *item) {
  printf("%s%s %s %d\n", item->type, item->optional ? "?" : "", item->name,
         item->position);

  if (item->next != NULL)
    print_ast_message(item->next);
}

void print_ast(AstNode *node) {
  if (node->kind == AST_MESSAGE) {
    printf("\nmessage %s\n", node->type.t_message.name);
    if (node->type.t_message.item_start != NULL) {
      print_ast_message(node->type.t_message.item_start);
    } else {
      printf("*empty message*\n");
    }
  }

  if (node->next != NULL)
    print_ast(node->next);
}

void parse(Token *token, char *source) {
  ParserContext *ctx = malloc(sizeof(ParserContext));
  ctx->current_token = token;
  ctx->source = source;
  ctx->ast_start = NULL;
  ctx->ast_end = NULL;

  printf("Starting parse\n");

  while (ctx->current_token != NULL) {
    if (ctx->current_token->type != TOKEN_IDENTIFIER) {
      errorf(ctx, "unexpected token. expected identifier, found %s",
             token_to_string(ctx->current_token));
      exit(1);
    }

    if (strcmp(ctx->current_token->lexeme, "message") == 0) {
      advance(ctx);
      parse_message(ctx);
    } else {
      errorf(ctx, "expected keyword 'message' or 'enum'. found %s",
             token_to_string(ctx->current_token));
      exit(1);
    }
  }

  print_ast(ctx->ast_start);
  free(ctx);
}
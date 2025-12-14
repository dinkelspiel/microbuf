#include "stdbool.h"
#include "types.h"

#ifndef PARSER_H

typedef struct AstMessageItem {
  char *type;
  char *name;
  int position;
  bool optional;

  struct AstMessageItem *next;
} AstMessageItem;

typedef struct {
  char *name;

  AstMessageItem *item_start;
  AstMessageItem *item_end;
  int item_length;
} AstMessage;

typedef struct {

} AstEnum;

typedef enum { AST_MESSAGE, AST_ENUM } AstNodeType;

typedef struct AstNode {
  union {
    AstMessage t_message;
    AstEnum t_enum;
  } type;
  AstNodeType kind;

  struct AstNode *next;
} AstNode;

typedef struct {
  char *source;
  Token *current_token;
  AstNode *ast_start;
  AstNode *ast_end;
} ParserContext;

AstNode *parse(Token *token, char *source);
void parse_message_item(ParserContext *ctx, AstNode *node);

#endif
#define PARSER_H
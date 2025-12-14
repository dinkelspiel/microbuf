#include "microbuf/file.h"
#include "microbuf/lexer.h"
#include "microbuf/parser.h"
#include <stdio.h>

int main() {
  FileContents *file = read_file("./examples/person.mb");

  Token *token = lex(file);

  Token *tmp_token = token;
  while (tmp_token != NULL) {
    print_token(tmp_token);
    tmp_token = tmp_token->next;
  }

  AstNode *ast = parse(token, file->contents);

  return 0;
}
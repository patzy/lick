#ifndef LICK_LEXER_H
#define LICK_LEXER_H 1

#include <stdlib.h>

typedef enum {
  TOK_START,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_QUOTE,
  TOK_ATOM,
  TOK_COMMENT
} token_type;

typedef struct _token_t {
  token_type type;
  const char *start,*end;
  size_t len;
  struct _token_t *next;
} token_t;

token_t*
tokenize(const char* str,size_t len);

void
token_free(token_t *token);

void
token_print(token_t *token);

#endif // LICK_LEXER_H

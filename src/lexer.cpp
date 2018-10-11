#include <lick/lexer.h>
#include <lick/lick.h>

token_t*
make_token(token_type type,const char* start,const char* end) {
  token_t *tok = LICK_ALLOC(token_t);
  tok->type = type;
  tok->start = start;
  tok->end = end;
  tok->len = (size_t)(end-start);
  tok->next = NULL;
  return tok;
}

bool
is_delim(char c) {
  return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '(' || c == ')' ||
          c == '\'');
}

token_t*
tokenize(const char* str,size_t len) {
  size_t i = 0;
  token_t *token = LICK_ALLOC(token_t);
  token->type = TOK_START;
  token->start = str;
  token->end = str+len;
  token->next = NULL;
  token_t *cur_tok = token;
  int32_t atom_start = -1;
  int32_t string_start = -1;
  int32_t comment_start = -1;
  while (i<len) {
    char c = str[i];
    // in a string skip until end quote
    // in a comment skip until EOL
    // skip delimiters
    if (is_delim(c)) {
      if (atom_start != -1) {
        token_t *tok = make_token(TOK_ATOM,str+atom_start,str+i);
        cur_tok->next = tok;
        cur_tok = tok;
        atom_start = -1;
      }
      if ((c == '\r' || c == '\n') && comment_start != -1) {
        token_t *tok = make_token(TOK_COMMENT,str+comment_start,str+i+1);
        cur_tok->next = tok;
        cur_tok = tok;
        comment_start = -1;
      }
    }
    else {
      if (c == '\"') {
        if (string_start == -1) {
          string_start = i;
        }
        else {
          token_t *tok = make_token(TOK_ATOM,str+string_start,str+i+1);
          cur_tok->next = tok;
          cur_tok = tok;
          string_start = -1;
        }
      }
      else if (string_start == -1 && c == ';') {
        if (atom_start != -1) {
          token_t *tok = make_token(TOK_ATOM,str+atom_start,str+i);
          cur_tok->next = tok;
          cur_tok = tok;
          atom_start = -1;
        }
        comment_start = i;
      }
      else if ((string_start == -1) && (comment_start == -1) && (atom_start == -1)) {
        atom_start = i;
      }
    }
    if (comment_start == -1) {
      if (c == '(') {
        token_t *tok = make_token(TOK_LPAREN,str+i,str+i+1);
        cur_tok->next = tok;
        cur_tok = tok;
      }
      if (c == ')') {
        token_t *tok = make_token(TOK_RPAREN,str+i,str+i+1);
        cur_tok->next = tok;
        cur_tok = tok;
      }
      if (c == '\'') {
        token_t *tok = make_token(TOK_QUOTE,str+i,str+i+1);
        cur_tok->next = tok;
        cur_tok = tok;
      }
    }
    ++i;
  }
  // read what's remaining
  if (atom_start != -1) {
    token_t *tok = make_token(TOK_ATOM,str+atom_start,str+i);
    cur_tok->next = tok;
    cur_tok = tok;
    atom_start = -1;
  }
  if (comment_start != -1) {
    token_t *tok = make_token(TOK_COMMENT,str+comment_start,str+i+1);
    cur_tok->next = tok;
    cur_tok = tok;
    comment_start = -1;
  }
  return token;
}

void
token_free(token_t *token) {
  token_t *next = token->next;
  if(next) {
    token_free(next);
  }
  LICK_FREE(token);
}

void
token_print(token_t *token) {
  token_t *tok = token;
  const char *c;
  while (tok!=NULL){
    switch(tok->type) {
    case TOK_START:
      printf("TOK_START\n");
      break;
    case TOK_QUOTE:
      printf("TOK_QUOTE\n");
      break;
    case TOK_LPAREN:
      printf("[\n");
      break;
    case TOK_RPAREN:
      printf("]\n");
      break;
    case TOK_ATOM:
      printf("TOK_ATOM ");
      c = tok->start;
      while (c!=tok->end) {
        printf("%c",*c);
        c++;
      }
      printf("\n");
      break;
    case TOK_COMMENT:
      printf("TOK_COMMENT ");
      c = tok->start;
      while (c!=tok->end) {
        printf("%c",*c);
        c++;
      }
      printf("\n");
      break;
    }
    tok = tok->next;
  }
}

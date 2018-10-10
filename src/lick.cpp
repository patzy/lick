#include <lick/lick.h>
#include <string.h>

struct {
  lick_t nil,t;
} g_env;

void
lick_init() {
  g_env.nil.type = LICK_NIL;
  g_env.t.type = LICK_TRUE;
}

void
lick_close() {
}

bool
lick_is_number(lick_t *obj) {
  return (obj->type == LICK_INT || obj->type == LICK_FLOAT);
}

lick_t*
lick_int(int val) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_INT;
  obj->int_val = val;
  obj->env = NULL;
  return obj;
}

lick_t*
lick_float(float val) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_FLOAT;
  obj->float_val = val;
  obj->env = NULL;
  return obj;
}

lick_t*
lick_string(const char* val) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_STRING;
  obj->string_val = strdup(val);
  obj->env = NULL;
  return obj;
}

lick_t*
lick_symbol(const char* val) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_SYMBOL;
  obj->string_val = strdup(val);
  obj->env = NULL;
  return obj;
}

lick_t*
lick_cons(lick_t *car,lick_t *cdr) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_CONS;
  obj->car = car;
  obj->cdr = cdr;
  obj->env = NULL;
  return obj;
}

lick_t*
lick_func(lick_t* args,lick_t *body) {
  lick_t *obj = LICK_ALLOC(lick_t);
  obj->ref_count = 0;
  obj->type = LICK_FUNC;
  obj->car = args;
  obj->cdr = body;
  obj->env = NULL;
  return obj;
}

lick_t*
lick_nil() {
  return &g_env.nil;
}

lick_t*
lick_true() {
  return &g_env.t;
}

lick_t*
lick_ref(lick_t *obj) {
  obj->ref_count++;
  return obj;
}

lick_t*
lick_append(lick_t *cons,lick_t *obj) {
  if (cons && cons->type != LICK_CONS) {
    printf("Append needs a list\n");
    exit(-1);
  }
  if (!cons) {
    return lick_cons(obj,lick_nil());
  }
  else {
    lick_t *tail = cons;
    while (tail->cdr->type != LICK_NIL) {
      tail = tail->cdr;
    }
    tail->cdr = lick_cons(obj,lick_nil());
  }
  return cons;
}

bool
lick_equal(lick_t *v1,lick_t *v2) {
  if (v1->type != v2->type) return false;
  switch(v1->type) {
  case LICK_INT:
    return v1->int_val == v2->int_val;
    break;
  case LICK_FLOAT:
    return v1->float_val == v2->float_val;
    break;
  case LICK_STRING:
  case LICK_SYMBOL:
    return strcmp(v1->string_val,v2->string_val) == 0;
    break;
  default:
    return false;
  }
}

bool
lick_less(lick_t *v1,lick_t *v2) {
  if (!lick_is_number(v1) || !lick_is_number(v2)) {
    return false;
  }
  if (v1->type == v2->type) {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val < v2->int_val;
    case LICK_FLOAT:
      return v1->float_val < v2->float_val;
    default:
      return false;
    }
  }
  else {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val < v2->float_val;
    case LICK_FLOAT:
      return v2->int_val > v1->float_val;
    default:
      return false;
    }
  }
}

bool
lick_sup(lick_t *v1,lick_t *v2) {
  if (!lick_is_number(v1) || !lick_is_number(v2)) {
    return false;
  }
  if (v1->type == v2->type) {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val > v2->int_val;
    case LICK_FLOAT:
      return v1->float_val > v2->float_val;
    default:
      return false;
    }
  }
  else {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val > v2->float_val;
    case LICK_FLOAT:
      return v2->int_val < v1->float_val;
    default:
      return false;
    }
  }
}

bool
lick_leq(lick_t *v1,lick_t *v2) {
  if (!lick_is_number(v1) || !lick_is_number(v2)) {
    return false;
  }
  if (v1->type == v2->type) {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val <= v2->int_val;
    case LICK_FLOAT:
      return v1->float_val <= v2->float_val;
    default:
      return false;
    }
  }
  else {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val <= v2->float_val;
    case LICK_FLOAT:
      return v2->int_val >= v1->float_val;
    default:
      return false;
    }
  }
}

bool
lick_seq(lick_t *v1,lick_t *v2) {
  if (!lick_is_number(v1) || !lick_is_number(v2)) {
    return false;
  }
  if (v1->type == v2->type) {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val >= v2->int_val;
    case LICK_FLOAT:
      return v1->float_val >= v2->float_val;
    default:
      return false;
    }
  }
  else {
    switch(v1->type) {
    case LICK_INT:
      return v1->int_val >= v2->float_val;
    case LICK_FLOAT:
      return v2->int_val <= v1->float_val;
    default:
      return false;
    }
  }
}

lick_t*
lick_f_less(lick_t *args,lick_env_t *env) {
  lick_t *v1 = lick_eval(args->car,env);
  lick_t *v2 = lick_eval(args->cdr->car,env);
  lick_t *res = (lick_less(v1,v2)?lick_true():lick_nil());
  lick_free(v1);
  lick_free(v2);
  return res;
}

lick_t*
lick_f_leq(lick_t *args,lick_env_t *env) {
  lick_t *v1 = lick_eval(args->car,env);
  lick_t *v2 = lick_eval(args->cdr->car,env);
  lick_t *res = (lick_leq(v1,v2)?lick_true():lick_nil());
  lick_free(v1);
  lick_free(v2);
  return res;
}

lick_t*
lick_f_sup(lick_t *args,lick_env_t *env) {
  lick_t *v1 = lick_eval(args->car,env);
  lick_t *v2 = lick_eval(args->cdr->car,env);
  lick_t *res = (lick_sup(v1,v2)?lick_true():lick_nil());
  lick_free(v1);
  lick_free(v2);
  return res;
}

lick_t*
lick_f_seq(lick_t *args,lick_env_t *env) {
  lick_t *v1 = lick_eval(args->car,env);
  lick_t *v2 = lick_eval(args->cdr->car,env);
  lick_t *res = (lick_seq(v1,v2)?lick_true():lick_nil());
  lick_free(v1);
  lick_free(v2);
  return res;
}

lick_t*
lick_car(lick_t *obj) {
  if (obj->type == LICK_CONS) {
    return obj->car;
  }
  return NULL;
}

lick_t*
lick_cdr(lick_t *obj) {
  if (obj->type == LICK_CONS) {
    return obj->cdr;
  }
  return NULL;
}

void
lick_free(lick_t *obj) {
  if (obj->ref_count == 0) {
    switch(obj->type) {
    case LICK_CONS:
    case LICK_FUNC:
      if (obj->car) {lick_free(obj->car);}
      if (obj->cdr) {lick_free(obj->cdr);}
      break;
    case LICK_STRING:
    case LICK_SYMBOL:
      free(obj->string_val);
      break;
    default:
      break;
    }
    if (obj != &g_env.nil && obj != &g_env.t) {
      // printf("Freeing lick_t@%p\n",obj);
      if (obj->env) {
        lick_destroy_env(obj->env);
      }
      LICK_FREE(obj);
    }
  }
  else {
    obj->ref_count--;
    // printf("Dec refcount@%p [%d]\n",obj,obj->ref_count);
  }
}

lick_t*
lick_apply(lick_t *func,lick_t *args,lick_env_t *env) {
  lick_env_t *local_env = lick_make_env(env);
  lick_t* func_args = func->car;
  lick_t* func_body = func->cdr;
  lick_t *cur_sym = func_args;
  lick_t *cur_arg = args;
  // TODO: check number of arguments
  while(cur_sym->type!=LICK_NIL) {
    //printf("Setting lambda arg\n");
    //lick_println(cur_sym->car);
    //lick_println(cur_arg->car);
    lick_t *val = lick_eval(cur_arg->car,env);
    lick_t *sym = cur_sym->car;
    lick_env_add_var(local_env,sym->string_val,lick_ref(val));
    lick_free(val);
    cur_sym = cur_sym->cdr;
    cur_arg = cur_arg->cdr;
  }
  // printf("Local env:\n");
  // lick_env_print(local_env);
  // printf("Outer env:\n");
  // lick_env_print(env);
  lick_t *cur_expr = func_body;
  lick_t *res = NULL;
  while (cur_expr->type!=LICK_NIL) {
    if (res) {lick_free(res);}
    res = lick_eval(cur_expr->car,local_env);
    cur_expr = cur_expr->cdr;
  }
  //  lick_destroy_env(local_env);
  res->env = local_env;
  return res;
}

lick_t*
_lick_parse(token_t *cur_tok,token_t **next_tok);

lick_t*
lick_parse_atom(token_t *token) {
  char str[256];
  // FIXME: use strtod() && strtol() here
  bool is_number = true;
  bool is_float = false;
  for (size_t i=0;i<token->len;++i) {
    if ((token->start[i] < '0' || token->start[i] > '9') &&
        (token->start[i] != '.')) {
      is_number = false;
      break;
    }
    if (token->start[i] == '.') {
      if (!is_float) {is_float = true;}
      else {
        is_number = false;
        break;
      }
    }
  }
  if (is_number) {
    strncpy(str,token->start,token->len);
    str[token->len] = '\0';
    if (is_float) {
      return lick_float(atof(str));
    }
    else {
      return lick_int(atoi(str));
    }
  }
  if (*token->start=='"') {
    strncpy(str,token->start+1,token->len-2);
    str[token->len-2] = '\0';
    return lick_string(str);
  }
  else {
    strncpy(str,token->start,token->len);
    str[token->len] = '\0';
    return lick_symbol(str);
  }
}

lick_t*
lick_parse_list(token_t* token,token_t **next_tok) {
  token_t *cur_tok = token;
  lick_t *lst = NULL;
  while(cur_tok!=NULL) {
    switch(cur_tok->type) {
    case TOK_LPAREN: {
      token_t *next = NULL;
      lst = lick_append(lst,lick_parse_list(cur_tok->next,&next));
      cur_tok = next;
      break;
    }
    case TOK_RPAREN:
      *next_tok = cur_tok->next;
      return lst;
      break;
    case TOK_ATOM:
      lst = lick_append(lst,lick_parse_atom(cur_tok));
      cur_tok = cur_tok->next;
      break;
    case TOK_QUOTE: {
      token_t *next = NULL;
      lst = lick_append(lst,lick_cons(lick_symbol("quote"),
                                      lick_cons(_lick_parse(cur_tok->next,&next),
                                                lick_nil())));
      cur_tok = next;
      break;
    }
    default:
      printf("Invalid token\n");
      return NULL;
    }
  }
  printf("Missing closing paren\n");
  return lst;
}


lick_t*
_lick_parse(token_t *cur_tok,token_t **next_tok) {
  switch(cur_tok->type) {
  case TOK_START:
    printf("Invalid start token\n");
    return NULL;
    break;
  case TOK_QUOTE: {
    return lick_cons(lick_symbol("quote"),lick_cons(_lick_parse(cur_tok->next,next_tok),
                                                    lick_nil()));
    break;
  }
  case TOK_LPAREN: {
    return lick_parse_list(cur_tok->next,next_tok);
    break;
  }
  case TOK_RPAREN:
    printf("Invalid closing paren\n");
    return NULL;
    break;
  case TOK_ATOM:
    *next_tok = cur_tok->next;
    return lick_parse_atom(cur_tok);
    break;
  }
  return NULL;
}


lick_t*
lick_parse(token_t *start_tok) {
  if (start_tok->type!=TOK_START) {
    printf("Invalid token\n");
    return NULL;
  }
  token_t *cur_tok = start_tok->next;
  token_t *tok = NULL;
  return _lick_parse(cur_tok,&tok);
}

void
lick_print_cons(lick_t *obj,bool inside) {
  if (obj->cdr->type != LICK_CONS) {
    if (obj->cdr->type != LICK_NIL) {
      if (inside) {
        printf("(");
      }
      lick_print(obj->car);
      printf(" . ");
      lick_print(obj->cdr);
      if (inside) {
        printf(")");
      }
    }
    else {
      lick_print(obj->car);
    }
  }
  else {
    lick_print(obj->car);
    printf(" ");
    lick_print_cons(obj->cdr,true);
  }
}

void
lick_print(lick_t *obj) {
  switch(obj->type) {
  case LICK_INT:
    printf("%i",obj->int_val);
    break;
  case LICK_FLOAT:
    printf("%f",obj->float_val);
    break;
  case LICK_STRING:
    printf("\"%s\"",obj->string_val);
    break;
  case LICK_SYMBOL:
    printf("%s",obj->string_val);
    break;
  case LICK_CONS:
    printf("(");
    lick_print_cons(obj,false);
    printf(")");
    break;
  case LICK_FUNC:
    printf("<function ");
    lick_print(obj->car);
    printf(" ");
    lick_print(obj->cdr);
    printf(">");
    break;
  case LICK_NIL:
    printf("NIL");
    break;
  case LICK_TRUE:
    printf("T");
    break;
  }
}

lick_t*
lick_f_quote(lick_t *args,lick_env_t*) {
  return lick_ref(args->car);
}

// lick_t*
// lick_f_quasiquote(lick_t *args,lick_env_t* env) {
//   lick_t *cur = args;
//   lick_t *v = NULL;
//   while(cur->type!=LICK_NIL) {
//     if (v != NULL) {lick_free(v);}
//     v = lick_eval(cur->car,env);
//     cur = cur->cdr;
//   }
//   return v;
// }

// lick_t*
// lick_f_macroexpand(lick_t *args,lick_env_t *env) {
//   lick_t *cur = args;
//   if (cur->type != LICK_CONS || (cur->type == LICK_SYMBOL &&
//                                  strncmp(cur->string_val,"quote",5))) {
//     return lick_ref(cur->cdr);
//   }
// }

lick_t*
lick_f_car(lick_t *args,lick_env_t *env) {
  if (args->type == LICK_CONS) {
    lick_t *v = lick_eval(args->car,env);
    lick_t *res = lick_ref(v->car);
    lick_free(v);
    return res;
  }
  return NULL;
}

lick_t*
lick_f_cdr(lick_t *args,lick_env_t *env) {
  if (args->type == LICK_CONS) {
    lick_t *v = lick_eval(args->car,env);
    lick_t *res = lick_ref(v->cdr);
    lick_free(v);
    return res;
  }
  return NULL;
}

lick_t*
lick_f_cons(lick_t *args,lick_env_t *env) {
  lick_t *ev_car = lick_eval(args->car,env);
  lick_t *ev_cdr = lick_eval(args->cdr->car,env);
  lick_println(ev_car);
  lick_println(ev_cdr);
  return lick_cons(ev_car,ev_cdr);
}

lick_t*
lick_f_atomp(lick_t *args,lick_env_t *env) {
  lick_t *val = lick_eval(args->car,env);
  switch (val->type) {
  case LICK_CONS:
    lick_free(val);
    return lick_nil();
    break;
  default:
    lick_free(val);
    return lick_true();
    break;
  }
  lick_free(val);
  return lick_nil();
}

lick_t*
lick_f_symbolp(lick_t *args,lick_env_t *env) {
  // (symbolp expr)
  lick_t *val = lick_eval(args->car,env);
  switch (val->type) {
  case LICK_SYMBOL:
    lick_free(val);
    return lick_true();
    break;
  default:
    lick_free(val);
    return lick_nil();
    break;
  }
    lick_free(val);
  return lick_nil();
}

lick_t*
lick_f_eq(lick_t *args,lick_env_t *env) {
  // (eq e1 e2)
  lick_t *v1 = lick_eval(args->car,env);
  lick_t *v2 = lick_eval(args->cdr->car,env);
  if (lick_equal(v1,v2)) {
    lick_free(v1);
    lick_free(v2);
    return lick_true();
  }
  else {
    lick_free(v1);
    lick_free(v2);
    return lick_nil();
  }
}

lick_t*
lick_f_cond(lick_t *args,lick_env_t *env) {
  // (cond (p1 e1) (p2 e2) ...)
  lick_t *cur = args;
  while(cur->type!=LICK_NIL) {
    lick_t *v = lick_eval(cur->car->car,env);
    if (v == lick_true()) {
      lick_free(v);
      return lick_eval(cur->car->cdr->car,env);
    }
    cur = cur->cdr;
  }
  return lick_nil();
}

lick_t*
lick_f_print(lick_t *args,lick_env_t* env) {
  lick_t *v = lick_eval(args->car,env);
  lick_print(v);
  lick_free(v);
  return lick_true();
}

lick_t*
lick_f_println(lick_t *args,lick_env_t* env) {
  lick_f_print(args,env);
  printf("\n");
  return lick_true();
}

lick_t*
lick_f_define(lick_t *args,lick_env_t *env) {
  // (define symbol expr) -> (eval expr)
  const char* sym = args->car->string_val;
  lick_t *val = lick_eval(args->cdr->car,env);
  lick_env_add_var(env,sym,lick_ref(val));
  return val;
}

lick_t*
lick_f_set(lick_t *args,lick_env_t *env) {
  // (set symbol expr) -> (eval expr)
  const char* sym = args->car->string_val;
  lick_env_entry_t *entry = lick_env_get(env,sym);
  if (entry) {
    lick_free(entry->value);
    lick_t *val = lick_eval(args->cdr->car,env);
    entry->value = lick_ref(val);
    return val;
  }
  else {
    return lick_f_define(args,env);
  }
}

lick_t*
lick_f_lambda(lick_t *args,lick_env_t *) {
  // (lambda (args) &body) -> function
  lick_t* largs = args->car;
  lick_t* body = args->cdr;
  lick_t* func = lick_func(lick_ref(largs),lick_ref(body));
  return func;
}

lick_t*
lick_f_progn(lick_t *args,lick_env_t *env) {
  // (progn &body) -> eval all forms and return last one result
  lick_t *cur = args;
  lick_t *v = NULL;
  while(cur->type!=LICK_NIL) {
    if (v != NULL) {lick_free(v);}
    v = lick_eval(cur->car,env);
    cur = cur->cdr;
  }
  return v;
}

lick_t*
lick_f_print_env(lick_t *args,lick_env_t *env) {
  // (#print-env &body)
  lick_t *cur = args;
  if (cur->type == LICK_NIL) {
    lick_env_print(env);
  }
  else {
    while(cur->type!=LICK_NIL) {
      lick_t *v = lick_eval(cur->car,env);
      lick_env_t *e = v->env;
      if (e) {
        while (e) {
          lick_env_print(e);
          printf("-----\n");
          e = e->outer;
        }
      }
      else {
        printf("No environment\n");
      }
      cur = cur->cdr;
      lick_free(v);
    }
  }
  return lick_nil();
}

lick_t*
lick_f_add(lick_t* args,lick_env_t *env) {
  lick_t *cur = args;
  bool all_int = true;
  float res = 0.0f;
  while (cur->type != LICK_NIL) {
    lick_t *val = lick_eval(cur->car,env);
    if (val->type == LICK_FLOAT) {
      res += val->float_val;
      all_int = false;
    }
    else if (val->type == LICK_INT) {
      res += val->int_val;
    }
    else {
      printf("Invalid number\n");
      lick_free(val);
      return NULL;
    }
    lick_free(val);
    cur = cur->cdr;
  }
  if (all_int) {
    return lick_int(res);
  }
  else {
    return lick_float(res);
  }
}

lick_t*
lick_f_sub(lick_t* args,lick_env_t *env) {
  lick_t *cur = args;
  bool all_int = true;
  bool init = true;
  float res = 0.0f;
  while (cur->type != LICK_NIL) {
    lick_t *val = lick_eval(cur->car,env);
    if (val->type == LICK_FLOAT) {
      all_int = false;
      if (init) {
        res = val->float_val;
        init = false;
      }
      else {
        res -= val->float_val;
      }
    }
    else if (val->type == LICK_INT) {
      if (init) {
        res = val->int_val;
        init = false;
      }
      else {
        res -= (float)val->int_val;
      }
    }
    else {
      printf("Invalid number\n");
      lick_free(val);
      return NULL;
    }
    lick_free(val);
    cur = cur->cdr;
  }
  if (all_int) {
    return lick_int(res);
  }
  else {
    return lick_float(res);
  }
}

lick_t*
lick_f_mul(lick_t* args,lick_env_t *env) {
  lick_t *cur = args;
  bool all_int = true;
  float res = 1.0f;
  while (cur->type != LICK_NIL) {
    lick_t *val = lick_eval(cur->car,env);
    if (val->type == LICK_FLOAT) {
      res *= val->float_val;
      all_int = false;
    }
    else if (val->type == LICK_INT) {
      res *= val->int_val;
    }
    else {
      printf("Invalid number\n");
      lick_free(val);
      return NULL;
    }
    lick_free(val);
    cur = cur->cdr;
  }
  if (all_int) {
    return lick_int(res);
  }
  else {
    return lick_float(res);
  }
}

lick_t*
lick_f_div(lick_t* args,lick_env_t *env) {
  lick_t *cur = args;
  bool init = true;
  float res = 0.0f;
  while (cur->type != LICK_NIL) {
    lick_t *val = lick_eval(cur->car,env);
    if (val->type == LICK_FLOAT) {
      if (init) {
        res = val->float_val;
        init = false;
      }
      else {
        res /= val->float_val;
      }
    }
    else if (val->type == LICK_INT) {
      if (init) {
        res = val->int_val;
        init = false;
      }
      else {
        res /= (float)val->int_val;
      }
    }
    else {
      printf("Invalid number\n");
      lick_free(val);
      return NULL;
    }
    lick_free(val);
    cur = cur->cdr;
  }
  return lick_float(res);
}


void
lick_env_set_defaults(lick_env_t *env) {
  lick_env_add_var(env,"t",lick_true());
  lick_env_add_var(env,"T",lick_true());
  lick_env_add_var(env,"nil",lick_nil());
  lick_env_add_var(env,"NIL",lick_nil());
  lick_env_add_func(env,"+",lick_f_add);
  lick_env_add_func(env,"-",lick_f_sub);
  lick_env_add_func(env,"*",lick_f_mul);
  lick_env_add_func(env,"/",lick_f_div);
  lick_env_add_func(env,"<",lick_f_less);
  lick_env_add_func(env,">",lick_f_sup);
  lick_env_add_func(env,"<=",lick_f_leq);
  lick_env_add_func(env,">=",lick_f_seq);
  lick_env_add_func(env,"quote",lick_f_quote);
  lick_env_add_func(env,"car",lick_f_car);
  lick_env_add_func(env,"cdr",lick_f_cdr);
  lick_env_add_func(env,"cons",lick_f_cons);
  lick_env_add_func(env,"atomp",lick_f_atomp);
  lick_env_add_func(env,"symbolp",lick_f_symbolp);
  lick_env_add_func(env,"eq",lick_f_eq);
  lick_env_add_func(env,"cond",lick_f_cond);
  lick_env_add_func(env,"define",lick_f_define);
  lick_env_add_func(env,"set!",lick_f_set);
  lick_env_add_func(env,"lambda",lick_f_lambda);
  lick_env_add_func(env,"begin",lick_f_progn);
  lick_env_add_func(env,"progn",lick_f_progn);
  lick_env_add_func(env,"print",lick_f_print);
  lick_env_add_func(env,"println",lick_f_println);
  lick_env_add_func(env,"#print-env",lick_f_print_env);
}

lick_env_t*
lick_make_env(lick_env_t *outer) {
  lick_env_t *env = LICK_ALLOC(lick_env_t);
  env->outer = outer;
  env->entries = NULL;
  return env;
}

lick_env_t*
lick_make_default_env() {
  lick_env_t *env=lick_make_env(NULL);
  lick_env_set_defaults(env);
  return env;
}

void
lick_destroy_env(lick_env_t *env) {
  lick_env_entry_t *s,*tmp;
  HASH_ITER(hh, env->entries, s, tmp) {
    HASH_DEL(env->entries,s);
    free(s->name);
    if (s->value) {
      lick_free(s->value);
    }
    LICK_FREE(s);
  }
  LICK_FREE(env);
}

lick_env_t*
lick_env_add_var(lick_env_t *env,const char *str,lick_t *value) {
  lick_env_entry_t *entry = LICK_ALLOC(lick_env_entry_t);
  entry->name = strdup(str);
  entry->value = value;
  entry->func = NULL;
  HASH_ADD_STR(env->entries,name,entry);
  //printf("Adding %s to env %p (entries: %p)\n",str,env,env->entries);
  return env;
}

lick_env_t*
lick_env_add_func(lick_env_t *env,const char *str,lick_func_t func) {
  lick_env_entry_t *entry = LICK_ALLOC(lick_env_entry_t);
  entry->name = strdup(str);
  entry->value = NULL;
  entry->func = func;
  HASH_ADD_STR(env->entries,name,entry);
  return env;
}

lick_env_entry_t*
lick_env_find(lick_env_t *env,const char* name) {
  lick_env_entry_t *entry;
  HASH_FIND_STR(env->entries,name,entry);
  return entry;
}

lick_env_t*
lick_env_remove(lick_env_t *env,const char* name) {
  lick_env_entry_t *entry = lick_env_find(env,name);
  if (entry) {
    HASH_DEL(env->entries, entry);
    free(entry->name);
    if (entry->value) {
      lick_free(entry->value);
    }
    LICK_FREE(entry);
  }
  return env;
}

lick_env_entry_t*
lick_env_get(lick_env_t *env,const char *name) {
  lick_env_entry_t *entry = lick_env_find(env,name);
  lick_env_t *outer = env->outer;
  while (!entry && outer) {
    entry = lick_env_find(outer,name);
    outer = outer->outer;
  }
  return entry;
}


void
lick_env_print(lick_env_t *env) {
  lick_env_entry_t *entry,*tmp;
  HASH_ITER(hh, env->entries, entry, tmp) {
    printf("%s: ",entry->name);
    if (entry->value) {
      printf("[%d] ",entry->value->ref_count);
      lick_println(entry->value);
    }
    else if (entry->func) {
      printf(" function@%p\n",entry->func);
    }
    // if (env->outer) {
    //   lick_env_print(env->outer);
    // }
  }
}

lick_t*
lick_eval(lick_t* expr,lick_env_t *env) {
  // printf("Evaluating:\n");
  // lick_println(expr);
  //printf("exp->type: %d\n",expr->type);
  // lick_env_print(env);
  switch(expr->type) {
  case LICK_CONS:
    switch (expr->car->type) {
    case LICK_SYMBOL: {
      lick_env_entry_t *entry = lick_env_get(env,expr->car->string_val);
      if (entry) {
        if (entry->func) {
          return entry->func(expr->cdr,env);
        }
        else if (entry->value && entry->value->type==LICK_FUNC) {
          lick_env_t *e = entry->value->env;
          if (!e) {e = env;}
          return lick_apply(entry->value,expr->cdr,e);
        }
        else {
          printf("Invalid function %s\n",expr->car->string_val);
          return NULL;
        }
      }
    }
      break;
    default:
      printf("Unhandled expression: ");
      lick_println(expr);
      break;
    }
    break;
  case LICK_SYMBOL: {
    lick_env_entry_t *entry = lick_env_get(env,expr->string_val);
    if (entry && entry->value) {
      //printf("Returning symbol value for %s\n",expr->string_val);
      return lick_ref(entry->value);
    }
    else {
      printf("Invalid symbol %s\n",expr->string_val);
      return NULL;
    }
  }
    break;
  default:
    return lick_ref(expr);
  }
  printf("Unable to eval: ");
  lick_println(expr);
  lick_env_print(env);
  return NULL;
}


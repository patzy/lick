#ifndef LICK_H
#define LICK_H 1
#include <stdlib.h>
#include <malloc.h>
#include <uthash/uthash.h>
#include <lick/lexer.h>

#define LICK_ALLOC(T) (T*)(malloc(sizeof(T)))
#define LICK_FREE(T)   free(T)

void
lick_init();

void
lick_close();

typedef enum {
  LICK_INT,
  LICK_FLOAT,
  LICK_STRING,
  LICK_SYMBOL,
  LICK_CONS,
  LICK_FUNC,
  // constants
  LICK_NIL,
  LICK_TRUE
} lick_type_t;

typedef struct _lick_t {
  lick_type_t type;
  union {
    int   int_val;
    float float_val;
    char *string_val;
    struct {
      struct _lick_t *car;
      struct _lick_t *cdr;
    };
  };
  unsigned int ref_count;
  struct _lick_env_t *env;
} lick_t;

typedef lick_t*(*lick_func_t)(lick_t*,struct _lick_env_t*);


typedef struct {
  char*   name;
  lick_t *value;
  lick_func_t func;
  UT_hash_handle hh;
} lick_env_entry_t;

typedef struct _lick_env_t {
  struct _lick_env_t *outer;
  lick_env_entry_t   *entries;
} lick_env_t;

lick_t*
lick_int(int val);

lick_t*
lick_float(float val);

lick_t*
lick_string(const char* val);

lick_t*
lick_symbol(const char* val);

lick_t*
lick_cons(lick_t *car,lick_t *cdr);

lick_t*
lick_func(lick_t* args,lick_t* body);

lick_t*
lick_nil();

lick_t*
lick_true();

lick_t*
lick_append(lick_t *cons,lick_t* obj);

bool
lick_equal(lick_t *v1,lick_t *v2);

lick_t*
lick_car(lick_t *obj);

lick_t*
lick_cdr(lick_t *obj);

void
lick_free(lick_t* obj);

lick_t*
lick_apply(lick_t *func,lick_t* args,lick_env_t *env);

lick_t*
lick_parse(token_t *tokens);

void
lick_print(lick_t *obj);

inline
void
lick_println(lick_t *obj) {
  lick_print(obj);
  printf("\n");
}

lick_env_t*
lick_make_env(lick_env_t *outer);

lick_env_t*
lick_make_default_env();

void
lick_destroy_env(lick_env_t *env);

lick_env_t*
lick_env_add_var(lick_env_t *env,const char *name,lick_t *value);

lick_env_t*
lick_env_add_func(lick_env_t *env,const char *name,lick_func_t func);

lick_env_t*
lick_env_remove(lick_env_t *env,const char* name);

lick_env_entry_t*
lick_env_get(lick_env_t *env,const char *name);

void
lick_env_print(lick_env_t *env);

lick_t*
lick_eval(lick_t* expr,lick_env_t *env);

#endif // LICK_H

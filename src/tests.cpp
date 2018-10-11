#include <lick/lick.h>
#include <lick/lexer.h>
#include <string.h>

int
main(int argc,char **argv) {
  (void)argc;
  (void)argv;
  lick_init();

  // // C API
  // lick_t *obj;
  // token_t* tok;
  // obj = lick_cons(lick_int(10),lick_symbol("test"));
  // lick_println(obj);
  // lick_free(obj);


  // obj = lick_cons(lick_string("some"),
  //                 lick_cons(lick_string("string"),
  //                           lick_cons(lick_string("list"),
  //                                     lick_nil())));
  // lick_println(obj);
  // lick_free(obj);

  // obj = lick_cons(lick_string("some"),
  //                 lick_cons(lick_nil(),
  //                           lick_cons(lick_string("list"),
  //                                     lick_int(3))));
  // lick_println(obj);
  // lick_free(obj);

  // obj = lick_cons(lick_symbol("quote"),
  //                 lick_cons(lick_cons(lick_symbol("a"),
  //                           lick_cons(lick_symbol("b"),
  //                                     lick_cons(lick_symbol("c"),lick_nil()))),
  //                           lick_nil()));
  // lick_println(obj);
  // lick_free(obj);

  // obj = lick_append(NULL,lick_string("append"));
  // lick_append(obj,lick_int(10));
  // lick_append(obj,lick_float(5.4));
  // lick_println(obj);
  // lick_free(obj);

  // obj = lick_append(NULL,lick_string("append_cons"));
  // lick_append(obj,lick_cons(lick_symbol("some"),lick_append(NULL,lick_int(10))));
  // lick_println(obj);
  // lick_free(obj);

  int i = 0;
  const char* tok_strings[] = {
    "((blah (blih bloh)) 'some \"test string\" '(list of) 10 4.3 'a)",
    "   ; some comment\n (print \"some 'weird () ;string\");eol comment",
    "some_atom;and a comment",
    NULL
  };
  while (tok_strings[i] != NULL) {
    const char *str = tok_strings[i];
    printf("Tokenizing %s\n",str);
    token_t *tok = tokenize(str,strlen(str));
    token_print(tok);
    //printf("\n");
    token_free(tok);
    i++;
  }

  // printf("Eval.....\n");

  // const char *eval_strings[] = {
  //   "(quote (\"test\" 2))",
  //   "(cons 'a 'b)",
  //   "(cons (quote a) nil)",
  //   "(atomp '(a b c))",
  //   "(symbolp '(a b c))",
  //   "(symbolp 'a)",
  //   "(eq 'a 'a)",
  //   "(eq 'a 'b)",
  //   "(car (cdr (quote (a b c d e))))",
  //   "(cond ((eq 'a 'b) 'first) ((atomp 'a) 'second))",
  //   "(define foo 42)",
  //   "(define bar foo)",
  //   "foo",
  //   "bar",
  //   "(set! foo (cons \"some test string\" 34))",
  //   "foo",
  //   "(define pi 3.14)",
  //   "(define func (lambda (a b c) pi c (* a a)))",
  //   "(func 10 \"str\" 'sym)",
  //   "(begin (+ 3 4) (+ 4.3 4))",
  //   NULL
  // };
  // i = 0;
  // lick_env_t *env = lick_make_default_env();
  // while (eval_strings[i] != NULL) {
  //   const char *str = eval_strings[i];
  //   tok = tokenize(str,strlen(str));
  //   //token_print(tok);
  //   //printf("\n");
  //   obj = lick_parse(tok);
  //   token_free(tok);
  //   lick_println(obj);
  //   printf("\n");
  //   lick_t * res = lick_eval(obj,env);
  //   lick_free(obj);
  //   lick_println(res);
  //   lick_free(res);
  //   //lick_env_print(env);
  //   i++;
  // }
  // lick_destroy_env(env);

  lick_close();
}

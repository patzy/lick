#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <lick/lick.h>
#include <lick/lexer.h>
#include <string.h>

#define RESTART_SYSCALL(result,syscall)                                \
         do { (result)=(syscall); } while(((result)<0)&&(errno==EINTR));

void
ep(const char* input,lick_env_t *env) {
  token_t *tok = tokenize(input,strlen(input));
  if (tok) {
    lick_t *ast = lick_parse(tok);
    if (!ast) {
      printf("Parse error\n");
      return;
    }
    lick_t *res = lick_eval(ast,env);
    lick_free(ast);
    if (res) {
      lick_println(res);
      lick_free(res);
    }
    else {
      printf("No result\n");
    }
  }
  else {
    printf("Invalid input\n");
  }
  token_free(tok);
}

void
repl(lick_env_t* env) {
  while(true) {
    char input[255];
    printf("> ");
    if (!fgets(input,255,stdin)) {
      printf("\n");
      break;
    }
    ep(input,env);
  }
}

int
main(int argc,char **argv) {
  lick_init();
  lick_env_t *env = lick_make_default_env();
  if (argc > 1) {
    struct stat st;
    int32_t r;
    RESTART_SYSCALL(r,stat(argv[1],&st));
    if (r==-1) {
      printf("Unable to read %s\n",argv[1]);
      return -1;
    }
    size_t fsize = st.st_size;
    char *buf = (char*)malloc(fsize+8);
    strncpy(buf,"(progn ",8);
    FILE *f = fopen(argv[1],"r");
    if (!f) {
      printf("Unable to open %s\n",argv[1]);
      return -1;
    }
    size_t nread = fread(buf+7,sizeof(char),fsize,f);
    if (nread != fsize) {
      printf("Unable t read buffer\n");
      return -1;
    }
    buf[fsize+7] = ')';
    fclose(f);
    ep(buf,env);
    free(buf);
  }
  else {
    printf("Lick Interpreter\n");
    repl(env);
  }
  lick_destroy_env(env);
  lick_close();
  return 0;
}

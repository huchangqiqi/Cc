#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

char * READ(char prompt[])
{
  char *line;
  line = readline(prompt);
  if(!line) return NULL;
  add_history(line);
  return line;
}

char *EVAL(char *ast,void *env)
{
  return ast;
}

char *PRINT(char *exp)
{
  return exp;
}

int main()
{
  char *ast,*exp;
  char prompt[100];

  snprintf(prompt,sizeof(prompt),"user> ");

  for(;;)
    {
      ast = READ(prompt);
      if(!ast) return 0;
      exp = EVAL(ast,NULL);
      puts(PRINT(exp));

      free(ast);
    }

}

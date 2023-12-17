#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define BS_RL_BUFSIZE 512
#define BS_TOK_BUFSIZE 64
#define BS_TOK_DELIM " \t\r\n\a"
//RC support disabled cuz bugs
//#define BS_DEFAULT_RC_PATH "~/.zshrc"

#include "util.h"
#include "builtin.h"
#include "prompt.h"

int bs_execute(char **args){
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < bs_num_builtins(); i++) {
    if (strcmp(args[0], bs_builtin_str[i]) == 0) {
      return (*bs_builtin_func[i])(args);
    }
  }

  return bs_launch(args);
}
void bs_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    bs_set_prompt();
    line = bs_read_line();
    args = bs_split_line(line);
    status = bs_execute(args);

    free(line);
    free(args);
  } while (status);
}
void bs_script_loop(char* path)
{
  char *line;
  char **args;
  int status;

  FILE* file = fopen(path, "r");
  
  if (!file) {
    fprintf(stderr, "bs: no script at %s\n",path);
    exit(EXIT_FAILURE);
  }

  do {
    line = bs_read_line_from_script(file);
    if (line != NULL) {
      if(line[0] != '#'){
        args = bs_split_line(line);
        status = bs_execute(args);
        free(args);
      }else{
        status = 1;
      }
      free(line);
    }else {
      status = 0;  // End the loop if there are no more lines
    }
  } while (status);

  fclose(file);
}

int main(int argc, char **argv)
{
  ////load the RC file
  //char* rcpath = getenv("BSRC_PATH");
  //if (rcpath == NULL){
  //  rcpath = BS_DEFAULT_RC_PATH;
  //}
  //rcpath = bs_expand_tilde(rcpath);
  //if (rcpath == NULL){
  //  printf("this is not sopposed to happen,report this rcpath bug to the RoidsOS devs!\n");
  //  return EXIT_FAILURE;
  //}
  //bs_script_loop(rcpath);

  if (argc == 2){
    bs_script_loop(argv[1]);
  }else{
    bs_loop();
  }
  return EXIT_SUCCESS;
}
